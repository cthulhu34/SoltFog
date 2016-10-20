#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include <QObject>

class LogicController : public QObject
{
    Q_OBJECT
public:
    explicit LogicController(QObject *parent = 0);

signals:

public slots:
};

#endif // LOGICCONTROLLER_H