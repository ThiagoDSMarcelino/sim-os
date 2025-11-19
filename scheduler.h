#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "taskcontrolblock.h"
#include <vector>

class Scheduler
{
public:
    virtual ~Scheduler() = default;

    virtual size_t getNextTask(std::vector<TaskControlBlock *> tasks) = 0;

protected:
    Scheduler() = default;
};

#endif // SCHEDULER_H
