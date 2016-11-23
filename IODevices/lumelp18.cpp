#include "lumelp18.h"
#include <QTextStream>

LumelP18::LumelP18(QModbusRtuSerialMaster *master, int addr, QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);

    this->modbusMaster = master;
    this->address = addr;

    inputs.resize(3);
    inputs.fill(0);
}


void LumelP18::sendReadResponse()
{
    if (!modbusMaster)
        return;

    if (auto *reply = modbusMaster->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 7002, 6), address)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &LumelP18::readAnalogInputs);
        else
            delete reply; // broadcast replies return immediately
    } else {
        emit errorDetected(tr("Read error: ") + modbusMaster->errorString());
        QTextStream err(stderr);
        err << tr("Read error: ") + modbusMaster->errorString();
    }
}

void LumelP18::readAnalogInputs()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();

        for(int i=0; i<inputs.size(); i++)
        {
            int16_t t[2];
            t[1]=unit.value(i*2);
            t[0]=unit.value(i*2+1);
            inputs.replace(i, *((float*)t));
        }

       // QTextStream err(stderr);
       // err << tr("\ncur Temp =: ") + QString::number(inputs.at(0), 'f', 5);
        emit updateInputs();

    } else if (reply->error() == QModbusDevice::ProtocolError) {
        emit errorDetected(tr("Read response error: %1 (Modbus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, address));
        QTextStream err(stderr);
        err << tr("Read response error: %1 (Modbus exception: 0x%2)").
               arg(reply->errorString()).
               arg(reply->rawResult().exceptionCode(), -1, address);
    } else {
        emit errorDetected(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, address));
        QTextStream err(stderr);
        err << tr("Read response error: %1 (Modbus exception: 0x%2)").
               arg(reply->errorString()).
               arg(reply->error(), -1, address);
    }

    reply->deleteLater();
}


float LumelP18::getTemperature()
{
    return inputs.at(0);
}

float LumelP18::getHumidity()
{
    return inputs.at(1);
}

float LumelP18::getDewPoint()
{
    return inputs.at(2);
}

void LumelP18::start(int msec)
{
    if(!modbusMaster)
        return;
    if(!timer)
        timer = new QTimer(this);
    timer->setInterval(msec);
    connect(timer, &QTimer::timeout, this, &LumelP18::sendReadResponse);
    timer->start();
}

void LumelP18::stop()
{
    if(!modbusMaster)
        return;
    if(!timer)
        return;
    if(timer->isActive())
        timer->stop();
}

