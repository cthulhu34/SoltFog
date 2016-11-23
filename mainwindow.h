#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
//#include "mdvvmodel.h"
//#include "mva8model.h"
#include "digitkeyboard.h"
#include "plotter.h"
//#include "dataminer.h"
#include "logiccontroller.h"
#include "devicecontroller.h"
#include "plotoptions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void errorsDispatch(int address, QString msg);

    //void eventsDispatch(DataMiner::Events e);

    void addDataToPlot();

    void on_btnTemperatureTarget_clicked();

    void on_btnWorkTime_clicked();

    void on_btnPauseTime_clicked();

    void updateUIValues();

    void on_btnRegulatorP_clicked();

    void on_btnRegulatorI_clicked();

    void on_btnRegulatorD_clicked();

    void on_btnControlMode_clicked();

    void on_btnControlMode_clicked(bool checked);

    void on_btnHeaterSwith_clicked(bool checked);

    void on_btnRefSwitch_clicked(bool checked);

    void on_btnAirCompressorSwitch_clicked(bool checked);

    void on_btnBrineCompressorSwitch_clicked(bool checked);

    void on_btnDays_clicked();

    void on_btnPlotOptions_clicked();

    void on_btnCurrent_clicked();

    void on_btnToStart_clicked();

    void on_btnBack_clicked();

    void on_btnForward_clicked();

    void on_btnToEnd_clicked();

    void on_btnStart_clicked(bool checked);

    void stopTesting(QString reason);

private:
    Ui::MainWindow *ui;

    LogicController  *logicController;

    QDateTime start, end;
    qint64 secsToEnd;

    QTimer *graphicTimer;
   // DataMiner *dataMiner;
    QTimer updateUITimer;

    float TSetpoint;

    void setTemperatureTarget(float t);
    void setWorkTime(int minutes);
    void setPauseTime(int minutes);
    void setTestingTime(int days);


};

#endif // MAINWINDOW_H
