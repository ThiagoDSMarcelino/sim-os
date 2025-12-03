#ifndef MUTEX_H
#define MUTEX_H

#include "taskcontrolblock.h"
#include <queue>

class Mutex
{
private:
    std::queue<TaskControlBlock *> blockedTasks;
    bool free;

public:
    Mutex();

    bool lock(TaskControlBlock *task);
    TaskControlBlock *unlock();
};

#endif // MUTEX_H
