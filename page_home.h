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

private slots:
    void applyModuleState(ModuleStateMachine::Module module,
                          ModuleStateMachine::State state,
                          const QString &message);

private:
    void updateIndicator(QLabel *icon, QLabel *label,
                         ModuleStateMachine::State state,
                         const QString &message);

    QLabel *cameraStatus;
    QLabel *aiStatus;
    QLabel *rtspStatus;
    QLabel *sensorStatus;
    QLabel *alarmOverview;

    QLabel *cameraIcon;
    QLabel *aiIcon;
    QLabel *rtspIcon;
    QLabel *sensorIcon;
    QLabel *alarmIcon;

    ModuleStateMachine *m_stateMachine = nullptr;
};

#endif
