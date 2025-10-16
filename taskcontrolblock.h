#ifndef TASKCONTROLBLOCK_H
#define TASKCONTROLBLOCK_H

#include <QString>

class TaskControlBlock
{
public:
    TaskControlBlock(QString id, QString color, int start_time, int duration, int priority);
    QString get_id();
    int get_priority();
    const QString *get_color();
    int get_start_time();
    int get_duration();

private:
    QString id;
    QString color;
    int start_time;
    int duration;
    int priority;
};

#endif // TASKCONTROLBLOCK_H
