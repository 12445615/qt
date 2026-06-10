#ifndef PAGE_HOME_H
#define PAGE_HOME_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QTimer>

#include "ModuleStateMachine.h"

class PageHome : public QWidget
{
    Q_OBJECT

public:
    PageHome(QWidget *parent=nullptr);
    void setStateMachine(ModuleStateMachine *stateMachine);
    void updateCloudSummary(const QString &dataTime,
                            const QString &systemSafety,
                            const QString &recentAlarm,
                            const QString &aiState,
                            const QString &environmentState);

private slots:
    void applyModuleState(ModuleStateMachine::Module module,
                          ModuleStateMachine::State state,
                          const QString &message);

private:
    void updateIndicator(QLabel *icon, QLabel *label,
                         ModuleStateMachine::State state,
                         const QString &message);
    void setPlainStatus(QLabel *icon, QLabel *label,
                        const QString &message,
                        const QString &color);

    QLabel *mqttStatus;
    QLabel *dataTimeStatus;
    QLabel *systemSafetyStatus;
    QLabel *alarmOverview;
    QLabel *aiStatus;
    QLabel *environmentStatus;
    QLabel *rtmpStatus;
    QLabel *playbackStatus;

    QLabel *mqttIcon;
    QLabel *dataTimeIcon;
    QLabel *systemSafetyIcon;
    QLabel *alarmIcon;
    QLabel *aiIcon;
    QLabel *environmentIcon;
    QLabel *rtmpIcon;
    QLabel *playbackIcon;

    ModuleStateMachine *m_stateMachine = nullptr;
};

#endif
