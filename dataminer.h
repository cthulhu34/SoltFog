#ifndef DATAMINER_H
#define DATAMINER_H

#include <QObject>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QTimer>
#include <QVariant>
#include <QDateTime>

#include "mva8model.h"
#include "mdvvmodel.h"

class DataMiner : public QObject
{
    Q_OBJECT
public:

    enum Parameters{
        temperatureMain
    };

    enum Events{
        criticalCircuitDamaged,
        criticalCircuitReturnToNormal
    };

    explicit DataMiner(QObject *parent = 0);

    QVariant getParameter(Parameters p);


private:
    struct Settings {
        QString port = "ttyUSB0";
        int parity = QSerialPort::NoParity;
        int baud = QSerialPort::Baud115200;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;
        int responseTime = 1000;
        int numberOfRetries = 3;
    };

    QModbusRtuSerialMaster* modbusDevice;
    Settings settings;

    MVA8Model* mva1 = Q_NULLPTR;
    MDVVModel* mdvv1 = Q_NULLPTR;
    MDVVModel* mdvv2 = Q_NULLPTR;
    //float currentTemperature, currentHumidity;

    int connectToRS485();
    int initDevices();

signals:
    void errorDetected(int address, QString msg);
    void eventOccurred(Events e);


private slots:
    void parseInputs(int address);
    void translateEvent(int address, int input);
    void translateError(int address, QString msg);

public slots:
};

#endif // DATAMINER_H
