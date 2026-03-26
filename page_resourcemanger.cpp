#include "page_resourcemanager.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QValueAxis>
#include <QPainter>

PageResourceManager::PageResourceManager(QWidget *parent)
    : QWidget(parent), timeIndex(0)
{
    // 整体黑色背景
    setStyleSheet("background-color:black; color:white;");

    QLabel *title = new QLabel("AI火灾风险分析中心");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:26px; font-weight:bold; color:white;");

    // 曲线系列
    series = new QLineSeries;
    series->setColor(QColor("#FF6B6B")); // 火焰橙红色

    chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("实时火灾概率曲线");
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setBackgroundBrush(QBrush(QColor("#0c121e"))); // 深蓝黑底

    // X 轴
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,50);
    axisX->setTitleText("时间");
    axisX->setLabelsColor(Qt::white);
    axisX->setGridLineColor(QColor("#394b61"));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y 轴
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,100);
    axisY->setTitleText("概率 %");
    axisY->setLabelsColor(Qt::white);
    axisY->setGridLineColor(QColor("#394b61"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(250);

    // 风险等级进度条
    riskBar = new QProgressBar;
    riskBar->setRange(0,100);
    riskBar->setStyleSheet(
        "QProgressBar {"
        "border: 1px solid #394b61;"
        "border-radius: 5px;"
        "background-color: #0c121e;"
        "text-align: center;"
        "color:white;"
        "}"
        "QProgressBar::chunk {"
        "background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "stop:0 #FF6B6B, stop:1 #FF8C42);"
        "}"
        );

    riskLabel = new QLabel("风险等级：低");
    riskLabel->setStyleSheet("font-size:20px; color:white;");

    // 报警记录表格
    alarmTable = new QTableWidget;
    alarmTable->setColumnCount(4);
    alarmTable->setHorizontalHeaderLabels({"时间","地点","类型","概率"});
    alarmTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    alarmTable->setStyleSheet(
        "QTableWidget {"
        "background-color: #0c121e;"
        "color: white;"
        "gridline-color: #394b61;"
        "}"
        "QHeaderView::section {"
        "background-color: #394b61;"
        "color: white;"
        "}"
        "QTableWidget::item:selected {"
        "background-color: #4a90e2;"
        "color: white;"
        "}"
        );

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(chartView);
    layout->addWidget(riskBar);
    layout->addWidget(riskLabel);
    layout->addWidget(alarmTable);
}

void PageResourceManager::updateAI(float fire, float smoke, float gas)
{
    float total = fire*0.6f + smoke*0.25f + gas*0.15f;
    int prob = total*100;

    // 曲线更新
    series->append(timeIndex++, prob);
    if(series->count() > 50)
        series->removePoints(0, series->count() - 50);

    // 更新 X 轴
    QValueAxis *axisX = qobject_cast<QValueAxis*>(chart->axisX());
    if(axisX)
        axisX->setRange(qMax(0, timeIndex-50), timeIndex);

    // 更新风险等级
    riskBar->setValue(prob);
    if(prob > 70)
    {
        riskLabel->setText("风险等级：高风险");

        QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
        addAlarm(time, "仓库A", prob);
    }
    else if(prob > 40)
        riskLabel->setText("风险等级：中风险");
    else
        riskLabel->setText("风险等级：低风险");
}

void PageResourceManager::addAlarm(const QString &time, const QString &location, float prob)
{
    int row = alarmTable->rowCount();
    alarmTable->insertRow(row);

    alarmTable->setItem(row, 0, new QTableWidgetItem(time));
    alarmTable->setItem(row, 1, new QTableWidgetItem(location));
    alarmTable->setItem(row, 2, new QTableWidgetItem("火灾报警"));
    alarmTable->setItem(row, 3, new QTableWidgetItem(QString::number(prob) + "%"));
}
