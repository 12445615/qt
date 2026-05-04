#include "page_home.h"
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
    cameraStatus = new QLabel("未检测");
    aiStatus     = new QLabel("未启动");
    rtspStatus   = new QLabel("未连接");
    sensorStatus = new QLabel("未连接");
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
        icon->setStyleSheet("background-color:gray; border-radius:8px;");
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
    grid->addWidget(new QLabel("RTMP推流:"), 2, 1);
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

}

void PageHome::setStateMachine(ModuleStateMachine *stateMachine)
{
    if(m_stateMachine)
        disconnect(m_stateMachine, nullptr, this, nullptr);

    m_stateMachine = stateMachine;
    if(!m_stateMachine)
        return;

    connect(m_stateMachine, &ModuleStateMachine::moduleStateChanged,
            this, &PageHome::applyModuleState);

    applyModuleState(ModuleStateMachine::Camera,
                     m_stateMachine->state(ModuleStateMachine::Camera),
                     m_stateMachine->message(ModuleStateMachine::Camera));
    applyModuleState(ModuleStateMachine::Ai,
                     m_stateMachine->state(ModuleStateMachine::Ai),
                     m_stateMachine->message(ModuleStateMachine::Ai));
    applyModuleState(ModuleStateMachine::Rtmp,
                     m_stateMachine->state(ModuleStateMachine::Rtmp),
                     m_stateMachine->message(ModuleStateMachine::Rtmp));
    applyModuleState(ModuleStateMachine::Sensor,
                     m_stateMachine->state(ModuleStateMachine::Sensor),
                     m_stateMachine->message(ModuleStateMachine::Sensor));
    applyModuleState(ModuleStateMachine::Alarm,
                     m_stateMachine->state(ModuleStateMachine::Alarm),
                     m_stateMachine->message(ModuleStateMachine::Alarm));
}

void PageHome::applyModuleState(ModuleStateMachine::Module module,
                                ModuleStateMachine::State state,
                                const QString &message)
{
    switch(module) {
    case ModuleStateMachine::Camera:
        updateIndicator(cameraIcon, cameraStatus, state, message);
        break;
    case ModuleStateMachine::Ai:
        updateIndicator(aiIcon, aiStatus, state, message);
        break;
    case ModuleStateMachine::Rtmp:
        updateIndicator(rtspIcon, rtspStatus, state, message);
        break;
    case ModuleStateMachine::Sensor:
        updateIndicator(sensorIcon, sensorStatus, state, message);
        break;
    case ModuleStateMachine::Alarm:
        updateIndicator(alarmIcon, alarmOverview, state, message);
        break;
    }
}

void PageHome::updateIndicator(QLabel *icon, QLabel *label,
                               ModuleStateMachine::State state,
                               const QString &message)
{
    QString color = "gray";
    switch(state) {
    case ModuleStateMachine::Unknown:
        color = "gray";
        break;
    case ModuleStateMachine::Starting:
    case ModuleStateMachine::Warning:
        color = "#f0c674";
        break;
    case ModuleStateMachine::Running:
        color = "#98c379";
        break;
    case ModuleStateMachine::Error:
    case ModuleStateMachine::Stopped:
        color = "#e06c75";
        break;
    }

    label->setText(message.isEmpty() ? QStringLiteral("未知") : message);
    icon->setStyleSheet(QStringLiteral("background-color:%1;border-radius:8px;").arg(color));
}
