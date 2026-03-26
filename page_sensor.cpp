#include "page_sensor.h"
#include <QFrame>
#include <QRandomGenerator>

PageSensor::PageSensor(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color:#1e1e2f;color:white;");

    QLabel *title = new QLabel("环境监测仪表盘");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:26px;font-weight:bold;");

    tempValue = new QLabel("25 ℃");
    humiValue = new QLabel("60 %");
    smokeValue = new QLabel("正常");
    fireValue = new QLabel("未检测");
    airpressureValue = new QLabel("950 hPa");
    combustible_gasValue = new QLabel("未检测");
    QList<QLabel*> labels = {tempValue,humiValue,smokeValue,fireValue,airpressureValue,combustible_gasValue};

    for(auto l:labels)
    {
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet("font-size:24px;");
    }

    auto createCard = [](QString title,QLabel *value)
    {
        QFrame *frame = new QFrame();
        frame->setStyleSheet(
            "QFrame{"
            "background-color:#2c2c3e;"
            "border-radius:10px;"
            "}");

        QLabel *t = new QLabel(title);
        t->setAlignment(Qt::AlignCenter);
        t->setStyleSheet("font-size:18px;color:#aaa;");

        QVBoxLayout *layout = new QVBoxLayout(frame);
        layout->addWidget(t);
        layout->addWidget(value);

        return frame;
    };

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(createCard("温度",tempValue),0,0);
    grid->addWidget(createCard("湿度",humiValue),0,1);
    grid->addWidget(createCard("烟雾浓度",smokeValue),1,0);
    grid->addWidget(createCard("火焰检测",fireValue),1,1);
    grid->addWidget(createCard("可燃气体检测",combustible_gasValue),2,0);
    grid->addWidget(createCard("气压检测",airpressureValue),2,1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(grid);

    // 模拟实时数据
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&PageSensor::updateData);
    timer->start(2000);
}

void PageSensor::updateData()
{
    int temp = QRandomGenerator::global()->bounded(20,40);
    int humi = QRandomGenerator::global()->bounded(40,80);

    tempValue->setText(QString::number(temp) + " ℃");
    humiValue->setText(QString::number(humi) + " %");

    if(temp > 35)
        smokeValue->setText("异常");
    else
        smokeValue->setText("正常");
}
