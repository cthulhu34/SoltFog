#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    keyboard->setMessage("Введите длительность работы");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setWorkTime(keyboard->getValue());
    }
}

void MainWindow::on_btnPauseTime_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0, 999);
    keyboard->setMessage("Введите длительность паузы");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setPauseTime(keyboard->getValue());
    }
}

void MainWindow::on_btnStart_clicked()
{
    graphicTimer = new QTimer;
    graphicTimer->setInterval(1000);
    connect(graphicTimer, &QTimer::timeout, this, &MainWindow::addDataToPlot);


    dataMiner = new DataMiner(this);
    connect(dataMiner, &DataMiner::eventOccurred, this, &MainWindow::eventsDispatch);

    graphicTimer->start();

    ui->chartView->startNewPlot();
    ui->chartView->setTimeStep(1000);
}

void MainWindow::on_btnStop_clicked()
{

}

void MainWindow::addDataToPlot()
{
    float t = dataMiner->getParameter(DataMiner::temperatureMain).toFloat();

    ui->chartView->addDataT(t);

    ui->lblTemperature->setText(QString::number(t, 'f', 2) + QChar(176) + " C");

}

void MainWindow::setTemperatureTarget(float t)
{
    ui->btnTemperatureTarget->setText(QString::number(t, 'f', 2) + QChar(176) + " C");
}

void MainWindow::setWorkTime(int minutes)
{
    ui->btnWorkTime->setText(QString::number(minutes));
}

void MainWindow::setPauseTime(int minutes)
{
    ui->btnPauseTime->setText(QString::number(minutes));
}

void MainWindow::eventsDispatch(DataMiner::Events e)
{
    switch (e){
    case DataMiner::criticalCircuitDamaged:
        ui->statusBar->showMessage("Critical circuit damaged");
        break;
    case DataMiner::criticalCircuitReturnToNormal:
        ui->statusBar->showMessage("Critical circuit returned to normal");
        break;
    default:
        break;
    }
}

void MainWindow::errorsDispatch(int address, QString msg)
{
    ui->statusBar->showMessage(QString(QDateTime::currentDateTime().toString("hh:mm dd.MM.YY") + " From " + QString::number(address) + " Msg: "));
}
