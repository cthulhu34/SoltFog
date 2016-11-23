#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->btnStart->setVisible(false);
    //ui->btnStop->setVisible(false);

    ui->tab_2->setLayout(ui->verticalLayout);
    ui->chartView->setGraphicsVisible(true, false);


    logicController = new LogicController(this);

      connect(&updateUITimer, &QTimer::timeout, this, &MainWindow::updateUIValues);
      updateUITimer.start(1000);

      this->setTemperatureTarget(35.0);
      logicController->setPIDvalues(20, 0.001, 5000);

      on_btnControlMode_clicked(false);

      setTestingTime(2);

      connect(logicController, &LogicController::testingStopped, this, &MainWindow::stopTesting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnTemperatureTarget_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0.00, 100.00);
    keyboard->setMessage("Введите значение температуры");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setTemperatureTarget(keyboard->getValue());
    }
}

void MainWindow::on_btnWorkTime_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0, 999);
    keyboard->setMessage("Введите длительность распыления");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setWorkTime(keyboard->getValue());
    }
}

void MainWindow::on_btnPauseTime_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0, 999);
    keyboard->setMessage("Введите длительность оседания");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setPauseTime(keyboard->getValue());
    }
}

void MainWindow::addDataToPlot()
{
    //float t = dataMiner->getParameter(DataMiner::temperatureMain).toFloat();

    //ui->chartView->addDataT(t);

    //ui->lblTemperature->setText(QString::number(t, 'f', 2) + QChar(176) + " C");

}

void MainWindow::setTemperatureTarget(float t)
{
    ui->btnTemperatureTarget->setText(QString::number(t, 'f', 1) + " " +QChar(176) + "C");
    TSetpoint = t;
    logicController->setTSetpoint(t);
    ui->chartView->setTSetpoint(t);
}

void MainWindow::setWorkTime(int minutes)
{
    ui->btnWorkTime->setText(QString::number(minutes) + " мин.");
    logicController->setAirCompressorWorkTime(minutes);
}

void MainWindow::setPauseTime(int minutes)
{
    ui->btnPauseTime->setText(QString::number(minutes)  + " мин.");
    logicController->setAirCompressorDelayTime(minutes);
}

//void MainWindow::eventsDispatch(DataMiner::Events e)
//{
//    switch (e){
//    case DataMiner::criticalCircuitDamaged:
//        ui->statusBar->showMessage("Critical circuit damaged");
//        break;
//    case DataMiner::criticalCircuitReturnToNormal:
//        ui->statusBar->showMessage("Critical circuit returned to normal");
//        break;
//    default:
//        break;
//    }
//}

void MainWindow::errorsDispatch(int address, QString msg)
{
    ui->statusBar->showMessage(QString(QDateTime::currentDateTime().toString("hh:mm dd.MM.YY") + " From " + QString::number(address) + " Msg: " + msg));
}

void MainWindow::updateUIValues()
{
    ui->lblTemperature->setText(QString::number(logicController->getCurrentTemperatureValue(), 'f', 1) + " °C");

    if(logicController->getAirCompressorStatus())
    {
        ui->lblAirCompressorStatus->setText("ВКЛ");
        ui->lblWork->setText("распыление");
    }
    else
    {
        ui->lblAirCompressorStatus->setText("ВЫКЛ");
        ui->lblWork->setText("пауза");
    }

    if(logicController->getBrineCompressorStatus())
    {
        ui->lblBrineCompressorStatus->setText("ВКЛ");
    }
    else
    {
        ui->lblBrineCompressorStatus->setText("ВЫКЛ");
    }

    if(logicController->getRefStatus())
    {
        ui->lblR134ValveStatus->setText("ВКЛ");
    }
    else
    {
        ui->lblR134ValveStatus->setText("ВЫКЛ");
    }

    int pwm = logicController->getHeaterStatus();
    ui->lblHeaterValue->setText(QString::number(pwm));
    if(pwm > 0)
    {
        ui->lblHeaterStatus->setText("ВКЛ");
    }
    else
    {
        ui->lblHeaterStatus->setText("ВЫКЛ");
    }

    ui->chartView->addDataT(logicController->getCurrentTemperatureValue());

    ui->lblPOutValue->setText(QString::number(logicController->getPOutValue(), 'f', 2));
    ui->lblIOutValue->setText(QString::number(logicController->getIOutValue(), 'f', 2));
    ui->lblDOutValue->setText(QString::number(logicController->getDOutValue(), 'f', 2));


    if(ui->btnStart->isChecked())
    {
       qint64 gone = start.secsTo(QDateTime::currentDateTime())+1;
       int days = gone/86400;
       int hours = (gone - days*86400)/3600;
       int minutes = (gone - days*86400 - hours*3600)/60;
       int secs = gone - days*86400 - hours*3600 - minutes*60;
       ui->lblTimeGone->setText(QString("%1д:%2ч:%3м:%4с").arg(days, 2).arg(hours, 2).arg(minutes, 2).arg(secs, 2));

       qint64 lost = QDateTime::currentDateTime().secsTo(end);
       days = lost/86400;
       hours = (lost - days*86400)/3600;
       minutes = (lost - days*86400 - hours*3600)/60;
       secs = lost - days*86400 - hours*3600 - minutes*60;
       ui->lblTimeLost->setText(QString("%1д:%2ч:%3м:%4с").arg(days, 2).arg(hours, 2).arg(minutes, 2).arg(secs, 2));
    }
}

