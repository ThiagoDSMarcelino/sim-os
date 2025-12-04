#ifndef TASKCONTROLBLOCK_H
#define TASKCONTROLBLOCK_H

#include <QColor>
#include <QString>
#include "ioevent.h"
#include "mutexevent.h"

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
                     std::vector<MutexEvent *> mutexEvents,
                     std::vector<IOEvent *> ioEvents);

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
    std::vector<MutexEvent *> getMutexEvents();
    std::vector<IOEvent *> getIOEvents();

private:
    QString id;
    QColor color;
    int startTime;
    int duration;
    int priority;
    int dynamicPriority;
    int elepsedTime;
    std::vector<MutexEvent *> mutexEvents;
    std::vector<IOEvent *> ioEvents;
};

#endif // TASKCONTROLBLOCK_H
