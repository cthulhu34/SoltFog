#include "plotter.h"
#include "ui_plotter.h"

#include <QTextStream>

plotter::plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotter)
{
    ui->setupUi(this);

    view = new QChartView();
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->horizontalLayout->addWidget(view);
    view->chart()->setTheme(QChart::ChartThemeQt);    // Установка темы QChartView
    //view->setRenderHint(QPainter::Antialiasing);
    view->setEnabled(false);

    showT = true;
    showH = true;
    xStep = 1000;
    timeWindow = 600;
    autoY = true;

    view->chart()->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    initPlot();

   // view->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
    //view->chart()->setAcceptTouchEvents(true);
    this->setAttribute(Qt::WA_AcceptTouchEvents, true);
    //view->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);

   // view->grabGesture(Qt::PinchGesture);
   // view->grabGesture(Qt::SwipeGesture);
   // view->grabGesture(Qt::PanGesture);

view->setAttribute(Qt::WA_AcceptTouchEvents, false);
//view->setAttribute(Qt::WA);

    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PanGesture);

   // QScroller::grabGesture(view->viewport(), QScroller::LeftMouseButtonGesture);

    scaleFactor = 1;

    //view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//    scrollArea = new QScrollArea;
//    scrollArea->setWidget(this);
   // QScroller::grabGesture(view->viewport(), QScroller::TouchGesture);

}

plotter::~plotter()
{
    delete ui;
}

void plotter::initPlot()
{



    int min = 0;
    int max = min + timeWindow;
    xZeroValue = QDateTime::currentDateTime();

    xTimeAxis = new QDateTimeAxis;
    xTimeAxis->setFormat("hh:mm:ss");
    xTimeAxis->setRange(xZeroValue, xZeroValue.addSecs(timeWindow));

    xAxis = new QValueAxis;
    xAxis->setRange(min, max);
    xAxis->setVisible(false);

    yAxis = new QValueAxis;

    yAxis->setTickCount(11);
    yAxis->setRange(0, 30);
    yMaxValue=29;
    yMinValue=1;



        axisY3 = new QCategoryAxis;
        axisY3->setLinePenColor(QColor(0, 0, 200));
        axisY3->setGridLinePen(QPen(Qt::DashLine));
        view->chart()->addAxis(axisY3, Qt::AlignRight);
        //diagramT->attachAxis(axisY3);

    channelT.setWidthF(3);
    channelT.setBrush(Qt::red);

    channelH.setWidthF(3);
    channelH.setBrush(Qt::blue);
    channelH.setJoinStyle(Qt::RoundJoin);

    diagramT = new QLineSeries;
    diagramT->setName(tr("Температура"));
    diagramT->setPen(channelT);
    diagramT->setVisible(showT);
//    diagramT->setUseOpenGL(true);               // Включить поддержку OpenGL
//

    diagramH = new QLineSeries;
    diagramH->setName(tr("Влажность"));
    diagramH->setPen(channelH);
    diagramH->setVisible(showH);
//    diagramH->setUseOpenGL(true);               // Включить поддержку OpenGL


    view->chart()->addSeries(diagramT);         // Добавить на график
    view->chart()->setAxisX(xAxis, diagramT);   // Назначить ось xAxis, осью X для diagramA
    view->chart()->setAxisY(yAxis, diagramT);
    diagramT->attachAxis(axisY3);

    view->chart()->addSeries(diagramH);         // Добавить на график
    view->chart()->setAxisX(xAxis, diagramH);   // Назначить ось xAxis, осью X для diagramA
    view->chart()->addAxis(xTimeAxis, Qt::AlignBottom);
    view->chart()->setAxisY(yAxis, diagramH);   // Назначить ось yAxis, осью Y для diagramA


    view->setRenderHint(QPainter::Antialiasing);

    connect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
}

void plotter::addDataT(double t)
{
    if(autoY)
    {
        bool needRescale=false;
        if(t < yMinValue) { yMinValue=t; needRescale=true; }
        if(t > yMaxValue) { yMaxValue=t; needRescale=true; }
        if(needRescale)
        {
            yAxis->setRange(yMinValue-1, yMaxValue+1);
        }
    }

   int lastIndex = diagramT->count()-1;
   int currentXValue=0;

   if(lastIndex != -1)
   {
       currentXValue = diagramT->at(lastIndex).x() + xStep/1000;
   }

   diagramT->append(currentXValue, t);
   diagramT->setName(tr("Температура: ") + QString::number(t, 'f', 2));

   if(currentXValue > xAxis->max())
   {
//       xMin+= timeWindow/4;
//       xMax= xMin + timeWindow;

//       xAxis->setRange(xMin, xMin + timeWindow);
//       xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMin + timeWindow));
       emit onlineIntervalEnded();
   }

}

