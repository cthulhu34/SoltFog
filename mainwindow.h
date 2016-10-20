#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
//#include "mdvvmodel.h"
//#include "mva8model.h"
#include "digitkeyboard.h"
#include "plotter.h"
#include "dataminer.h"

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

    void eventsDispatch(DataMiner::Events e);

    void addDataToPlot();

    void on_btnTemperatureTarget_clicked();

    void on_btnWorkTime_clicked();

    void on_btnPauseTime_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *graphicTimer;
    DataMiner *dataMiner;

    void setTemperatureTarget(float t);
    void setWorkTime(int minutes);
    void setPauseTime(int minutes);

};

#endif // MAINWINDOW_H
