#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QtCharts>
#include <QDateTime>
//#include "qtouchchartview.h"

using namespace QtCharts;

namespace Ui {
class plotter;
}

class plotter : public QWidget
{
    Q_OBJECT

public:
    explicit plotter(QWidget *parent = 0);
    ~plotter();

    void setTimeRange(int sec);
    void setValuesRange(int min, int max);
    void setTimeStep(int msec);
    void setAutoYScale(bool autoYScale);
    void setGraphicsVisible(bool t, bool h);

    int getTimeRange();
    int getMinY();
    int getMaxY();
    bool getAutoY();
    int getXMin();
    QDateTime getXZeroValue();
    bool getTVisible();
    bool getHVisible();

protected:
    bool event(QEvent *event);

private:
    Ui::plotter *ui;

    QChartView *view;

    QValueAxis *xAxis;  // Ось X
    QValueAxis *yAxis;  // Ось Y

    QDateTime xZeroValue; //begin point for time axis
    QDateTimeAxis *xTimeAxis; //visible time axis

    QPen channelT;   // Pen for temperatue
    QPen channelH; //Pen for humidity

    QLineSeries *diagramT;  // Temperature diagram
    QLineSeries *diagramH; // Humidity diagram

    //int xMin, xMax; //min & max values for x axis
    //int yMin, yMax;
    bool autoY;
    int yMaxValue, yMinValue;
    int timeWindow; //time window for show in seconds
    int xStep; //x step in msec

    bool showT, showH; //graphics visible

    qreal scaleFactor, currentStepScaleFactor;

    QPointF mousePrevPos, mouseCurPos;

    //void pausePlot();
    //void continuePlot();

    bool gestureEvent(QGestureEvent *event);
    void panTriggered(QPanGesture* );
    void pinchTriggered(QPinchGesture*);
    void swipeTriggered(QSwipeGesture*);

    void mousePanTriggered(QMouseEvent* event);

public slots:
    void initPlot();

    void addDataT(double t);
    void addDataH(double h);

    void showInterval(QDateTime start, QDateTime end);
    void updateOnlineInterval();
    void returnToOnline();

    void toStart();
    void toEnd();
    void screenBack();
    void screenForward();

    void startNewPlot();

signals:
    void xRangeChanged();
    void onlineIntervalEnded();
};

#endif // PLOTTER_H
