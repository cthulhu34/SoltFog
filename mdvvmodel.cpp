#include "mdvvmodel.h"

MDVVModel::MDVVModel(QModbusRtuSerialMaster *modbusMaster, int address, QObject *parent) : QObject(parent)
{
    currentInputs = new QBitArray(12, 0);
    currentOutputs = new QVector<int16_t>(8, 0);
    prevInputs = new QBitArray(12, 0);
    prevOutputs = new QVector<int16_t>(8, 0);

    this->modbusMaster = modbusMaster;
    this->address = address;

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
}

void MDVVModel::setInterval(int msec)
{
    if(!timer)
        return;
    timer->setInterval(msec);
}


void MDVVModel::sendResponse()
{
    if (!modbusMaster)
        return;

    if (auto *reply = modbusMaster->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 51, 1), address)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MDVVModel::readInputs);
        else
            delete reply; // broadcast replies return immediately
    } else {
        emit errorDetected(address, tr("Read error: ") + modbusMaster->errorString());
    }
}

void MDVVModel::readInputs()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();


        int16_t dest;

        dest=unit.value(0);

        *prevInputs = *currentInputs;
        for(int i=0; i<currentInputs->size(); i++)
        {
            currentInputs->setBit(i, dest & (1<<i));
        }

        for(int i=0; i<currentInputs->size(); i++)
        {
            if(!(currentInputs->testBit(i) && prevInputs->testBit(i)))
                emit updateInput(address, i);
        }

        if(!(*currentInputs == *prevInputs))
        {
            emit updateInputs(address);
        }


    } else if (reply->error() == QModbusDevice::ProtocolError) {
        emit errorDetected(address, tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, address));
    } else {
        emit errorDetected(address, tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, address));
    }

    reply->deleteLater();
}

void MDVVModel::readOutputsResponse()
{
    if (!modbusMaster)
        return;

    if (auto *reply = modbusMaster->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 8), address)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MDVVModel::readOutputs);
        else
            delete reply; // broadcast replies return immediately
    } else {
        emit errorDetected(address, tr("Read error: ") + modbusMaster->errorString());
    }
}

void MDVVModel::readOutputs()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();

        *prevOutputs = *currentOutputs;

        for(int i=0; i<8; i++)
        {
            currentOutputs->replace(i, unit.value(i));
        }

        if(*prevOutputs != *currentOutputs)
            emit updateOutputs(address);


    } else if (reply->error() == QModbusDevice::ProtocolError) {
        emit errorDetected(address, tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, address));
    } else {
        emit errorDetected(address, tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, address));
    }

    reply->deleteLater();
}

QBitArray MDVVModel::getInputs()
{
    return *currentInputs;
}

bool MDVVModel::getInput(int index)
{
    return currentInputs->testBit(index);
}

QVector<int16_t> MDVVModel::getOutputs()
{
    return *currentOutputs;
}

int16_t MDVVModel::getOutput(int index)
{
    return currentOutputs->at(index);
}

void MDVVModel::writeOutputs()
{

    if (!modbusMaster)
        return;


    if(*currentOutputs == *prevOutputs)
       return;

    //emit errorDetected("Write outputs");
   //QModbusDataUnit request(QModbusDataUnit::HoldingRegisters, 0, *currentOutputs);
    QModbusDataUnit request(QModbusDataUnit::HoldingRegisters, 0, 8);
    for(int i=0; i<8; i++)
    {
        request.setValue(i, currentOutputs->value(i));
    }


    if (auto *reply = modbusMaster->sendWriteRequest(request, 16)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    emit errorDetected(address, tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    emit errorDetected(address, tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
                *prevOutputs=*currentOutputs;
                emit updateOutputs(address);
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        emit errorDetected(address, tr("Write error: ") + modbusMaster->errorString());
    }
}

void MDVVModel::setOutputs(QVector<int16_t> &outputs)
{
    *prevOutputs = *currentOutputs;
    *currentOutputs = outputs;
}

void MDVVModel::setOutput(int index, int16_t value)
{
    *prevOutputs = *currentOutputs;
    currentOutputs->replace(index, value);
}

void MDVVModel::start(int msec)
{
    //timer = new QTimer(this);
    if(!timer)
    {
        timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
    }
    timer->setInterval(msec);
    connect(timer, &QTimer::timeout, this, &MDVVModel::sendResponse);
    connect(timer, &QTimer::timeout, this, &MDVVModel::readOutputsResponse);
    connect(timer, &QTimer::timeout, this, &MDVVModel::writeOutputs);
    timer->start();

}

void MDVVModel::stop()
{
    //timer->stop();
    if(!timer) return;
    if(timer->isActive())
        timer->stop();
}

int MDVVModel::getAddress()
{
   return this->address;
}
