#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "IODevices/lumelp18.h"

class DeviceController : public QObject
{
    Q_OBJECT

    enum Parameters{
        temperatureMain
    };

    enum Events{
        criticalCircuitDamaged,
        criticalCircuitReturnToNormal
    };

//Описание устройств ввода-вывода

//Описание датчиков и устройств дикретного ввода
    LumelP18 *THsensor = Q_NULLPTR; //Датчик температуры и влажности

//Описание исполнительных устройств


//Настройки, объекты и функции для связи по RS485
    struct RS485Settings {
        QString port = "ttyUSB0";
        int parity = QSerialPort::NoParity;
        int baud = QSerialPort::Baud115200;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;
        int responseTime = 1000;
        int numberOfRetries = 3;
    };

    QModbusRtuSerialMaster* modbusMaster;
    RS485Settings rs485settings;

    int connectToRS485();
public:
    explicit DeviceController(QObject *parent = 0);

    int initDevices();


signals:
    void errorDetected(int address, QString msg);
    void eventOccurred(Events e);

public slots:
};

#endif // DEVICECONTROLLER_H
