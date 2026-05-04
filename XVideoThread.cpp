#include "XVideoThread.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QMutex>
#include <cstdio>

namespace {

QMutex g_ffmpegNetworkMutex;
int g_ffmpegNetworkUsers = 0;

void retainFfmpegNetwork()
{
    QMutexLocker locker(&g_ffmpegNetworkMutex);
    if(g_ffmpegNetworkUsers == 0)
        avformat_network_init();
    ++g_ffmpegNetworkUsers;
}

void releaseFfmpegNetwork()
{
    QMutexLocker locker(&g_ffmpegNetworkMutex);
    if(g_ffmpegNetworkUsers <= 0)
        return;

    --g_ffmpegNetworkUsers;
    if(g_ffmpegNetworkUsers == 0)
        avformat_network_deinit();
}

} // namespace

XVideoThread::XVideoThread(QObject *parent)
    : QThread(parent)
{
    fprintf(stderr, "[RTMP][thread] create XVideoThread=%p\n", static_cast<void *>(this));
    retainFfmpegNetwork();
    m_fmtCtx = nullptr;
    m_codecCtx = nullptr;
    m_swsCtx = nullptr;
    m_videoIndex = -1;
}

XVideoThread::~XVideoThread()
{
    fprintf(stderr, "[RTMP][thread] destroy begin XVideoThread=%p\n", static_cast<void *>(this));
    requestInterruption();
    quit();
    wait();
    unInit();
    releaseFfmpegNetwork();
    fprintf(stderr, "[RTMP][thread] destroy end XVideoThread=%p\n", static_cast<void *>(this));
}

void XVideoThread::setUrl(const QString &url)
{
    m_url = url;
    fprintf(stderr, "[RTMP][config] set url=%s\n", m_url.toUtf8().constData());
}

