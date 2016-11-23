#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVariant>
#include <QTimer>
#include "IODevices/lumelp18.h"
#include "wiringPi.h"
#include "softPwm.h"
#include <QTextStream>

class DeviceController : public QObject
{
    Q_OBJECT



//Описание устройств ввода-вывода

//Описание датчиков и устройств дикретного ввода
    LumelP18 *THsensor = Q_NULLPTR; //Датчик температуры и влажности

    bool brine_level_high; //уровень раствора в баке ВЫСОКИЙ
    bool brine_level_low;   //уровень раствора в баке НИЗКИЙ
    bool door_control;      //геркон двери TRUE - закрыта, FALSE - открыта
    bool pressure_R134;     //реле давление R134 TRUE - норма, FALSE - авария

//Описание исполнительных устройств

    bool air_compressor_on;     //включение воздушного компрессора
    bool brine_compressor_on;   //включение насоса соляного раствора
    int  heater_pwm;            //значение для софтверного ШИМа на ТЭНы
    bool r134_supply;           //подача R134 на испаритель
    bool system_start;



//Настройки, объекты и функции для связи по RS485
    struct RS485Settings {
        QString port = "ttyUSB0";
        int parity = QSerialPort::NoParity;
        int baud = QSerialPort::Baud57600;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;
        int responseTime = 300;
        int numberOfRetries = 3;
    };

    QModbusRtuSerialMaster* modbusMaster;
    RS485Settings rs485settings;

    int connectToRS485();

    //Таймеры
    QTimer updateGPIOInputsTimer;

    //Контроль входов-выходов
    void updateGPIOInputs();


public:

    enum Parameters{
        temperatureMain,
        air_compressor_status,
        brine_compressor_status,
        heater_pwm_value,
        r134_valve_status,
        brine_high,
        brine_low,
        door_status,
        pressure_R134_status
    };

    enum Commands{
        air_compressor_turn_on,
        air_compressor_turn_off,
        brine_comopressor_turn_on,
        brine_compressor_turn_off,
        r134_supply_on,
        r134_supply_off,
        system_turn_on,
        system_turn_off
    };

    enum Events{
        criticalCircuitDamaged,
        criticalCircuitReturnToNormal
    };

    explicit DeviceController(QObject *parent = 0);
    ~DeviceController();

    int initDevices();

    QVariant getParameter(Parameters p);


    //выполнение поступающих команд
    void commandsExec(Commands command);

signals:
    void errorDetected(int address, QString msg);
    void eventOccurred(Events e);

public slots:
    void setHeaterPwmValue(int value);

};

#endif // DEVICECONTROLLER_H