void plotter::addDataH(double h)
{
    if(autoY)
    {
        bool needRescale=false;
        if(h < yMinValue) { yMinValue=h; needRescale=true; }
        if(h > yMaxValue) { yMaxValue=h; needRescale=true; }
        if(needRescale)
        {
            yAxis->setRange(yMinValue-1, yMaxValue+1);
            //yMin = yMinValue-1;
            //yMax = yMaxValue+1;
        }
    }

   int lastIndex = diagramH->count()-1;
   int currentXValue=0;

   if(lastIndex != -1)
   {
       currentXValue = diagramH->at(lastIndex).x() + xStep/1000;
   }

   diagramH->append(currentXValue, h);
   diagramH->setName(tr("Влажность: ") + QString::number(h, 'f', 2));

   if(currentXValue > xAxis->max())
   {
//       xMin+= timeWindow/4;
//       xMax= xMin + timeWindow;

//       xAxis->setRange(xMin, xMin + timeWindow);
//       xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMin + timeWindow));
       emit onlineIntervalEnded();
   }

}

void plotter::setTimeRange(int sec)
{
    timeWindow = sec;
    //xMax = xMin + timeWindow;
    xAxis->setRange(xAxis->min(), xAxis->min() + sec);
    xTimeAxis->setRange(xZeroValue.addSecs(xAxis->min()), xZeroValue.addSecs(xAxis->max()));
}

void plotter::setTimeStep(int msec)
{
    xStep = msec;
}

void plotter::setValuesRange(int min, int max)
{
    if(autoY) return;

    //yMin = min;
    //yMax = max;
    yAxis->setRange(min, max);
}

void plotter::setAutoYScale(bool autoYScale)
{
    autoY = autoYScale;
}


bool plotter::getAutoY()
{
    return autoY;
}

int plotter::getMinY()
{
    return yAxis->min();
}

int plotter::getMaxY()
{
    return yAxis->max();
}

int plotter::getTimeRange()
{
    return timeWindow;
}

int plotter::getXMin()
{
    return xAxis->min();
}

QDateTime plotter::getXZeroValue()
{
    return xZeroValue;
}

void plotter::showInterval(QDateTime start, QDateTime end)
{

    int min, max;
    min = xZeroValue.secsTo(start);
    max = xZeroValue.secsTo(end);

    xAxis->setRange(min, max);
    xTimeAxis->setRange(start, end);

    disconnect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
}

void plotter::updateOnlineInterval()
{

     int min = xAxis->min() + timeWindow/4;
     int max = min + timeWindow;

     xAxis->setRange(min, max);
     xTimeAxis->setRange(xZeroValue.addSecs(min), xZeroValue.addSecs(max));

}

void plotter::returnToOnline()
{
    int min = 0;
    int max = timeWindow;
    int currentX = diagramT->at(diagramT->count()-1).x();
    if(currentX > max)
    {
        max = currentX + timeWindow/4;
        min = max - timeWindow;
    }

    xAxis->setRange(min, max);
    xTimeAxis->setRange(xZeroValue.addSecs(min), xZeroValue.addSecs(max));
    connect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
}

void plotter::toStart()
{
    showInterval(xZeroValue, xZeroValue.addSecs(timeWindow));
}

void plotter::toEnd()
{
    int min = 0;
    int max = timeWindow;
    int currentX = diagramT->at(diagramT->count()-1).x();
    if(currentX > max)
    {
        max = currentX + timeWindow/4;
        min = max - timeWindow;
    }

    xAxis->setRange(min, max);
    xTimeAxis->setRange(xZeroValue.addSecs(min), xZeroValue.addSecs(max));
    connect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
}

void plotter::screenBack()
{
    //QDateTimeAxis *timeAxis = (QDateTimeAxis*)view->chart()->axisX(diagramT);
    //showInterval(xTimeAxis->min().addSecs((-timeWindow), xTimeAxis->min());

    //view->chart()->scroll(-timeWindow, 0);

    //xTimeAxis->setRange(xZeroValue.addSecs(xAxis->min()), xZeroValue.addSecs(xAxis->max()));
    if((xAxis->min() - timeWindow) > 0)
        showInterval(xTimeAxis->min().addSecs(-timeWindow), xTimeAxis->min());
    else
        showInterval(xZeroValue, xZeroValue.addSecs(timeWindow));

}

void plotter::screenForward()
{
    //QTextStream err(stderr);
    //QDateTimeAxis *timeAxis = (QDateTimeAxis*)view->chart()->axisX(diagramT);

    //view->chart()->scroll(view->chart()->, 0);
    //err << "\nxAxisMin = " << (int)xAxis->min() << "  " << view->chart()->size().width();
    //xTimeAxis->setRange(xZeroValue.addSecs(xAxis->min()), xZeroValue.addSecs(xAxis->max()));

    int currentX = diagramT->at(diagramT->count()-1).x();
    if((xAxis->max() + timeWindow) < currentX)
    {
        showInterval(xTimeAxis->max(), xTimeAxis->max().addSecs(timeWindow));
    }
    else
    {

        int max = currentX + timeWindow/4;
        int min = max - timeWindow;
        showInterval(xZeroValue.addSecs(min), xZeroValue.addSecs(max));
    }
}

void plotter::setGraphicsVisible(bool t, bool h)
{
    showT = t;
    showH = h;
    diagramT->setVisible(t);
    diagramH->setVisible(h);
}

