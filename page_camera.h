#ifndef PAGE_CAMERA_H
#define PAGE_CAMERA_H

#include <QWidget>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QThread>
#include <QImage>
#include "XVideoThread.h"

class XVideoThread;

struct MediaObjectInfo {
    QString fileName;
    QString filePath;
};

class PageCamera : public QWidget
{
    Q_OBJECT
public:
    explicit PageCamera(QWidget *parent = nullptr);
    ~PageCamera() override;

signals:
    void frameReady(const QImage &img);
    void positionChanged(qint64 pos);      // 播放进度
    void durationChanged(qint64 duration); // 视频总时长

private slots:
    void btn_play_clicked();
    void btn_next_clicked();
    void btn_volmeup_clicked();
    void btn_volmedown_clicked();
    void btn_fullscreen_clicked();
    void listWidgetClicked(QListWidgetItem *item);


private:
    void videoLayout();
    void mediaPlayerInit();
    void scanVideoFiles();
    void appendAIResult(const QString &result);


    // UI 控件
    QVideoWidget *videoWidget;
    QPushButton *pushButton[5];
    QSlider *durationSlider;
    QSlider *volumeSlider;
    QLabel *label[2];       // 改成指针
    QListWidget *listWidget;
    QTextEdit *aiResultText;
    QLabel *aiTitleLabel;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QHBoxLayout *controlLayout;
    QLineEdit *rtmpLineEdit;
    QPushButton *rtmpPlayBtn;
    QLabel *videoLabel;

    // 播放状态
    QList<MediaObjectInfo> mediaObjectInfo;
    int currentIndex = 0;
    QString currentRTMPUrl;

    XVideoThread *videoThread; // 播放线程
};

#endif // PAGE_CAMERA_H
