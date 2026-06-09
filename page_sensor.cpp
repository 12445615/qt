#include "page_sensor.h"

#include <QFrame>
#include <QProcessEnvironment>

PageSensor::PageSensor(QWidget *parent)
    : QWidget(parent),
      mqttStatusLabel(new QLabel(QStringLiteral("MQTT: 未配置"), this)),
      tempValue(new QLabel(QStringLiteral("-- ℃"), this)),
      humiValue(new QLabel(QStringLiteral("-- %"), this)),
      smokeValue(new QLabel(QStringLiteral("--"), this)),
      fireValue(new QLabel(QStringLiteral("未检测"), this)),
      combustible_gasValue(new QLabel(QStringLiteral("--"), this)),
      airpressureValue(new QLabel(QStringLiteral("-- hPa"), this)),
      aiDetectStateValue(new QLabel(QStringLiteral("无目标"), this)),
      mqttClient(new AliyunMqttClient(this)),
      mqttReconnectTimer(new QTimer(this))
{
    setStyleSheet(QStringLiteral("background-color:#1e1e2f;color:white;"));

    QLabel *title = new QLabel(QStringLiteral("环境监测仪表盘"));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(QStringLiteral("font-size:26px;font-weight:bold;"));

    mqttStatusLabel->setAlignment(Qt::AlignCenter);
    mqttStatusLabel->setStyleSheet(QStringLiteral("font-size:14px;color:#f0c674;"));

    QList<QLabel *> labels = {
        tempValue,
        humiValue,
        smokeValue,
        fireValue,
        airpressureValue,
        combustible_gasValue,
        aiDetectStateValue
    };

    for (QLabel *label : labels) {
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(QStringLiteral("font-size:24px;"));
    }

    auto createCard = [](const QString &titleText, QLabel *valueLabel) {
        QFrame *frame = new QFrame();
        frame->setStyleSheet(
            "QFrame{"
            "background-color:#2c2c3e;"
            "border-radius:10px;"
            "}");

        QLabel *titleLabel = new QLabel(titleText);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QStringLiteral("font-size:18px;color:#aaa;"));

        QVBoxLayout *layout = new QVBoxLayout(frame);
        layout->addWidget(titleLabel);
        layout->addWidget(valueLabel);

        return frame;
    };

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(createCard(QStringLiteral("温度"), tempValue), 0, 0);
    grid->addWidget(createCard(QStringLiteral("湿度"), humiValue), 0, 1);
    grid->addWidget(createCard(QStringLiteral("烟雾浓度"), smokeValue), 1, 0);
    grid->addWidget(createCard(QStringLiteral("火焰检测"), fireValue), 1, 1);
    grid->addWidget(createCard(QStringLiteral("可燃气体检测"), combustible_gasValue), 2, 0);
    grid->addWidget(createCard(QStringLiteral("气压检测"), airpressureValue), 2, 1);
    grid->addWidget(createCard(QStringLiteral("AI状态"), aiDetectStateValue), 3, 0, 1, 2);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addWidget(mqttStatusLabel);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(grid);

    mqttReconnectTimer->setInterval(5000);
    connect(mqttReconnectTimer, &QTimer::timeout,
            this, &PageSensor::reconnectMqttIfNeeded);

    connect(mqttClient, &AliyunMqttClient::sensorDataReceived,
            this, &PageSensor::applySensorData);
    connect(mqttClient, &AliyunMqttClient::stateChanged,
            this, &PageSensor::updateMqttState);
    connect(mqttClient, &AliyunMqttClient::errorOccurred,
            this, &PageSensor::showMqttError);

    initMqttClient();
}

void PageSensor::applySensorData(const AliyunSensorData &data)
{
    if (data.hasTemperature)
        tempValue->setText(formatNumber(data.temperature) + QStringLiteral(" ℃"));

    if (data.hasHumidity)
        humiValue->setText(formatNumber(data.humidity) + QStringLiteral(" %"));

    if (data.hasSmoke)
        smokeValue->setText(formatNumber(data.smoke) + QStringLiteral(" %"));

    if (data.hasFire)
        fireValue->setText(data.fireDetected ? QStringLiteral("检测到火焰") : QStringLiteral("未检测"));

    if (data.hasCombustibleGas)
        combustible_gasValue->setText(formatNumber(data.combustibleGas) + QStringLiteral(" ppm"));

    if (data.hasAirPressure)
        airpressureValue->setText(formatNumber(data.airPressure) + QStringLiteral(" hPa"));

    if (data.hasAiDetectState)
        aiDetectStateValue->setText(aiDetectStateText(data.aiDetectState));

    const bool alarm =
        (data.hasSmoke && data.smoke > 0.0)
        || (data.hasFire && data.fireDetected)
        || (data.hasCombustibleGas && data.combustibleGasDetected);

    if(alarm)
        emit sensorAlarmStateChanged(false, QStringLiteral("传感器报警"));
    else
        emit sensorAlarmStateChanged(true, QStringLiteral("无"));
}

