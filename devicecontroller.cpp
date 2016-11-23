#include "devicecontroller.h"
#include <QTextStream>

#define MAX_PWM_VALUE 10000

DeviceController::DeviceController(QObject *parent) : QObject(parent)
{
    //setenv("WIRINGPI_GPIOMEM", "1", 1);
    initDevices();
}

DeviceController::~DeviceController()
{
    softPwmWrite(21, 100);
}

int DeviceController::initDevices()
{
    //init RS485 devices
    connectToRS485();
    if(modbusMaster->state() == QModbusDevice::ConnectedState)
    {
        THsensor = new LumelP18(modbusMaster, 3, this);
        THsensor->start(1000);
    }
    else
    {
        QTextStream err(stderr);
        err << tr("Connect failed: ") + modbusMaster->errorString();
    }

    //init onboard IOs
    //инициализация библиотеки wiringPi

    wiringPiSetup();
    //инициализация пинов
    pinMode(0, INPUT);
    brine_level_high = digitalRead(0); //уровень раствора в баке ВЫСОКИЙ
    pinMode(2, INPUT);
    brine_level_low = digitalRead(2);   //уровень раствора в баке НИЗКИЙ
    pinMode(3, INPUT);
    door_control = digitalRead(3);      //геркон двери
    pinMode(4, INPUT);
    pressure_R134 = digitalRead(4);     //реле давления R134

    connect(&updateGPIOInputsTimer, &QTimer::timeout, this, &DeviceController::updateGPIOInputs);
    updateGPIOInputsTimer.start(1000);

    air_compressor_on = false;              //выход для воздушного компрессора
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    brine_compressor_on = false;            //выход для насоса соляного раствора
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
    r134_supply = false;                    //выход на клапан подачи r134 на испаритель
    pinMode(22, OUTPUT);
    digitalWrite(22, HIGH);
    heater_pwm = 10;                         //софт ШИМ на ТЭНы
    softPwmCreate(21, 0, MAX_PWM_VALUE);
    softPwmWrite(21, MAX_PWM_VALUE);

    system_start = false;                   //пуск системы (вместо хардварной кнопки)
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);



    return 0;
}

int DeviceController::connectToRS485()
{
    modbusMaster = new QModbusRtuSerialMaster(this);
    if(!modbusMaster)   return -1;


    if (modbusMaster->state() != QModbusDevice::ConnectedState)
    {

        modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, rs485settings.port);
        modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, rs485settings.parity);
        modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, rs485settings.baud);
        modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, rs485settings.dataBits);
        modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, rs485settings.stopBits);

        modbusMaster->setTimeout(rs485settings.responseTime);
        modbusMaster->setNumberOfRetries(rs485settings.numberOfRetries);

        if(!modbusMaster->connectDevice())
        {
            QTextStream err(stderr);
            err << tr("Connect failed: ") + modbusMaster->errorString();
            emit errorDetected(0, tr("Connect failed: ") + modbusMaster->errorString());
            return -1;
        }
    }

    return 1;
}

QVariant DeviceController::getParameter(Parameters p)
{
    /*temperatureMain,
    air_compressor_status,
    brine_compressor_status,
    heater_pwm_value,
    r134_valve_status,
    brine_high,
    brine_low,
    door_status,
    pressure_R134_status*/

    QVariant result;
    switch (p) {
    case Parameters::temperatureMain:
        result = THsensor->getTemperature();
        break;
    case Parameters::brine_low:
        result = brine_level_low;
        break;
    case Parameters::brine_high:
        result = brine_level_high;
        break;
    case Parameters::brine_compressor_status:
        result = brine_compressor_on;
        break;
    case Parameters::air_compressor_status:
        result = air_compressor_on;
        break;
    case Parameters::r134_valve_status:
        result = r134_supply;
        break;
    case Parameters::heater_pwm_value:
        result = heater_pwm;
        break;
    default:
        break;
    }
    return result;
}

void DeviceController::updateGPIOInputs()
{
    brine_level_high = !digitalRead(0); //уровень раствора в баке ВЫСОКИЙ
    brine_level_low = !digitalRead(2);   //уровень раствора в баке НИЗКИЙ
    door_control = !digitalRead(3);      //геркон двери
    pressure_R134 = !digitalRead(4);     //реле давления R134

//    QTextStream err(stderr);
//    err << "\nLevel high = " + QString::number(brine_level_high);
//    err << "\nLevel low = " + QString::number(brine_level_low);
//    err << "\nLevel door control = " + QString::number(door_control);
//    err << "\nR134 pressure = " + QString::number(pressure_R134);

}

void DeviceController::setHeaterPwmValue(int value)
{
    int val = 0;
    if(value > 100){ val = 100; }
    else if(value < 0) {val = 0;}
    else val = value*100;

    softPwmWrite(21, MAX_PWM_VALUE - val);
    heater_pwm = val;
}

void DeviceController::commandsExec(Commands command)
{
    switch (command) {
    case Commands::air_compressor_turn_on:
        air_compressor_on = true;
        digitalWrite(5, LOW);
        break;
    case Commands::air_compressor_turn_off:
        air_compressor_on = false;
        digitalWrite(5, HIGH);
        break;
    case Commands::brine_comopressor_turn_on:
        brine_compressor_on = true;
        digitalWrite(6, LOW);
        break;
    case Commands::brine_compressor_turn_off:
        brine_compressor_on = false;
        digitalWrite(6, HIGH);
        break;
    case Commands::r134_supply_on:
        r134_supply = true;
        digitalWrite(22, LOW);
        break;
    case Commands::r134_supply_off:
        r134_supply = false;
        digitalWrite(22, HIGH);
        break;
    case Commands::system_turn_on:
        system_start = true;
        digitalWrite(27, LOW);
        break;
    case Commands::system_turn_off:
        system_start = false;
        digitalWrite(27, HIGH);
        break;
    default:
        break;
    }
}
