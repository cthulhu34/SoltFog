#include "logiccontroller.h"

#include <QTextStream>

LogicController::LogicController(QObject *parent) : QObject(parent)
{
    deviceController = new DeviceController(this);

    manualControl = false;

    period = 6000;
    connect(&controlTimer, &QTimer::timeout, this, &LogicController::IOControl);

    refMachineStatus = MachineStatuses::wait_for_command;
    refMachineWorkTime = 2000;
    refMachineDelayTime = 10000;
    refMachineTimer.setSingleShot(true);
    connect(&refMachineTimer, &QTimer::timeout, this, &LogicController::refMachineChangeStatus);

    airCompressorStatus = MachineStatuses::wait_for_command;
    airCompressorWorkTime = 900000;
    airCompressorDelayTime = 2700000;
    airCompressorTimer.setSingleShot(true);
    connect(&airCompressorTimer, &QTimer::timeout, this, &LogicController::airCompressorChangeStatus);

    //overallTimer.setInterval(604800000);

    P = 0;
    I = 0;
    D = 0;

    POutVal = 0;
    IOutVal = 0;
    DOutVal = 0;
}

float LogicController::getCurrentTemperatureValue()
{
    return (deviceController->getParameter(DeviceController::temperatureMain)).toFloat();
}

float LogicController::temperatureRegulator()
{
    float curTemperatureValue;
    static float prevTemperatureValue = 0;
    static float errSum = 0;

    float out, Pout, Iout, Dout;

    curTemperatureValue = getCurrentTemperatureValue();

    Pout = P*(TSetpoint - curTemperatureValue);
    errSum += TSetpoint - curTemperatureValue;
    Iout = I * errSum * ((float)period/(float)1000);
    Dout = D * ((TSetpoint - curTemperatureValue) - (TSetpoint - prevTemperatureValue))/(period/1000);
    out = Pout + Iout + Dout;




    prevTemperatureValue = curTemperatureValue;

    return out;

    /*signed int pid_control(signed char setpoint, signed char currentpoint) {
     static signed int last_error;
     signed int P;
     signed int D;
     static signed int I;
     signed int PID;
      P = (setpoint - currentpoint);
      I = (I + (setpoint - currentpoint) * INTERVAL);
      D = (((setpoint - currentpoint) - last_error) / INTERVAL);
      last_error = setpoint - currentpoint;
      PID = (Kp * P) + (Ki * I) + (Kd * D);
     return (PID);
    }*/

}

float LogicController::heaterRegulator()
{
    float curTemperatureValue;
    static float prevTemperatureValue = 0;
    static float errSum = 0;

    float out, Pout, Iout, Dout;

    if(resetIntegral)
    {
        errSum = 0;
        resetIntegral = false;
    }
    curTemperatureValue = getCurrentTemperatureValue();
    float dT = TSetpoint - curTemperatureValue;
    //Pout = P*(TSetpoint - curTemperatureValue);
    Pout = P*dT;
    if(!((dT>2) or (dT<-2)))
    {
        errSum += TSetpoint - curTemperatureValue;

    }
    else
    {
        errSum = 0;

    }
    Iout = I * errSum * (float)period/(float)1000;
    //Dout = D * ((float)((TSetpoint - curTemperatureValue) - (TSetpoint - prevTemperatureValue))/((float)period/(float)1000));
    Dout = D * ((float)((- curTemperatureValue + prevTemperatureValue))/((float)period/(float)1000));
    out = Pout + Iout  + Dout;
    //if (out>100) out=100;
    //if (out<0)   out=0;

    prevTemperatureValue = curTemperatureValue;

    POutVal = Pout;
    IOutVal = Iout;
    DOutVal = Dout;

    QTextStream err(stderr);

    err << "\nTempRegulator: Pout = " + QString::number(Pout, 'f', 2) + " Iout = " + QString::number(Iout, 'f', 2) + " Dout = " + QString::number(Dout, 'f', 2);
    err << "\nOut = " + QString::number(out);
    err << "\nErrSum = " + QString::number(errSum);
    err << "\nprevT = " + QString::number(prevTemperatureValue);

    return out;
}

void LogicController::setIntegralReset(bool reset)
{
    resetIntegral = reset;
}

float LogicController::refRegulator()
{
    float curTemperatureValue;
    static float prevTemperatureValue = 0;

    float out, Pout, Dout;
    curTemperatureValue = getCurrentTemperatureValue();

    if ((TSetpoint - curTemperatureValue) < 0)
    {
        Pout = P*(TSetpoint - curTemperatureValue);
        Dout = D * ((TSetpoint - curTemperatureValue) - (TSetpoint - prevTemperatureValue))/period;
        out = - (Pout + Dout);
    }
    else
    {
        out = 0;
    }
    if (out>100) out=100;
    if (out<0)   out=0;
    prevTemperatureValue = curTemperatureValue;

    return out;
}

