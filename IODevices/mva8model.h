#ifndef MVA8MODEL_H
#define MVA8MODEL_H

#include <QObject>
#include <QTimer>
#include <QModbusRtuSerialMaster>

class MVA8Model : public QObject
{
    Q_OBJECT

    struct AnalogInput{
        bool enabled = false;
        float value = 0;
        int reg = 0;
    };

public:
    explicit MVA8Model(QModbusRtuSerialMaster* modbusMaster, int address, QObject *parent = 0);


public slots:

    float getInput(int index);

    void setInterval(int msec);
    void start(int msec);
    void stop();
    int getAddress();
    void setEnabledInput(int input, bool enabled);

private:
    QVector<AnalogInput> inputs;
    QTimer *timer;
    QModbusRtuSerialMaster* modbusMaster;
    int address;
    int currentInput;
    int period;

private slots:
    void readInput();
    void sendResponse();

signals:
    void errorDetected(int address, QString errMessage);
    void updateInput(int address, int input);
    void updateAllInputs(int address);
};

#endif // MVA8MODEL_H
