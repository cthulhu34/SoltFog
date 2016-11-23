#include "plotoptions.h"
#include "ui_plotoptions.h"
#include "digitkeyboard.h"

PlotOptions::PlotOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotOptions)
{
    ui->setupUi(this);
}

PlotOptions::~PlotOptions()
{
    delete ui;
}

void PlotOptions::on_pbOk_clicked()
{
    //maxY = ui->sbYMax->value();
    //minY = ui->sbYMin->value();
    //timeH = ui->sbXHour->value();
    //timeM = ui->sbXMinute->value();
    //showT = ui->cbTemperature->checkState();
    //showH = ui->cbHumi->checkState();
    autoY = ui->cbAutoY->checkState();

    if(minY>=maxY)
    {
        ui->lblMessage->setText("Недопустимый диапазон значений");
        return;
    }

    //if((timeH+timeM)==0)
    if(timeRange == 0)
    {
        ui->lblMessage->setText("Недопустимый диапазон времни");
        return;
    }

    //timeRange = (timeH*60 + timeM)*60;
    QDialog::accept();

}

void PlotOptions::on_pbCancel_clicked()
{
    QDialog::reject();
}

void PlotOptions::on_cbAutoY_stateChanged(int arg1)
{
    if(arg1 == Qt::CheckState::Checked)
    {
        ui->pbMaxYValue->setEnabled(false);
        ui->pbMaxYMinus->setEnabled(false);
        ui->pbMaxYPlus->setEnabled(false);

        ui->pbMinYValue->setEnabled(false);
        ui->pbMinYMinus->setEnabled(false);
        ui->pbMinYPlus->setEnabled(false);

        autoY = true;
    }

    if(arg1 == Qt::CheckState::Unchecked)
    {
        ui->pbMaxYValue->setEnabled(true);
        ui->pbMaxYMinus->setEnabled(true);
        ui->pbMaxYPlus->setEnabled(true);

        ui->pbMinYValue->setEnabled(true);
        ui->pbMinYMinus->setEnabled(true);
        ui->pbMinYPlus->setEnabled(true);

        autoY = false;
    }
}

void PlotOptions::setAutoY(bool autoy)
{
    this->autoY = autoy;
    ui->cbAutoY->setChecked(autoy);
}

void PlotOptions::setTimeRange(int range)
{
    int hours, minutes;
    hours = range/3600;
    minutes = (range - hours*3600)/60;
    //ui->sbXHour->setValue(hours);
    //ui->sbXMinute->setValue(minutes);
    ui->pbHourValue->setText(QString::number(hours));
    ui->pbMinuteValue->setText(QString::number(minutes));
    timeRange = range;
}

void PlotOptions::setMinY(int min)
{
    minY = min;
    ui->pbMinYValue->setText(QString::number(min));
}

void PlotOptions::setMaxY(int max)
{
    maxY = max;
    ui->pbMaxYValue->setText(QString::number(max));
}

void PlotOptions::setGraphicsVisible(bool t, bool h)
{
    //ui->cbTemperature->setChecked(t);
   // ui->cbHumi->setChecked(h);
    showT = t;
    showH = h;
}


void PlotOptions::on_pbMinYMinus_clicked()
{
    minY = minY-1;
    ui->pbMinYValue->setText(QString::number(minY));
}

void PlotOptions::on_pbMinYPlus_clicked()
{
    minY = minY + 1;
    ui->pbMinYValue->setText(QString::number(minY));
}

void PlotOptions::on_pbMinYValue_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(-70, 130);
    keyboard->setMessage("Введите значение");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setMinY(keyboard->getValue());
    }
}

void PlotOptions::on_pbMaxYMinus_clicked()
{
    maxY = maxY-1;
    ui->pbMaxYValue->setText(QString::number(maxY));
}

void PlotOptions::on_pbMaxYPlus_clicked()
{
    maxY = maxY+1;
    ui->pbMaxYValue->setText(QString::number(maxY));
}

void PlotOptions::on_pbMaxYValue_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(-70, 130);
    keyboard->setMessage("Введите значение");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setMaxY(keyboard->getValue());
    }
}

void PlotOptions::on_pbHourMins_clicked()
{
    timeRange = timeRange - 3600;
    setTimeRange(timeRange);
}

void PlotOptions::on_pbHourPlus_clicked()
{
    timeRange = timeRange + 3600;
    setTimeRange(timeRange);
}

void PlotOptions::on_pbHourValue_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0, 24);
    keyboard->setMessage("Введите значение");

    if(keyboard->exec() == QDialog::Accepted)
    {
        int hours = keyboard->getValue();
        timeRange = timeRange - (timeRange/3600)*3600 + hours*3600;
        setTimeRange(timeRange);
    }
}

void PlotOptions::on_pbMinuteMinus_clicked()
{
    timeRange = timeRange - 600;
    setTimeRange(timeRange);
}

void PlotOptions::on_pbMinutePlus_clicked()
{
    timeRange = timeRange + 600;
    setTimeRange(timeRange);
}

void PlotOptions::on_pbMinuteValue_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0, 60);
    keyboard->setMessage("Введите значение");

    if(keyboard->exec() == QDialog::Accepted)
    {
        int minutes = keyboard->getValue();
        timeRange = (timeRange/3600)*3600 + minutes*60;
        setTimeRange(timeRange);
    }
}

