#include "dataminer.h"
#include <QTextStream>

DataMiner::DataMiner(QObject *parent) : QObject(parent)
{
    connectToRS485();
    initDevices();
}

int DataMiner::connectToRS485()
{
    modbusDevice = new QModbusRtuSerialMaster(this);
    if(!modbusDevice)   return -1;


    if (modbusDevice->state() != QModbusDevice::ConnectedState)
    {

        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, settings.port);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, settings.parity);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, settings.baud);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, settings.dataBits);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, settings.stopBits);

        modbusDevice->setTimeout(settings.responseTime);
        modbusDevice->setNumberOfRetries(settings.numberOfRetries);

        if(!modbusDevice->connectDevice())
        {
            QTextStream err(stderr);
            err << tr("Connect failed: ") + modbusDevice->errorString();
            emit errorDetected(0, tr("Connect failed: ") + modbusDevice->errorString());
            return -1;
        }
    }

    return 1;
}

int DataMiner::initDevices()
{
    if (!modbusDevice)
        return -1;

//    mva1 = new MVA8Model(modbusDevice, 1, this);
//    mva1->setEnabledInput(0, true);
//   // mva1->setEnabledInput(1, true);
//   // mva1->setEnabledInput(2, true);
//    mva1->start(400);

//    mdvv1 = new MDVVModel(modbusDevice, 16, this);
//    connect(mdvv1, &MDVVModel::updateInputs, this, &DataMiner::parseInputs);
//    connect(mdvv1, &MDVVModel::updateInput, this, &DataMiner::translateEvent);
//    connect(mdvv1, &MDVVModel::errorDetected, this, &DataMiner::translateError);
//    mdvv1->start(1000);
    
    //mdvv2 = new MDVVModel(modbusDevice, 32, this);
    //connect(mdvv2, &MDVVModel::updateInputs, this, &DataMiner::parseInputs);
    return 0;
}

QVariant DataMiner::getParameter(Parameters p)
{
    QVariant result;
    switch (p) {
    case Parameters::temperatureMain:
    {
        //result = mva1->getInput(0);//new QVariant(mva1->getInput(0));
    }
    default:
        break;
    }
    return result;
}

void DataMiner::parseInputs(int address)
{
//    switch (address) {
//    case 1:

//        break;
        
//    case 16:
//    {
//        if(mdvv1->getInput(0))

//        break;
//    }
//    case 32:
//        break;
        
//    default:
//        break;
//    }
}

void DataMiner::translateEvent(int address, int input)
{
    switch (address) {
    case 1:

        break;

//    case 16:
//    {
//        switch(input){
//        case 0:
//        {
//            if(mdvv1->getInput(input))
//            {
//                emit eventOccurred(Events::criticalCircuitReturnToNormal);
//            }
//            else
//            {
//                emit eventOccurred(Events::criticalCircuitDamaged);
//            }
//            break;
//        }
//        case 1:
//            break;
//        }

//        break;
//    }
    case 32:
        break;

    default:
        break;
    }
}

void DataMiner::translateError(int address, QString msg)
{
    emit errorDetected(address, msg);
}
