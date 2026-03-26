#ifndef PAGE_HOME_H
#define PAGE_HOME_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QTimer>

class PageHome : public QWidget
{
    Q_OBJECT

public:
    PageHome(QWidget *parent=nullptr);

private slots:
    void updateStatus();  // 模拟状态更新

private:
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

    QTimer *timer;
};

#endif
