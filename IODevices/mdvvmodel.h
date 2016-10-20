#ifndef MDVVMODEL_H
#define MDVVMODEL_H

#include <QObject>
#include <QTimer>
#include <QModbusRtuSerialMaster>
#include <QBitArray>

class MDVVModel : public QObject
{
    Q_OBJECT
public:
    explicit MDVVModel(QModbusRtuSerialMaster* modbusMaster, int address, QObject *parent = 0);

public slots:

    QBitArray getInputs();
    bool getInput(int index);

    QVector<int16_t> getOutputs();
    int16_t getOutput(int index);

    void setOutputs(QVector<int16_t> &outputs);
    void setOutput(int index, int16_t value);

    void setInterval(int msec);
    void start(int msec);
    void stop();
    int getAddress();

private:
    QBitArray *currentInputs, *prevInputs;
    QVector<int16_t> *currentOutputs, *prevOutputs;
    QTimer *timer;
    QModbusRtuSerialMaster* modbusMaster;
    int address;

private slots:
    void readInputs();
    void writeOutputs();
    void sendResponse();

    void readOutputsResponse();
    void readOutputs();

signals:
    void errorDetected(int address, QString errMessage);
    void updateInputs(int address);
    void updateInput(int address, int input);
    void updateOutputs(int address);
};

#endif // MDVVMODEL_H
