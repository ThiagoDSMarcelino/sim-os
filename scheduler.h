#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "taskcontrolblock.h"

class Scheduler
{
public:
    Scheduler();

    virtual TaskControlBlock *next(std::vector<TaskControlBlock *> tasks) = 0;
};

#endif // SCHEDULER_H
