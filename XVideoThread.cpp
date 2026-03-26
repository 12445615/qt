#include "XVideoThread.h"
#include <QDebug>

XVideoThread::XVideoThread(QObject *parent)
    : QThread(parent)
{
    avformat_network_init();
    m_fmtCtx = nullptr;
    m_codecCtx = nullptr;
    m_swsCtx = nullptr;
    m_videoIndex = -1;
}

XVideoThread::~XVideoThread()
{
    requestInterruption();
    quit();
    wait();
    unInit();
    avformat_network_deinit();
}

void XVideoThread::setUrl(const QString &url)
{
    m_url = url;
}

bool XVideoThread::init()
{
    AVDictionary *opts = nullptr;
    av_dict_set(&opts,"rtmp_transport","tcp",0);
    av_dict_set(&opts,"stimeout","5000000",0);
    av_dict_set(&opts,"buffer_size","1024000",0);
    av_dict_set(&opts,"max_delay","500000",0);
    av_dict_set(&opts,"fflags","nobuffer",0);
    av_dict_set(&opts,"analyzeduration","1000000",0);
    av_dict_set(&opts,"probesize","1000000",0);

    if(avformat_open_input(&m_fmtCtx, m_url.toStdString().c_str(), nullptr, &opts) != 0){
        qDebug() << "打开流失败";
        av_dict_free(&opts);
        return false;
    }

    av_dict_free(&opts);
    avformat_find_stream_info(m_fmtCtx,nullptr);

    qDebug() << "初始化完成";
    return true;
}

void XVideoThread::unInit()
{
    if(m_swsCtx) sws_freeContext(m_swsCtx);
    if(m_codecCtx) avcodec_free_context(&m_codecCtx);
    if(m_fmtCtx) avformat_close_input(&m_fmtCtx);
}

QImage XVideoThread::frameToImage(AVFrame *frame)
{
    QImage img(frame->width, frame->height, QImage::Format_RGB32);
    uint8_t *dst[4] = { img.bits(), nullptr, nullptr, nullptr };
    int dst_linesize[4] = { (int)img.bytesPerLine(),0,0,0 };
    sws_scale(m_swsCtx,
              frame->data,
              frame->linesize,
              0,
              frame->height,
              dst,
              dst_linesize);
    qDebug() << "转换 QImage 完成, 宽:" << frame->width << " 高:" << frame->height;
    return img;
}

void XVideoThread::run()
{
    if(!init()){
        emit sig_sendInitState(false);
        return;
    }

    AVPacket pkt;
    AVFrame *frame = av_frame_alloc();
    bool hasEmitInit = false;

    while(!isInterruptionRequested() && av_read_frame(m_fmtCtx, &pkt) >= 0){

        // 第一次发现视频流索引
        if(m_videoIndex < 0) {
            for(unsigned int i=0;i<m_fmtCtx->nb_streams;i++){
                AVStream *st = m_fmtCtx->streams[i];
                if(st && st->codecpar && st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
                    m_videoIndex = i;
                    qDebug() << "识别视频流:" << m_videoIndex;
                    break;
                }
            }
            // 如果仍未找到，强制使用1路
            if(m_videoIndex < 0){
                qDebug() << "未找到视频流，强制视频流索引:1";
                m_videoIndex = 1;
            }
        }

        if(pkt.stream_index != m_videoIndex){
            av_packet_unref(&pkt);
            continue;
        }

        AVStream *st = m_fmtCtx->streams[m_videoIndex];

        // ⭐ 延迟初始化解码器
        if(!m_codecCtx){
            const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
            m_codecCtx = avcodec_alloc_context3(codec);
            if(st && st->codecpar) avcodec_parameters_to_context(m_codecCtx, st->codecpar);
            avcodec_open2(m_codecCtx, codec, nullptr);
            hasEmitInit = true;
            emit sig_sendInitState(true);
        }

        // ⭐ AVCC → Annex-B 转换
        int pos = 0;
        while(pos + 4 <= pkt.size){
            uint32_t nalu_size = (pkt.data[pos]<<24) | (pkt.data[pos+1]<<16) |
                                 (pkt.data[pos+2]<<8) | pkt.data[pos+3];
            pos += 4;
            if(pos + nalu_size > pkt.size) break;

            AVPacket pkt2;
            av_new_packet(&pkt2, nalu_size + 4);
            uint8_t start_code[4] = {0x00,0x00,0x00,0x01};
            memcpy(pkt2.data, start_code, 4);
            memcpy(pkt2.data+4, pkt.data+pos, nalu_size);
            pkt2.pts = pkt.pts;
            pkt2.dts = pkt.dts;

            avcodec_send_packet(m_codecCtx, &pkt2);
            while(avcodec_receive_frame(m_codecCtx, frame) == 0){
                if(!m_swsCtx){
                    m_swsCtx = sws_getContext(
                        frame->width, frame->height,
                        (AVPixelFormat)frame->format,
                        frame->width, frame->height,
                        AV_PIX_FMT_RGB32,
                        SWS_BICUBIC, nullptr, nullptr, nullptr);
                }
                QImage img = frameToImage(frame);
                emit sig_SendOneFrame(img);
            }
            av_packet_unref(&pkt2);
            pos += nalu_size;
        }

        av_packet_unref(&pkt);
    }

    av_frame_free(&frame);
    if(!hasEmitInit){
        emit sig_sendInitState(false);
    }
    unInit();
}