void MainWindow::on_btnRegulatorP_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0.00, 10000.00);
    keyboard->setMessage("Введите значение П-коэффициента");

    if(keyboard->exec() == QDialog::Accepted)
    {
        logicController->setPValue(keyboard->getValue());
        ui->btnRegulatorP->setText(QString::number(keyboard->getValue(), 'f', 2));
    }
}

void MainWindow::on_btnRegulatorI_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0.00001, 10000.00);
    keyboard->setMessage("Введите значение И-коэффициента");

    if(keyboard->exec() == QDialog::Accepted)
    {
        logicController->setIValue(keyboard->getValue());
        ui->btnRegulatorI->setText(QString::number(keyboard->getValue(), 'f', 2));
    }
}

void MainWindow::on_btnRegulatorD_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0.00, 10000.00);
    keyboard->setMessage("Введите значение Д-коэффициента");

    if(keyboard->exec() == QDialog::Accepted)
    {
        logicController->setDValue(keyboard->getValue());
        ui->btnRegulatorD->setText(QString::number(keyboard->getValue(), 'f', 2));
    }
}

void MainWindow::on_btnControlMode_clicked()
{

}

void MainWindow::on_btnControlMode_clicked(bool checked)
{
//    ui->btnHeaterSwith->setEnabled(checked);
//    ui->btnRefSwitch->setEnabled(checked);
//    ui->btnBrineCompressorSwitch->setEnabled(checked);
//    ui->btnAirCompressorSwitch->setEnabled(checked);

    QString styleRed;
    styleRed = "color: rgb(10, 10, 30);\
            background-color: #C00000;\
            border: 3px solid #990000;\
            border-top-left-radius: 5px;\
            border-top-right-radius: 5px;\
            border-bottom-left-radius: 5px;\
            border-bottom-right-radius: 5px;";

    QString styleGreen;
    styleGreen = "color: rgb(10, 10, 30);\
            background-color: #00C000;\
            border: 3px solid #009900;\
            border-top-left-radius: 5px;\
            border-top-right-radius: 5px;\
            border-bottom-left-radius: 5px;\
            border-bottom-right-radius: 5px;";

    QString styleGray;
    styleGray = "color: rgb(20, 20, 20);\
            background-color: #dddddd;\
            border: 3px solid #d0d0d0;\
            border-top-left-radius: 5px;\
            border-top-right-radius: 5px;\
            border-bottom-left-radius: 5px;\
            border-bottom-right-radius: 5px;";

    if(checked)
    {
        logicController->stopTesting();
        ui->statusBar->showMessage("Активирован ручной режим, испытание остановлено");
        QString styleGreen2 =
                "color: rgb(10, 10, 30);\
                background-color: #00C000;\
                border: 6px solid #009900;\
                border-top-left-radius: 10px;\
                border-top-right-radius: 10px;\
                border-bottom-left-radius: 10px;\
                border-bottom-right-radius: 10px;";
        ui->btnStart->setStyleSheet(styleGreen2);
        ui->btnStart->setText("Пуск");
        ui->btnStart->setChecked(false);

        ui->btnHeaterSwith->setStyleSheet(styleRed);
        ui->btnRefSwitch->setStyleSheet(styleRed);
        ui->btnAirCompressorSwitch->setStyleSheet(styleRed);
        ui->btnBrineCompressorSwitch->setStyleSheet(styleRed);
        ui->btnControlMode->setStyleSheet(styleGreen);
        ui->btnControlMode->setText("Включено");
    }
    else
    {
        ui->statusBar->showMessage("Ручной режим деактивирован");

        on_btnHeaterSwith_clicked(false);
        on_btnRefSwitch_clicked(false);
        on_btnAirCompressorSwitch_clicked(false);
        on_btnBrineCompressorSwitch_clicked(false);

        ui->btnHeaterSwith->setStyleSheet(styleGray);
        ui->btnRefSwitch->setStyleSheet(styleGray);
        ui->btnAirCompressorSwitch->setStyleSheet(styleGray);
        ui->btnBrineCompressorSwitch->setStyleSheet(styleGray);
        ui->btnControlMode->setStyleSheet(styleRed);
        ui->btnControlMode->setText("Выключено");
    }
}

