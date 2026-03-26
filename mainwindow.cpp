#include "mainwindow.h"
#include <QHBoxLayout>
#include "page_home.h"
#include "page_camera.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0,0,800,480);
    central = new QWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(central);

    menu = new QListWidget;

    menu->addItem("首页");
    menu->addItem("视频监控");
    menu->addItem("环境监测");
    menu->addItem("报警中心");//resource
    menu->addItem("系统管理");//mechine

    stack = new QStackedWidget;

    home = new  PageHome();
    camera = new PageCamera();
    mechinemanager = new PageMechineManager();
    resourcemanager = new PageResourceManager();
    sensor = new PageSensor();

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
}

MainWindow::~MainWindow() {}