void LogicController::setPIDvalues(float P, float I, float D)
{
    this->P = P;
    this->I = I;
    this->D = D;
}

float LogicController::getPvalue()
{
    return P;
}

float LogicController::getIvalue()
{
    return I;
}

float LogicController::getDvalue()
{
    return D;
}

float LogicController::getPOutValue()
{
    return POutVal;
}

float LogicController::getIOutValue()
{
    return IOutVal;
}

float LogicController::getDOutValue()
{
    return DOutVal;
}

void LogicController::IOControl()
{
    if(!manualControl)
    {
        //Управление ТЭНами
        if(overallTimer.isActive())
        {
            int regOut = heaterRegulator();
            int heatReg = regOut;//heaterRegulator();
            if(heatReg>100) heatReg = 100;
            if(heatReg<0)   heatReg = 0;
            deviceController->setHeaterPwmValue(heatReg);
        //}

        //Управление подачей фреона
       // if(overallTimer.isActive())
       // {
//          float refReg = refRegulator();
//          if ((refReg > 0) & (refMachineStatus == MachineStatuses::wait_for_command))
//          {
//              if (refReg > 20)
//              {
//                  refMachineWorkTime = 4000;
//                  refMachineDelayTime = 6000;
//              }
//              else
//              {
//                  refMachineWorkTime = 2000;
//                  refMachineDelayTime = 8000;
//              }
//              refMachineChangeStatus();
            //}
            int refReg = -regOut;
            if(refReg>100) refReg = 100;
            if(refReg<0)   refReg = 0;
            refReg = refReg/10;
            refReg = refReg*1000;
            refMachineWorkTime = refReg;
            refMachineDelayTime = 10000 - refReg;
            if((refMachineStatus == MachineStatuses::wait_for_command) && (refMachineWorkTime!=0)) refMachineChangeStatus();
            QTextStream err(stderr);
            err << "\nRefReg = " + QString::number(refReg);


        }

        //Управление воздушным компрессором


        //Управление насосом соляного раствора
        if(overallTimer.isActive())
        {
          bool brine_low = (deviceController->getParameter(DeviceController::brine_low)).toBool();
          bool brine_high = (deviceController->getParameter(DeviceController::brine_high)).toBool();
          bool brine_compressor_status = (deviceController->getParameter(DeviceController::brine_compressor_status)).toBool();
          if(!brine_low & !brine_compressor_status)
              deviceController->commandsExec(DeviceController::brine_comopressor_turn_on);
          if(brine_compressor_status & brine_high)
              deviceController->commandsExec(DeviceController::brine_compressor_turn_off);
        }
    }
}

void LogicController::refMachineChangeStatus()
{
    switch (refMachineStatus) {
    case MachineStatuses::wait_for_command:
        if(refMachineWorkTime>0)
            //refMachineTimer.singleShot(refMachineWorkTime, this, &LogicController::refMachineChangeStatus);
            refMachineTimer.start(refMachineWorkTime);
        refMachineStatus = MachineStatuses::work;
        deviceController->commandsExec(DeviceController::r134_supply_on);
        break;
    case MachineStatuses::work:
        if(refMachineDelayTime>0)
        {
            //refMachineTimer.singleShot(refMachineDelayTime, this, &LogicController::refMachineChangeStatus);
            refMachineTimer.start(refMachineDelayTime);
            refMachineStatus = MachineStatuses::delay;
            deviceController->commandsExec(DeviceController::r134_supply_off);
        }
        else
            refMachineStatus = MachineStatuses::wait_for_command;
        break;
    case MachineStatuses::delay:
        refMachineStatus = MachineStatuses::wait_for_command;
        break;
    default:
        break;
    }

}

void LogicController::airCompressorChangeStatus()
{
    switch (airCompressorStatus) {
    case MachineStatuses::wait_for_command:
        //airCompressorTimer.singleShot(airCompressorWorkTime, this, &LogicController::airCompressorChangeStatus);
        airCompressorTimer.start(airCompressorWorkTime);
        airCompressorStatus = MachineStatuses::work;
        deviceController->commandsExec(DeviceController::air_compressor_turn_on);
        break;
    case MachineStatuses::delay:
        //airCompressorTimer.singleShot(airCompressorWorkTime, this, &LogicController::airCompressorChangeStatus);
        airCompressorTimer.start(airCompressorWorkTime);
        airCompressorStatus = MachineStatuses::work;
        deviceController->commandsExec(DeviceController::air_compressor_turn_on);
        break;
    case MachineStatuses::work:
        //airCompressorTimer.singleShot(airCompressorDelayTime, this, &LogicController::airCompressorChangeStatus);
        airCompressorTimer.start(airCompressorDelayTime);
        airCompressorStatus = MachineStatuses::delay;
        deviceController->commandsExec(DeviceController::air_compressor_turn_off);
        break;
    default:
        break;
    }
}

