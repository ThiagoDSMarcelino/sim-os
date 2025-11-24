#ifndef TASKCONTROLBLOCK_H
#define TASKCONTROLBLOCK_H

#include <QColor>
#include <QString>

#define MAX_PRIORITY 99
#define MIN_PRIORITY 0

class TaskControlBlock
{
public:
    TaskControlBlock(QString id, QColor color, int start_time, int duration, int priority);
    QString const getId();
    int const getPriority();
    QColor const getColor();
    int const getStartTime();
    void run();
    bool hasFinish();
    int const getRemainingTime();
    int const getDynamicPriority();
    void incresseDynamicPriority(int alpha);
    void resetDynamicPriority();

private:
    QString id;
    QColor color;
    int startTime;
    int duration;
    int priority;
    int dynamicPriority;
    int elepsedTime;
};

#endif // TASKCONTROLBLOCK_H