void MainWindow::on_btnHeaterSwith_clicked(bool checked)
{
    logicController->setManualHeaterEnabled(checked);

    if(checked)
    {
        QString styleGreen;
        styleGreen = "color: rgb(10, 10, 30);\
                background-color: #00C000;\
                border: 3px solid #009900;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnHeaterSwith->setStyleSheet(styleGreen);
        ui->btnHeaterSwith->setText("Включен");
    }
    else
    {
        QString styleRed;
        styleRed = "color: rgb(10, 10, 30);\
                background-color: #C00000;\
                border: 3px solid #990000;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnHeaterSwith->setStyleSheet(styleRed);
        ui->btnHeaterSwith->setText("Выключен");
    }
}

void MainWindow::on_btnRefSwitch_clicked(bool checked)
{
    logicController->setManualRefEnabled(checked);

    if(checked)
    {
        QString styleGreen;
        styleGreen = "color: rgb(10, 10, 30);\
                background-color: #00C000;\
                border: 3px solid #009900;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnRefSwitch->setStyleSheet(styleGreen);
        ui->btnRefSwitch->setText("Включен");
    }
    else
    {
        QString styleRed;
        styleRed = "color: rgb(10, 10, 30);\
                background-color: #C00000;\
                border: 3px solid #990000;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnRefSwitch->setStyleSheet(styleRed);
        ui->btnRefSwitch->setText("Выключен");
    }
}



void MainWindow::on_btnAirCompressorSwitch_clicked(bool checked)
{
    logicController->setManualAirCompressorEnabled(checked);

    if(checked)
    {
        QString styleGreen;
        styleGreen = "color: rgb(10, 10, 30);\
                background-color: #00C000;\
                border: 3px solid #009900;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnAirCompressorSwitch->setStyleSheet(styleGreen);
        ui->btnAirCompressorSwitch->setText("Включен");
    }
    else
    {
        QString styleRed;
        styleRed = "color: rgb(10, 10, 30);\
                background-color: #C00000;\
                border: 3px solid #990000;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnAirCompressorSwitch->setStyleSheet(styleRed);
        ui->btnAirCompressorSwitch->setText("Выключен");
    }
}

void MainWindow::on_btnBrineCompressorSwitch_clicked(bool checked)
{
    logicController->setManualBrineCompressorEnabled(checked);

    if(checked)
    {
        QString styleGreen;
        styleGreen = "color: rgb(10, 10, 30);\
                background-color: #00C000;\
                border: 3px solid #009900;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnBrineCompressorSwitch->setStyleSheet(styleGreen);
        ui->btnBrineCompressorSwitch->setText("Включен");
    }
    else
    {
        QString styleRed;
        styleRed = "color: rgb(10, 10, 30);\
                background-color: #C00000;\
                border: 3px solid #990000;\
                border-top-left-radius: 5px;\
                border-top-right-radius: 5px;\
                border-bottom-left-radius: 5px;\
                border-bottom-right-radius: 5px;";
        ui->btnBrineCompressorSwitch->setStyleSheet(styleRed);
        ui->btnBrineCompressorSwitch->setText("Выключен");
    }
}

