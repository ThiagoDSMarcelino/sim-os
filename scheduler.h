#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "taskcontrolblock.h"
#include <vector>

class Scheduler
{
public:
    virtual ~Scheduler() = default;

    virtual TaskControlBlock *getNextTask() = 0;
    virtual void addTask(TaskControlBlock *task) = 0;
    virtual void sleepTask(QString id) = 0;
    virtual void wakeTask(QString id) = 0;
    virtual std::vector<TaskControlBlock *> getTasks() = 0;

protected:
    Scheduler() = default;
};

#endif // SCHEDULER_H
