#include "devicecontroller.h"
#include <QTextStream>

DeviceController::DeviceController(QObject *parent) : QObject(parent)
{

}

int DeviceController::initDevices()
{
    //init RS485 devices


    //init I2C devices


    //init onboard IOs
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