void PageSensor::updateMqttState(QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::Disconnected:
        updateStatusLabel(QStringLiteral("MQTT: 已断开"), QStringLiteral("#e06c75"));
        emit sensorConnectionStateChanged(false, QStringLiteral("已断开"));
        if (!mqttReconnectTimer->isActive())
            mqttReconnectTimer->start();
        break;
    case QMqttClient::Connecting:
        updateStatusLabel(QStringLiteral("MQTT: 连接中"), QStringLiteral("#f0c674"));
        emit sensorConnectionStateChanged(false, QStringLiteral("连接中"));
        if (!mqttReconnectTimer->isActive())
            mqttReconnectTimer->start();
        break;
    case QMqttClient::Connected:
        updateStatusLabel(QStringLiteral("MQTT: 已连接"), QStringLiteral("#98c379"));
        emit sensorConnectionStateChanged(true, QStringLiteral("正常"));
        mqttReconnectTimer->stop();
        break;
    }
}

void PageSensor::showMqttError(const QString &message)
{
    updateStatusLabel(QStringLiteral("MQTT: %1").arg(message), QStringLiteral("#e06c75"));
    emit sensorConnectionStateChanged(false, message);
    if (mqttClient->state() != QMqttClient::Connected && !mqttReconnectTimer->isActive())
        mqttReconnectTimer->start();
}

void PageSensor::reconnectMqttIfNeeded()
{
    if (mqttClient->state() == QMqttClient::Connected) {
        mqttReconnectTimer->stop();
        return;
    }

    if (mqttClient->state() == QMqttClient::Connecting)
        return;

    updateStatusLabel(QStringLiteral("MQTT: 正在自动重连"), QStringLiteral("#f0c674"));
    emit sensorConnectionStateChanged(false, QStringLiteral("正在自动重连"));
    mqttClient->connectToAliyun();
}

void PageSensor::initMqttClient()
{
    const QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    const QString productKey = env.value(QStringLiteral("ALIYUN_PRODUCT_KEY"),QStringLiteral("k29ovUMboAH"));
    const QString deviceName = env.value(QStringLiteral("ALIYUN_DEVICE_NAME"),QStringLiteral("0122-qt"));
    const QString deviceSecret = env.value(QStringLiteral("ALIYUN_DEVICE_SECRET"),QStringLiteral("28971840ce3e479526c8a41a8c3ae2a6"));
    const QString regionId = env.value(QStringLiteral("ALIYUN_REGION_ID"), QStringLiteral("cn-shanghai"));
    const QString mqttHostUrl = env.value(QStringLiteral("ALIYUN_MQTT_HOST_URL"),
                                          QStringLiteral("iot-06z00be8pk7p1uz.mqtt.iothub.aliyuncs.com"));
    const QString clientId = env.value(QStringLiteral("ALIYUN_CLIENT_ID"),
                                       QStringLiteral("k29ovUMboAH.0122-qt"));
    const QString customTopic = env.value(QStringLiteral("ALIYUN_SUB_TOPIC"));

    if (productKey.isEmpty() || deviceName.isEmpty() || deviceSecret.isEmpty()) {
        updateStatusLabel(QStringLiteral("MQTT: 缺少阿里云环境变量配置"), QStringLiteral("#f0c674"));
        emit sensorConnectionStateChanged(false, QStringLiteral("配置不完整"));
        return;
    }

    AliyunMqttClient::Config config;
    config.productKey = productKey;
    config.deviceName = deviceName;
    config.deviceSecret = deviceSecret;
    config.regionId = regionId;
    config.mqttHostUrl = mqttHostUrl;
    config.clientId = clientId;
    config.port = 8883;

    if (!customTopic.isEmpty())
        config.subscribeTopics << customTopic;

    mqttClient->configure(config);
    mqttClient->connectToAliyun();
}

void PageSensor::updateStatusLabel(const QString &text, const QString &color)
{
    mqttStatusLabel->setText(text);
    mqttStatusLabel->setStyleSheet(
        QStringLiteral("font-size:14px;color:%1;").arg(color));
}

QString PageSensor::formatNumber(double value, int precision)
{
    return QString::number(value, 'f', precision);
}

QString PageSensor::aiDetectStateText(int state)
{
    switch (state) {
    case 0:
        return QStringLiteral("无目标");
    case 1:
        return QStringLiteral("PPE合规");
    case 2:
        return QStringLiteral("未戴安全帽");
    case 3:
        return QStringLiteral("未穿防护服");
    case 4:
        return QStringLiteral("同时PPE不合规");
    case 5:
        return QStringLiteral("工作区火光");
    case 6:
        return QStringLiteral("非工作区火光");
    case 7:
        return QStringLiteral("危险区人员闯入");
    default:
        return QStringLiteral("未知状态 %1").arg(state);
    }
}