bool plotter::getTVisible()
{
    return showT;
}

bool plotter::getHVisible()
{
    return showH;
}

void plotter::startNewPlot()
{
    xZeroValue = QDateTime::currentDateTime();
    diagramT->clear();
    diagramH->clear();

    int min = 0;
    int max = min + timeWindow;

    xAxis->setRange(min, max);
    xTimeAxis->setRange(xZeroValue.addSecs(min), xZeroValue.addSecs(max));
    //if(!autoY)
        //yAxis->setRange(yMin, yMax);
    //else
    //{
        yAxis->setRange(0, 30);
        yMaxValue=29;
        yMinValue=1;
   // }
}

bool plotter::event(QEvent *event)
{
   // QTextStream err(stderr);
    //err << tr("Connect failed: ") + event->type().;

    if (event->type() == QEvent::Gesture)
    {
       // err << tr("\nQEvent type = Gesture");
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    if(event->type() == QEvent::MouseButtonPress)
    {
        mousePrevPos = (static_cast<QMouseEvent *>(event))->localPos();
        disconnect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
       // err << "\nMouse button press";
        //m_mousePressed = true;
    }
    if(event->type() == QEvent::MouseMove)
    {
        //err << "\nMouse button move";
        //if(m_mousePressed)
            mousePanTriggered(static_cast<QMouseEvent *>(event));
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
       // err << "\nMouse button release";
        //m_mousePressed = false;
    }
    return QWidget::event(event);
}

bool plotter::gestureEvent(QGestureEvent *event)
{
    //QTextStream err(stderr);
    if(QGesture *swipe = event->gesture(Qt::SwipeGesture))
    {
       // err << tr("\nGesture type = swipe");
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    }
    if(QGesture *pan = event->gesture(Qt::PanGesture))
    {
       // err << tr("\nGesture type = pan");
        panTriggered(static_cast<QPanGesture *>(pan));
    }
    if(QGesture *pinch = event->gesture(Qt::PinchGesture))
    {
        // err << tr("\nGesture type = pinch");
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    }
    return true;
}

void plotter::swipeTriggered(QSwipeGesture *gesture)
{
    if(gesture->state() == Qt::GestureFinished)
    {
        if(gesture->horizontalDirection() == QSwipeGesture::Left)
            this->screenForward();
        if(gesture->horizontalDirection() == QSwipeGesture::Right)
            this->screenBack();
        update();
    }
}

void plotter::panTriggered(QPanGesture *gesture)
{
    //QTextStream err(stderr);

    QPointF delta = gesture->delta();

   // err << "\ndelta.x = " << delta.x() << "  delta.y = " << delta.y();
}

void plotter::pinchTriggered(QPinchGesture *gesture)
{
   // QTextStream err(stderr);

    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();

   // err << "\n Enter to pinch\n";
       if (changeFlags & QPinchGesture::ScaleFactorChanged) {
           if(gesture->scaleFactor() >1)
           {
               if(timeWindow > 600)
                  setTimeRange(timeWindow*0.9);
           }
           else
           {
               if(timeWindow < 10800)
                  setTimeRange(timeWindow*1.1);
           }
           currentStepScaleFactor = gesture->totalScaleFactor();
           //err << "pinchTriggered(): zoom by " <<
  //             gesture->scaleFactor() << "->" << currentStepScaleFactor;
            //setTimeRange(timeWindow/currentStepScaleFactor);
       }
       if (gesture->state() == Qt::GestureFinished) {
           //scaleFactor *= currentStepScaleFactor;

           currentStepScaleFactor = 1;
           //err << "\nscale finished " << scaleFactor;
           //setTimeRange(timeWindow*scaleFactor);
       }
       update();
       //view->chart()->scroll();
}

void plotter::mousePanTriggered(QMouseEvent *event)
{
    mouseCurPos = event->localPos();
    float dx = mousePrevPos.x() - mouseCurPos.x();
    float dy = mouseCurPos.y() - mousePrevPos.y();
    //QTextStream err(stderr);
   // err << "/ndx = " << dx << " dy = " << dy;
    //int currentX = diagramT->at(diagramT->count()-1).x();
    //int interval = xAxis->max() - xAxis->min();

        view->chart()->scroll(dx, dy);
        xTimeAxis->setRange(xZeroValue.addSecs(xAxis->min()), xZeroValue.addSecs(xAxis->max()));

       // if(xAxis->max() > (currentX + interval/4))
       //     showInterval(xZeroValue.addSecs(currentX + interval/4 - interval), xZeroValue.addSecs(currentX + interval/4));
       // if(xAxis->min() < 0)
       //     showInterval(xZeroValue, xZeroValue.addSecs(xAxis->max() - xAxis->min()));



    mousePrevPos = mouseCurPos;
    //update();
}

void plotter::setTSetpoint(float setpoint)
{
    //TSetpoint = setpoint;
    QStringList labels = axisY3->categoriesLabels();
    foreach (QString label, labels) {
        axisY3->remove(label);
    }
    axisY3->append(QString::number(setpoint), setpoint);
}
