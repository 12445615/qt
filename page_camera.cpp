#include "page_camera.h"
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QUrl>
#include <QLineEdit>
#include <QPushButton>
#include <QThread>
#include <QTextCursor>
#include <QDebug>
#include <QLabel>

#include "XVideoThread.h"

PageCamera::PageCamera(QWidget *parent) : QWidget(parent)
{
    videoLayout();
    scanVideoFiles();

    pushButton[0]->setCheckable(true);

    connect(pushButton[0], &QPushButton::clicked, this, &PageCamera::btn_play_clicked);
    connect(pushButton[1], &QPushButton::clicked, this, &PageCamera::btn_next_clicked);
    connect(pushButton[2], &QPushButton::clicked, this, &PageCamera::btn_volmedown_clicked);
    connect(pushButton[3], &QPushButton::clicked, this, &PageCamera::btn_volmeup_clicked);
    connect(pushButton[4], &QPushButton::clicked, this, &PageCamera::btn_fullscreen_clicked);

    connect(listWidget, &QListWidget::itemClicked, this, &PageCamera::listWidgetClicked);

    videoThread = nullptr;

    // ================= RTMP 播放 =================
    connect(rtmpPlayBtn, &QPushButton::clicked, this, [=](){

        QString url = rtmpLineEdit->text().trimmed();
        if(url.isEmpty()) return;

        currentRTMPUrl = url;

        appendAIResult(QString("播放 RTMP 地址：%1").arg(url));

        // 停止旧线程
        if(videoThread){
            videoThread->requestInterruption();
            videoThread->wait();
            delete videoThread;
            videoThread = nullptr;
        }

        videoThread = new XVideoThread(this);
        videoThread->setUrl(url);

        // 线程安全更新UI
        connect(videoThread, &XVideoThread::sig_SendOneFrame,
                this,
                [=](const QImage &img){
                    videoLabel->setPixmap(
                        QPixmap::fromImage(img).scaled(
                            videoLabel->size(),
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
                },
                Qt::QueuedConnection);

        videoThread->start();
    });

    // ================= 进度条 =================
    connect(this, &PageCamera::positionChanged, this, [=](qint64 pos){
        if (!durationSlider->isSliderDown())
            durationSlider->setValue(pos / 1000);

        int sec = pos / 1000;
        int min = sec / 60;
        sec %= 60;

        label[0]->setText(QString("%1:%2")
                              .arg(min,2,10,QChar('0'))
                              .arg(sec,2,10,QChar('0')));
    });

    connect(this, &PageCamera::durationChanged, this, [=](qint64 dur){
        durationSlider->setRange(0, dur / 1000);

        int sec = dur / 1000;
        int min = sec / 60;
        sec %= 60;

        label[1]->setText(QString("/%1:%2")
                              .arg(min,2,10,QChar('0'))
                              .arg(sec,2,10,QChar('0')));
    });
}

PageCamera::~PageCamera()
{
    if(videoThread){
        videoThread->requestInterruption();
        videoThread->wait();
        delete videoThread;
        videoThread = nullptr;
    }
}

void PageCamera::videoLayout()
{
    this->resize(800,480);

    // ================= FFmpeg 显示用 =================
    videoLabel = new QLabel(this);
    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setStyleSheet("background:black;");
    videoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    listWidget = new QListWidget();
    listWidget->setMinimumWidth(250);

    durationSlider = new QSlider(Qt::Horizontal);

    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0,100);
    volumeSlider->setValue(50);

    for(int i=0;i<5;i++)
        pushButton[i] = new QPushButton(this);

    pushButton[0]->setText("Play");
    pushButton[1]->setText("Next");
    pushButton[2]->setText("-");
    pushButton[3]->setText("+");
    pushButton[4]->setText("Full");

    label[0] = new QLabel("00:00", this);
    label[1] = new QLabel("/00:00", this);

    aiTitleLabel = new QLabel("AI Detection Result", this);

    aiResultText = new QTextEdit(this);
    aiResultText->setReadOnly(true);
    aiResultText->setMinimumHeight(90);
    aiResultText->setStyleSheet(
        "QTextEdit{background:#111;color:#00ff7f;border:1px solid #444;font-family:Consolas;}");

    // ================= 布局 =================
    topLayout = new QHBoxLayout();
    topLayout->addWidget(videoLabel);   // ⭐ 改这里
    topLayout->addWidget(listWidget);

    controlLayout = new QHBoxLayout();
    controlLayout->addWidget(pushButton[0]);
    controlLayout->addWidget(pushButton[1]);
    controlLayout->addWidget(pushButton[2]);
    controlLayout->addWidget(volumeSlider);
    controlLayout->addWidget(pushButton[3]);
    controlLayout->addWidget(label[0]);
    controlLayout->addWidget(label[1]);
    controlLayout->addStretch();
    controlLayout->addWidget(pushButton[4]);

    QHBoxLayout *rtmpLayout = new QHBoxLayout();
    rtmpLineEdit = new QLineEdit(this);
    rtmpLineEdit->setPlaceholderText("请输入 RTMP 地址，例如 rtmp://192.168.1.100/live/stream");

    rtmpPlayBtn = new QPushButton("播放", this);

    rtmpLayout->addWidget(rtmpLineEdit);
    rtmpLayout->addWidget(rtmpPlayBtn);

    QVBoxLayout *aiLayout = new QVBoxLayout();
    aiLayout->addWidget(aiTitleLabel);
    aiLayout->addWidget(aiResultText);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(durationSlider);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(rtmpLayout);
    mainLayout->addLayout(aiLayout);

    setLayout(mainLayout);
}

void PageCamera::scanVideoFiles()
{
    QDir dir(QCoreApplication::applicationDirPath()+"/myVideo");
    if(!dir.exists()) return;

    QStringList filter{"*.mp4","*.mkv","*.avi"};
    QFileInfoList files = dir.entryInfoList(filter,QDir::Files);

    for(const QFileInfo &file: files){
        MediaObjectInfo info{file.fileName(),file.absoluteFilePath()};
        mediaObjectInfo.append(info);
        listWidget->addItem(info.fileName);
    }
}

void PageCamera::appendAIResult(const QString &result)
{
    aiResultText->append(result);
    QTextCursor cursor = aiResultText->textCursor();
    cursor.movePosition(QTextCursor::End);
    aiResultText->setTextCursor(cursor);
}

// ================= 按钮逻辑 =================

void PageCamera::btn_play_clicked()
{

}

void PageCamera::btn_next_clicked()
{
    if(mediaObjectInfo.isEmpty()) return;

    currentIndex++;
    if(currentIndex >= mediaObjectInfo.size())
        currentIndex = 0;
}

void PageCamera::btn_volmeup_clicked()
{
    volumeSlider->setValue(qMin(volumeSlider->value()+5,100));
}

void PageCamera::btn_volmedown_clicked()
{
    volumeSlider->setValue(qMax(volumeSlider->value()-5,0));
}

void PageCamera::btn_fullscreen_clicked()
{
    if(videoLabel->isFullScreen()){
        videoLabel->showNormal();
        listWidget->setVisible(true);
    } else {
        videoLabel->showFullScreen();
        listWidget->setVisible(false);
    }
}

void PageCamera::listWidgetClicked(QListWidgetItem *item)
{
    currentIndex = listWidget->row(item);
}
