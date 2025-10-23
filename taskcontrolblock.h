#ifndef TASKCONTROLBLOCK_H
#define TASKCONTROLBLOCK_H

#include <QColor>
#include <QString>

class TaskControlBlock
{
public:
    TaskControlBlock(QString id, QColor color, int start_time, int duration, int priority);
    QString const get_id();
    int const get_priority();
    QColor const get_color();
    int const get_start_time();
    int const get_duration();

private:
    QString id;
    QColor color;
    int start_time;
    int duration;
    int priority;
};

#endif // TASKCONTROLBLOCK_H
