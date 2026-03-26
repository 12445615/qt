#ifndef PAGE_SENSOR_H
#define PAGE_SENSOR_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTimer>


class PageSensor : public QWidget
{
    Q_OBJECT

public:
    PageSensor(QWidget *parent=nullptr);

private slots:
    void updateData();

private:
    QLabel *tempValue;
    QLabel *humiValue;
    QLabel *smokeValue;
    QLabel *fireValue;
    QLabel *combustible_gasValue;
    QLabel *airpressureValue;
    QTimer *timer;
};


#endif // PAGE_SENSOR_H
