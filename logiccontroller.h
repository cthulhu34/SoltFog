#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "devicecontroller.h"

class LogicController : public QObject
{
    Q_OBJECT


private:

    QTimer overallTimer;
    QTimer controlTimer;

    DeviceController *deviceController;
    float P, I, D;
    float POutVal, IOutVal, DOutVal;
    float TSetpoint;
    int period;


    enum MachineStatuses{
        delay,
        work,
        wait_for_command,
        manual_control
    };

    MachineStatuses refMachineStatus;
    int refMachineWorkTime;
    int refMachineDelayTime;

    QTimer refMachineTimer;

    float temperatureRegulator();
    float heaterRegulator();
    float refRegulator();

    QTimer airCompressorTimer;
    int airCompressorWorkTime;
    int airCompressorDelayTime;
    MachineStatuses airCompressorStatus;

    bool manualControl;

    bool TAchivement;

    bool resetIntegral;



public:
    explicit LogicController(QObject *parent = 0);

    void setPIDvalues(float P, float I, float D);

    float getPvalue();
    float getIvalue();
    float getDvalue();

    float getPOutValue();
    float getIOutValue();
    float getDOutValue();

    void setPValue(float value);
    void setIValue(float);
    void setDValue(float);

    void setIntegralReset(bool reset);

    float getTSetpoint();
    void setTSetpoint(float t);

    void startTesting();
    void stopTesting();

    void setAirCompressorWorkTime(int minutes);
    void setAirCompressorDelayTime(int minutes);

    void setTestingTime(int msec);

    int getHeaterStatus();
    bool getRefStatus();
    bool getAirCompressorStatus();
    bool getBrineCompressorStatus();

    void setManualControl(bool isManual);
    bool getControlMode();

    void setManualHeaterEnabled(bool isEnabled);
    void setManualRefEnabled(bool isEnabled);
    void setManualAirCompressorEnabled(bool isEnabled);
    void setManualBrineCompressorEnabled(bool isEnabled);

signals:
    void testingStopped(QString reason);


private slots:
    void IOControl();
    void refMachineChangeStatus();
    void airCompressorChangeStatus();
public slots:
    float getCurrentTemperatureValue();

};

#endif // LOGICCONTROLLER_H