void LogicController::setAirCompressorWorkTime(int minutes)
{
    airCompressorWorkTime = minutes*60000;
}

void LogicController::setAirCompressorDelayTime(int minutes)
{
    airCompressorDelayTime = minutes*60000;
}

void LogicController::setTestingTime(int msec)
{
    overallTimer.setInterval(msec);
}

void LogicController::startTesting()
{
    connect(&overallTimer, &QTimer::timeout, this, &LogicController::stopTesting);
    overallTimer.setSingleShot(true);
    overallTimer.start();
    //overallTimer.start(604800000);
    //overallTimer.singleShot(604800000, this, &LogicController::stopTesting);
    controlTimer.start(period);
    airCompressorChangeStatus();
    TAchivement = false;
    resetIntegral = true;
    deviceController->commandsExec(DeviceController::system_turn_on);

}

void LogicController::stopTesting()
{
    deviceController->commandsExec(DeviceController::system_turn_off);
    QTextStream err(stderr);
    if(!overallTimer.isActive()) //нормальный конец испытания
    {
        controlTimer.stop();

        refMachineTimer.stop();
        deviceController->commandsExec(DeviceController::r134_supply_off);
        refMachineStatus = MachineStatuses::wait_for_command;

        airCompressorTimer.stop();
        deviceController->commandsExec(DeviceController::air_compressor_turn_off);
        airCompressorStatus = MachineStatuses::wait_for_command;

        deviceController->commandsExec(DeviceController::brine_compressor_turn_off);

        deviceController->setHeaterPwmValue(0);

        err << "\n Testing stoped normally";

        emit testingStopped("timeout");
    }
    else
    {
        //отстановка по неведомым причинам во время испытания

        overallTimer.stop();

        controlTimer.stop();

        refMachineTimer.stop();
        deviceController->commandsExec(DeviceController::r134_supply_off);
        refMachineStatus = MachineStatuses::wait_for_command;

        airCompressorTimer.stop();
        deviceController->commandsExec(DeviceController::air_compressor_turn_off);
        airCompressorStatus = MachineStatuses::wait_for_command;

        deviceController->commandsExec(DeviceController::brine_compressor_turn_off);

        deviceController->setHeaterPwmValue(0);

        err << "\nTesting crashed ";

        emit testingStopped("crash");
    }
}

bool LogicController::getAirCompressorStatus()
{
    return (deviceController->getParameter(DeviceController::air_compressor_status)).toBool();
}

bool LogicController::getBrineCompressorStatus()
{
    return (deviceController->getParameter(DeviceController::brine_compressor_status)).toBool();
}

bool LogicController::getRefStatus()
{
    return (deviceController->getParameter(DeviceController::r134_valve_status)).toBool();
}

int LogicController::getHeaterStatus()
{
    return (deviceController->getParameter(DeviceController::heater_pwm_value)).toInt();
}

void LogicController::setTSetpoint(float t)
{
    TSetpoint = t;
}

void LogicController::setManualControl(bool isManual)
{
    manualControl = isManual;
}

void LogicController::setPValue(float value)
{
    P = value;
}

void LogicController::setIValue(float value)
{
    I = value;
}

void LogicController::setDValue(float value)
{
    D = value;
}

void LogicController::setManualHeaterEnabled(bool isEnabled)
{
    if(isEnabled)
        deviceController->setHeaterPwmValue(100);
    else
        deviceController->setHeaterPwmValue(0);
}

void LogicController::setManualRefEnabled(bool isEnabled)
{
    if(isEnabled)
        deviceController->commandsExec(DeviceController::r134_supply_on);
    else
        deviceController->commandsExec(DeviceController::r134_supply_off);
}

void LogicController::setManualAirCompressorEnabled(bool isEnabled)
{
    if(isEnabled)
        deviceController->commandsExec(DeviceController::air_compressor_turn_on);
    else
        deviceController->commandsExec(DeviceController::air_compressor_turn_off);
}

void LogicController::setManualBrineCompressorEnabled(bool isEnabled)
{
    if(isEnabled)
        deviceController->commandsExec(DeviceController::brine_comopressor_turn_on);
    else
        deviceController->commandsExec(DeviceController::brine_compressor_turn_off);
}
