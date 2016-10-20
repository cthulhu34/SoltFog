#ifndef LUMELP18_H
#define LUMELP18_H

#include <QObject>
#include <QVector>
#include <QModbusRtuSerialMaster>
#include <QTimer>

class LumelP18 : public QObject
{
    Q_OBJECT
public:
    explicit LumelP18(QModbusRtuSerialMaster *master, int addr, QObject *parent);

    float getTemperature();
    float getHumidity();
    float getDewPoint();

    void start(int msec);
    void stop();

private:
    QTimer *timer;
    QModbusRtuSerialMaster* modbusMaster;
    int address;
    QVector<float> inputs;

public slots:
    void readAnalogInputs();
    void sendReadResponse();

signals:
    void errorDetected(QString errMessage);
    void updateInputs();

};

#endif // LUMELP18_H