bool XVideoThread::init()
{
    QElapsedTimer initTimer;
    initTimer.start();

    fprintf(stderr, "[RTMP][init] begin url=%s\n", m_url.toUtf8().constData());
    fprintf(stderr,
            "[RTMP][ffmpeg] compile libavformat=%u runtime=%u compile libavcodec=%u runtime=%u compile libavutil=%u runtime=%u\n",
            LIBAVFORMAT_VERSION_INT,
            avformat_version(),
            LIBAVCODEC_VERSION_INT,
            avcodec_version(),
            LIBAVUTIL_VERSION_INT,
            avutil_version());

    AVDictionary *opts = nullptr;
    const bool isRtmp = m_url.startsWith(QStringLiteral("rtmp://"), Qt::CaseInsensitive)
                        || m_url.startsWith(QStringLiteral("rtmps://"), Qt::CaseInsensitive);

    av_dict_set(&opts,"rtmp_transport","tcp",0);
    av_dict_set(&opts,"stimeout","5000000",0);
    av_dict_set(&opts,"rw_timeout","5000000",0);
    av_dict_set(&opts,"buffer_size", isRtmp ? "327680" : "1024000", 0);
    av_dict_set(&opts,"max_delay", isRtmp ? "100000" : "300000", 0);
    av_dict_set(&opts,"analyzeduration", isRtmp ? "1500000" : "3000000", 0);
    av_dict_set(&opts,"probesize", isRtmp ? "1048576" : "2097152", 0);
    if(isRtmp) {
        av_dict_set(&opts,"fflags","nobuffer",0);
        av_dict_set(&opts,"flags","low_delay",0);
    }

    const QByteArray urlBytes = m_url.toUtf8();
    fprintf(stderr, "[RTMP][init] avformat_open_input start\n");
    int ret = avformat_open_input(&m_fmtCtx, urlBytes.constData(), nullptr, &opts);
    if(ret < 0){
        const QString message = QString("打开流失败: %1, url=%2")
                                    .arg(avErrorString(ret), m_url);
        qWarning().noquote() << message;
        emit sig_errorMessage(message);
        av_dict_free(&opts);
        return false;
    }
    fprintf(stderr, "[RTMP][init][cost] avformat_open_input ok cost=%lldms fmtCtx=%p\n",
            initTimer.elapsed(),
            static_cast<void *>(m_fmtCtx));

    av_dict_free(&opts);
    fprintf(stderr, "[RTMP][init] avformat_find_stream_info start\n");
    const qint64 findInfoStartMs = initTimer.elapsed();
    ret = avformat_find_stream_info(m_fmtCtx,nullptr);
    if(ret < 0){
        const QString message = QString("读取流信息失败: %1, url=%2")
                                    .arg(avErrorString(ret), m_url);
        qWarning().noquote() << message;
        emit sig_errorMessage(message);
        unInit();
        return false;
    }
    fprintf(stderr, "[RTMP][init][cost] avformat_find_stream_info ok step=%lldms total=%lldms nb_streams=%u\n",
            initTimer.elapsed() - findInfoStartMs,
            initTimer.elapsed(),
            m_fmtCtx->nb_streams);
    av_dump_format(m_fmtCtx, 0, urlBytes.constData(), 0);

    fprintf(stderr, "[RTMP][init] stream list begin\n");
    for(unsigned int i = 0; i < m_fmtCtx->nb_streams; ++i){
        AVStream *stream = m_fmtCtx->streams[i];
        AVCodecParameters *par = stream ? stream->codecpar : nullptr;
        fprintf(stderr,
                "[RTMP][init] stream[%u] type=%s codec_id=%d width=%d height=%d extradata=%d\n",
                i,
                par ? av_get_media_type_string(par->codec_type) : "null",
                par ? par->codec_id : -1,
                par ? par->width : 0,
                par ? par->height : 0,
                par ? par->extradata_size : 0);
    }
    fprintf(stderr, "[RTMP][init] stream list end\n");

    fprintf(stderr, "[RTMP][init] av_find_best_stream start\n");
    ret = av_find_best_stream(m_fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if(ret < 0){
        const QString message = QString("未找到视频流: %1, url=%2")
                                    .arg(avErrorString(ret), m_url);
        qWarning().noquote() << message;
        emit sig_errorMessage(message);
        unInit();
        return false;
    }

    m_videoIndex = ret;
    fprintf(stderr, "[RTMP][init] selected video stream index=%d\n", m_videoIndex);
    AVStream *st = m_fmtCtx->streams[m_videoIndex];
    if(!st || !st->codecpar){
        fprintf(stderr,
                "[RTMP][init] selected stream has empty codecpar, fallback scanning streams\n");
        m_videoIndex = -1;
        for(unsigned int i = 0; i < m_fmtCtx->nb_streams; ++i){
            AVStream *candidate = m_fmtCtx->streams[i];
            AVCodecParameters *par = candidate ? candidate->codecpar : nullptr;
            if(par && par->codec_type == AVMEDIA_TYPE_VIDEO){
                m_videoIndex = static_cast<int>(i);
                st = candidate;
                fprintf(stderr, "[RTMP][init] fallback selected video stream index=%d\n", m_videoIndex);
                break;
            }
        }

        if(m_videoIndex < 0 || !st || !st->codecpar){
            const QString message = QStringLiteral("视频流参数为空");
            qWarning().noquote() << message;
            emit sig_errorMessage(message);
            unInit();
            return false;
        }
    }

    const AVCodec *codec = avcodec_find_decoder(st->codecpar->codec_id);
    if(!codec){
        const QString message = QString("未找到解码器: codec_id=%1")
                                    .arg(st->codecpar->codec_id);
        qWarning().noquote() << message;
        emit sig_errorMessage(message);
        unInit();
        return false;
    }
    fprintf(stderr, "[RTMP][init] decoder found name=%s codec_id=%d\n",
            codec->name, st->codecpar->codec_id);

    m_codecCtx = avcodec_alloc_context3(codec);
    if(!m_codecCtx){
        const QString message = QStringLiteral("创建解码器上下文失败");
        qWarning().noquote() << message;
        emit sig_errorMessage(message);
        unInit();
        return false;
    }

    fprintf(stderr, "[RTMP][init] avcodec_parameters_to_context start\n");
    ret = avcodec_parameters_to_context(m_codecCtx, st->codecpar);
    if(ret < 0){
        const QString message = QString("复制解码参数失败: %1")
                                    .arg(avErrorString(ret));
        qWarning().noquote() << message;
        emit sig_errorMessage(message);
        unInit();
        return false;
    }

    fprintf(stderr, "[RTMP][init] avcodec_open2 start\n");
    const qint64 openCodecStartMs = initTimer.elapsed();
    ret = avcodec_open2(m_codecCtx, codec, nullptr);
    if(ret < 0){
        const QString message = QString("打开解码器失败: %1, codec=%2")
                                    .arg(avErrorString(ret), QString::fromUtf8(codec->name));
        qWarning().noquote() << message;
        emit sig_errorMessage(message);
        unInit();
        return false;
    }

    fprintf(stderr,
            "[RTMP][init][cost] ok total=%lldms codecOpen=%lldms videoIndex=%d decoder=%s width=%d height=%d pix_fmt=%d\n",
            initTimer.elapsed(),
            initTimer.elapsed() - openCodecStartMs,
            m_videoIndex,
            codec->name,
            m_codecCtx->width,
            m_codecCtx->height,
            m_codecCtx->pix_fmt);
    qDebug() << "初始化完成, 视频流:" << m_videoIndex << "解码器:" << codec->name;
    return true;
}

void XVideoThread::unInit()
{
    fprintf(stderr,
            "[RTMP][cleanup] begin bsf=%p sws=%p codec=%p fmt=%p\n",
            static_cast<void *>(m_bsfCtx),
            static_cast<void *>(m_swsCtx),
            static_cast<void *>(m_codecCtx),
            static_cast<void *>(m_fmtCtx));
    if(m_bsfCtx) av_bsf_free(&m_bsfCtx);
    if(m_swsCtx) sws_freeContext(m_swsCtx);
    if(m_codecCtx) avcodec_free_context(&m_codecCtx);
    if(m_fmtCtx) avformat_close_input(&m_fmtCtx);
    m_swsCtx = nullptr;
    m_codecCtx = nullptr;
    m_fmtCtx = nullptr;
    m_videoIndex = -1;
    fprintf(stderr, "[RTMP][cleanup] end\n");
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

QString XVideoThread::avErrorString(int errorCode) const
{
    char buffer[AV_ERROR_MAX_STRING_SIZE] = {0};
    av_strerror(errorCode, buffer, sizeof(buffer));
    return QString::fromUtf8(buffer);
}

void XVideoThread::run()
{
    fprintf(stderr, "[RTMP][run] thread run begin XVideoThread=%p\n", static_cast<void *>(this));
    if(!init()){
        fprintf(stderr, "[RTMP][run] init failed\n");
        emit sig_sendInitState(false);
        return;
    }

    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    if(!pkt || !frame){
        fprintf(stderr, "[RTMP][run] alloc packet/frame failed pkt=%p frame=%p\n",
                static_cast<void *>(pkt),
                static_cast<void *>(frame));
        emit sig_errorMessage(QStringLiteral("分配 FFmpeg 帧/包失败"));
        emit sig_sendInitState(false);
        av_packet_free(&pkt);
        av_frame_free(&frame);
        unInit();
        return;
    }

    emit sig_sendInitState(true);
    bool emittedFrame = false;
    int readPacketCount = 0;
    int videoPacketCount = 0;
    int decodedFrameCount = 0;

    while(!isInterruptionRequested()){
        const int readRet = av_read_frame(m_fmtCtx, pkt);
        if(readRet < 0){
            fprintf(stderr, "[RTMP][read] av_read_frame failed ret=%d error=%s\n",
                    readRet,
                    avErrorString(readRet).toUtf8().constData());
            if(readRet != AVERROR_EOF){
                emit sig_errorMessage(QString("读取视频帧失败: %1").arg(avErrorString(readRet)));
            }
            break;
        }

        ++readPacketCount;
        if(readPacketCount <= 20 || readPacketCount % 100 == 0){
            fprintf(stderr,
                    "[RTMP][read] packet #%d stream=%d size=%d pts=%lld dts=%lld\n",
                    readPacketCount,
                    pkt->stream_index,
                    pkt->size,
                    static_cast<long long>(pkt->pts),
                    static_cast<long long>(pkt->dts));
        }

        if(pkt->stream_index != m_videoIndex){
            av_packet_unref(pkt);
            continue;
        }

        ++videoPacketCount;
        const int sendRet = avcodec_send_packet(m_codecCtx, pkt);
        av_packet_unref(pkt);
        if(sendRet < 0){
            fprintf(stderr, "[RTMP][decode] avcodec_send_packet failed ret=%d error=%s\n",
                    sendRet,
                    avErrorString(sendRet).toUtf8().constData());
            emit sig_errorMessage(QString("发送视频包到解码器失败: %1").arg(avErrorString(sendRet)));
            continue;
        }

        while(!isInterruptionRequested()){
            const int recvRet = avcodec_receive_frame(m_codecCtx, frame);
            if(recvRet == AVERROR(EAGAIN) || recvRet == AVERROR_EOF)
                break;
            if(recvRet < 0){
                fprintf(stderr, "[RTMP][decode] avcodec_receive_frame failed ret=%d error=%s\n",
                        recvRet,
                        avErrorString(recvRet).toUtf8().constData());
                emit sig_errorMessage(QString("解码视频帧失败: %1").arg(avErrorString(recvRet)));
                break;
            }

            if(!m_swsCtx){
                fprintf(stderr,
                        "[RTMP][sws] create sws width=%d height=%d pix_fmt=%d\n",
                        frame->width,
                        frame->height,
                        frame->format);
                m_swsCtx = sws_getContext(
                    frame->width, frame->height,
                    (AVPixelFormat)frame->format,
                    frame->width, frame->height,
                    AV_PIX_FMT_RGB32,
                    SWS_BICUBIC, nullptr, nullptr, nullptr);
                if(!m_swsCtx){
                    fprintf(stderr, "[RTMP][sws] create sws failed\n");
                    emit sig_errorMessage(QStringLiteral("创建像素格式转换器失败"));
                    av_frame_unref(frame);
                    continue;
                }
            }

            QImage img = frameToImage(frame);
            emit sig_SendOneFrame(img);
            emittedFrame = true;
            ++decodedFrameCount;
            if(decodedFrameCount <= 10 || decodedFrameCount % 50 == 0){
                fprintf(stderr,
                        "[RTMP][decode] frame #%d width=%d height=%d format=%d videoPackets=%d totalPackets=%d\n",
                        decodedFrameCount,
                        frame->width,
                        frame->height,
                        frame->format,
                        videoPacketCount,
                        readPacketCount);
            }
            av_frame_unref(frame);
        }
    }

    if(!emittedFrame){
        fprintf(stderr,
                "[RTMP][run] exit without decoded frame, totalPackets=%d videoPackets=%d\n",
                readPacketCount,
                videoPacketCount);
        emit sig_sendInitState(false);
    }

    av_packet_free(&pkt);
    av_frame_free(&frame);
    unInit();
    fprintf(stderr,
            "[RTMP][run] thread run end totalPackets=%d videoPackets=%d decodedFrames=%d interrupted=%d\n",
            readPacketCount,
            videoPacketCount,
            decodedFrameCount,
            isInterruptionRequested() ? 1 : 0);
}
