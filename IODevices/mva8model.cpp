#include "mva8model.h"

#include <QTextStream>

MVA8Model::MVA8Model(QModbusRtuSerialMaster *modbusMaster, int address, QObject *parent) : QObject(parent)
{
    inputs.resize(8);
    inputs[0].reg= 0x4;
    inputs[1].reg= 0xA;
    inputs[2].reg= 0x10;
    inputs[3].reg= 0x15;
    inputs[4].reg= 0x1A;
    inputs[5].reg= 0x20;
    inputs[6].reg= 0x25;
    inputs[7].reg= 0x2A;

    currentInput = 0;

    this->modbusMaster = modbusMaster;
    this->address = address;

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
}

void MVA8Model::setInterval(int msec)
{
    if(!timer)
        return;
    timer->setInterval(msec);
}


void MVA8Model::sendResponse()
{
    if (!modbusMaster)
        return;

    while(!inputs[currentInput].enabled)
    {
        currentInput++;
        if(currentInput > 7)
            currentInput = 0;
    }
    if (auto *reply = modbusMaster->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters, inputs[currentInput].reg, 2), address)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MVA8Model::readInput);
        else
            delete reply; // broadcast replies return immediately
    } else {
        emit errorDetected(address, tr("Read error: ") + modbusMaster->errorString());
    }
}

void MVA8Model::readInput()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();


        int16_t t[2];
        t[1]=unit.value(0);
        t[0]=unit.value(1);

        QTextStream err(stderr);

        inputs[currentInput].value = (*((float*)t));
        err << tr("t in mva at input ") + QString::number(currentInput) + " = " + QString::number(inputs[currentInput].value, 'f', 2) + '\n';
        currentInput++;

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
    timer->singleShot(period, this, &MVA8Model::sendResponse);
}


float MVA8Model::getInput(int index)
{
    return inputs[index].value;
}


void MVA8Model::start(int msec)
{
    //timer = new QTimer(this);
//    if(!timer)
//    {
//        timer = new QTimer(this);
//        timer->setTimerType(Qt::PreciseTimer);
//    }
//    timer->setInterval(msec);
//    connect(timer, &QTimer::timeout, this, &MDVVModel::sendResponse);
//    timer->start();
    period = msec;
    timer->singleShot(period, this, &MVA8Model::sendResponse);

}

void MVA8Model::stop()
{
    //timer->stop();
    if(!timer) return;
    if(timer->isActive())
        timer->stop();
}

int MVA8Model::getAddress()
{
   return this->address;
}

void MVA8Model::setEnabledInput(int input, bool enabled)
{
    inputs[input].enabled = enabled;
}
