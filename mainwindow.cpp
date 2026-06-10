#include "mainwindow.h"
#include <QHBoxLayout>
#include "page_home.h"
#include "page_camera.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0,0,1280,720);
    central = new QWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(central);

    menu = new QListWidget;

    menu->addItem("首页");
    menu->addItem("视频监控");
    menu->addItem("环境监测");
    menu->addItem("报警中心");//resource
    menu->addItem("系统管理");//mechine

    stack = new QStackedWidget;

    stateMachine = new ModuleStateMachine(this);
    home = new  PageHome();
    camera = new PageCamera();
    mechinemanager = new PageMechineManager();
    resourcemanager = new PageResourceManager();
    sensor = new PageSensor();
    home->setStateMachine(stateMachine);

    stateMachine->transition(ModuleStateMachine::Camera,
                             ModuleStateMachine::Running,
                             QStringLiteral("正常"));
    stateMachine->transition(ModuleStateMachine::Ai,
                             ModuleStateMachine::Stopped,
                             QStringLiteral("未接入"));

    stack->addWidget(home);
    stack->addWidget(camera);
    stack->addWidget(sensor);
    stack->addWidget(resourcemanager);
    stack->addWidget(mechinemanager);

    layout->addWidget(menu);
    layout->addWidget(stack,1);

    setCentralWidget(central);

    connect(menu,&QListWidget::currentRowChanged,
            stack,&QStackedWidget::setCurrentIndex);

    connect(camera, &PageCamera::liveStreamStateChanged,
            this, [this](bool ok, const QString &message){
                stateMachine->transition(ModuleStateMachine::Rtmp,
                                         ok ? ModuleStateMachine::Running : ModuleStateMachine::Error,
                                         message);
            });

    connect(camera, &PageCamera::playbackStateChanged,
            this, [this](bool ok, const QString &message){
                stateMachine->transition(ModuleStateMachine::Camera,
                                         ok ? ModuleStateMachine::Running : ModuleStateMachine::Warning,
                                         ok ? QStringLiteral("回放正常") : message);
            });

    connect(sensor, &PageSensor::sensorConnectionStateChanged,
            this, [this](bool ok, const QString &message){
                stateMachine->transition(ModuleStateMachine::Sensor,
                                         ok ? ModuleStateMachine::Running : ModuleStateMachine::Warning,
                                         message);
            });

    connect(sensor, &PageSensor::sensorAlarmStateChanged,
            this, [this](bool ok, const QString &message){
                stateMachine->transition(ModuleStateMachine::Alarm,
                                         ok ? ModuleStateMachine::Running : ModuleStateMachine::Error,
                                         message);
            });

    connect(sensor, &PageSensor::homeSummaryUpdated,
            home, &PageHome::updateCloudSummary);
}

MainWindow::~MainWindow() {}
