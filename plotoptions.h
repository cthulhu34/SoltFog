#ifndef PLOTOPTIONS_H
#define PLOTOPTIONS_H

#include <QDialog>


namespace Ui {
class PlotOptions;
}

class PlotOptions : public QDialog
{
    Q_OBJECT

public:
    explicit PlotOptions(QWidget *parent = 0);
    ~PlotOptions();

    int minY, maxY;
    bool autoY;
    int timeH, timeM, timeRange;
    bool showT, showH;

    void setMinY(int min);
    void setMaxY(int max);
    void setAutoY(bool autoy);
    void setTimeRange(int range);
    void setGraphicsVisible(bool t, bool h);

private slots:
    void on_pbOk_clicked();

    void on_pbCancel_clicked();

    void on_cbAutoY_stateChanged(int arg1);

    void on_pbMinYMinus_clicked();

    void on_pbMinYPlus_clicked();

    void on_pbMinYValue_clicked();

    void on_pbMaxYMinus_clicked();

    void on_pbMaxYPlus_clicked();

    void on_pbMaxYValue_clicked();

    void on_pbHourMins_clicked();

    void on_pbHourPlus_clicked();

    void on_pbHourValue_clicked();

    void on_pbMinuteMinus_clicked();

    void on_pbMinutePlus_clicked();

    void on_pbMinuteValue_clicked();


private:
    Ui::PlotOptions *ui;


};

#endif // PLOTOPTIONS_H
