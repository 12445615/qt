#include "page_home.h"
#include <QRandomGenerator>
#include <QFont>
#include <QVBoxLayout>
#include <QGridLayout>

PageHome::PageHome(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color:#1e1e2f;color:white;");

    QLabel *title = new QLabel("智能多模态环境监测与消防预警系统");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:28px;font-weight:bold;");

    // 初始化状态文字
    cameraStatus = new QLabel("正常");
    aiStatus     = new QLabel("运行中");
    rtspStatus   = new QLabel("正常");
    sensorStatus = new QLabel("正常");
    alarmOverview= new QLabel("无");

    QList<QLabel*> statusLabels = {cameraStatus, aiStatus, rtspStatus, sensorStatus, alarmOverview};
    for(auto l : statusLabels)
    {
        l->setAlignment(Qt::AlignLeft);
        l->setStyleSheet("font-size:20px;");
    }

    // 初始化状态指示灯
    auto createIcon = []() {
        QLabel *icon = new QLabel;
        icon->setFixedSize(16,16);
        icon->setStyleSheet("background-color:green; border-radius:8px;");
        return icon;
    };

    cameraIcon = createIcon();
    aiIcon     = createIcon();
    rtspIcon   = createIcon();
    sensorIcon = createIcon();
    alarmIcon  = createIcon();

    QGridLayout *grid = new QGridLayout;
    grid->setHorizontalSpacing(30);
    grid->setVerticalSpacing(20);

    // 第一列: 指示灯
    grid->addWidget(cameraIcon, 0, 0);
    grid->addWidget(aiIcon, 1, 0);
    grid->addWidget(rtspIcon, 2, 0);
    grid->addWidget(sensorIcon, 3, 0);
    grid->addWidget(alarmIcon, 4, 0);

    // 第二列: 状态文字
    grid->addWidget(new QLabel("摄像头状态:"), 0, 1);
    grid->addWidget(new QLabel("AI检测状态:"), 1, 1);
    grid->addWidget(new QLabel("RTSP推流:"), 2, 1);
    grid->addWidget(new QLabel("传感器状态:"), 3, 1);
    grid->addWidget(new QLabel("最近报警:"), 4, 1);

    grid->addWidget(cameraStatus, 0, 2);
    grid->addWidget(aiStatus, 1, 2);
    grid->addWidget(rtspStatus, 2, 2);
    grid->addWidget(sensorStatus, 3, 2);
    grid->addWidget(alarmOverview, 4, 2);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(grid);
    mainLayout->addStretch();

    // 定时器模拟动态状态
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PageHome::updateStatus);
    timer->start(3000);
}

void PageHome::updateStatus()
{
    QStringList cam = {"正常","离线","异常"};
    QStringList ai  = {"运行中","停止","异常"};
    QStringList rtsp= {"正常","中断","异常"};
    QStringList sensor = {"正常","异常"};
    QStringList alarm  = {"无","火焰报警","烟雾报警","可燃气体异常"};

    auto randomIndex = [](int size){ return QRandomGenerator::global()->bounded(size); };

    cameraStatus->setText(cam[randomIndex(cam.size())]);
    aiStatus->setText(ai[randomIndex(ai.size())]);
    rtspStatus->setText(rtsp[randomIndex(rtsp.size())]);
    sensorStatus->setText(sensor[randomIndex(sensor.size())]);
    alarmOverview->setText(alarm[randomIndex(alarm.size())]);

    // 更新指示灯颜色
    auto updateIcon = [](QLabel *icon, const QString &status){
        QString color = "green";
        if(status.contains("异常") || status.contains("中断") || status.contains("离线")) color = "red";
        else if(status.contains("停止")) color = "yellow";
        icon->setStyleSheet(QString("background-color:%1;border-radius:8px;").arg(color));
    };

    updateIcon(cameraIcon, cameraStatus->text());
    updateIcon(aiIcon, aiStatus->text());
    updateIcon(rtspIcon, rtspStatus->text());
    updateIcon(sensorIcon, sensorStatus->text());
    updateIcon(alarmIcon, alarmOverview->text());

}