void MainWindow::on_btnDays_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0, 999);
    keyboard->setMessage("Введите длительность испытания, сутки.");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setTestingTime(keyboard->getValue());
    }
}

void MainWindow::setTestingTime(int days)
{
    logicController->setTestingTime(days*86400000);
    ui->btnDays->setText(QString::number(days) + " сут.");
    secsToEnd = days*86400;

//    logicController->setTestingTime(10000);
//    ui->btnDays->setText(QString::number(days) + " сут.");
//    secsToEnd = 10;
}

void MainWindow::on_btnPlotOptions_clicked()
{
    PlotOptions *options = new PlotOptions(this);

    options->setAutoY(ui->chartView->getAutoY());
    options->setMinY(ui->chartView->getMinY());
    options->setMaxY(ui->chartView->getMaxY());
    options->setGraphicsVisible(ui->chartView->getTVisible(), ui->chartView->getHVisible());
    options->setTimeRange(ui->chartView->getTimeRange());

    if(options->exec() == QDialog::Accepted)
    {
        ui->chartView->setAutoYScale(options->autoY);
        //if(!options->autoY)
            ui->chartView->setValuesRange(options->minY, options->maxY);
        ui->chartView->setTimeRange(options->timeRange);
        //ui->chartView->setGraphicsVisible(options->showT, options->showH);
    }
}

void MainWindow::on_btnCurrent_clicked()
{
    ui->chartView->returnToOnline();
}

void MainWindow::on_btnToStart_clicked()
{
    ui->chartView->toStart();
}

void MainWindow::on_btnBack_clicked()
{
    ui->chartView->screenBack();
}

void MainWindow::on_btnForward_clicked()
{
    ui->chartView->screenForward();
}

void MainWindow::on_btnToEnd_clicked()
{
    ui->chartView->toEnd();
}

void MainWindow::on_btnStart_clicked(bool checked)
{
    if (!ui->btnControlMode->isChecked())
    {
        if(checked)
        {

            logicController->startTesting();
            start = QDateTime::currentDateTime();
            end = QDateTime::currentDateTime().addSecs(secsToEnd);
            ui->statusBar->showMessage("Начало испытания " + start.toString("dd.MM.yyyy  hh:mm"));
            QString styleRed =
                    "color: rgb(10, 10, 30);\
                    background-color: #C00000;\
                    border: 6px solid #990000;\
                    border-top-left-radius: 10px;\
                    border-top-right-radius: 10px;\
                    border-bottom-left-radius: 10px;\
                    border-bottom-right-radius: 10px;";
            ui->btnStart->setStyleSheet(styleRed);
            ui->btnStart->setText("Стоп");


        }
        else
        {
            logicController->stopTesting();
            ui->statusBar->showMessage("Испытание прервано оператором");

            QString styleGreen =
                    "color: rgb(10, 10, 30);\
                    background-color: #00C000;\
                    border: 6px solid #009900;\
                    border-top-left-radius: 10px;\
                    border-top-right-radius: 10px;\
                    border-bottom-left-radius: 10px;\
                    border-bottom-right-radius: 10px;";
            ui->btnStart->setStyleSheet(styleGreen);
            ui->btnStart->setText("Пуск");
         }
    }
    else
    {
        ui->statusBar->showMessage("Активирован ручной режим, проведение испытания невозможно.");
        ui->btnStart->setChecked(false);
    }
}

void MainWindow::stopTesting(QString reason)
{
    if(reason == "timeout")
    {
        QString styleGreen =
                "color: rgb(10, 10, 30);\
                background-color: #00C000;\
                border: 6px solid #009900;\
                border-top-left-radius: 10px;\
                border-top-right-radius: 10px;\
                border-bottom-left-radius: 10px;\
                border-bottom-right-radius: 10px;";
        ui->btnStart->setStyleSheet(styleGreen);
        ui->btnStart->setText("Пуск");
        ui->btnStart->setChecked(false);
        ui->statusBar->showMessage("Испытание упешно завершено");
    }

}
