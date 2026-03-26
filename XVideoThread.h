#ifndef XVIDEOTHREAD_H
#define XVIDEOTHREAD_H

#include <QThread>
#include <QImage>
#include <QObject>
#include <QDebug>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/mem.h>
#include <libavutil/frame.h>
#include <libavcodec/bsf.h>
#include <libavcodec/parser.h>
}

/**
 * XVideoThread
 * -------------------------
 * 用于 RTMP/H264 视频流解码
 * 自动处理 codecpar 为空
 * 自动用 h264_mp4toannexb BSF 转 AnnexB
 * 输出 QImage 信号用于 Qt 显示
 */
class XVideoThread : public QThread
{
    Q_OBJECT
public:
    explicit XVideoThread(QObject *parent = nullptr);
    ~XVideoThread() override;

    void setUrl(const QString &url); // 设置流地址

signals:
    void sig_sendInitState(bool ok);    // 初始化成功或失败
    void sig_SendOneFrame(const QImage &img); // 发送一帧图片

protected:
    void run() override;

private:
    bool init();        // 初始化 AVFormatContext
    void unInit();      // 释放资源

    QImage frameToImage(AVFrame *frame);

private:
    QString m_url;

    AVFormatContext  *m_fmtCtx  = nullptr;
    AVCodecContext   *m_codecCtx = nullptr;
    SwsContext       *m_swsCtx  = nullptr;
    AVBSFContext     *m_bsfCtx  = nullptr; // H264 AVCC -> AnnexB 转换
    int m_videoIndex             = -1;
};

#endif // XVIDEOTHREAD_H
