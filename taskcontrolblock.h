#ifndef TASKCONTROLBLOCK_H
#define TASKCONTROLBLOCK_H

#include <QColor>
#include <QString>
#include "event.h"

#define MAX_PRIORITY 99
#define MIN_PRIORITY 0

class TaskControlBlock
{
public:
    TaskControlBlock(QString id,
                     QColor color,
                     int start_time,
                     int duration,
                     int priority,
                     std::vector<Event *> events);

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
    std::vector<Event *> getInstantEvents();

private:
    QString id;
    QColor color;
    int startTime;
    int duration;
    int priority;
    int dynamicPriority;
    int elepsedTime;
    std::vector<Event *> events;
};

#endif // TASKCONTROLBLOCK_H
