#ifndef SLEEPINGTASK_H
#define SLEEPINGTASK_H

#include "taskcontrolblock.h"

class SleepingTask
{
private:
    TaskControlBlock *task;
    int duration;

public:
    SleepingTask(TaskControlBlock *task, int duration);
    void runTime();
    bool hasFinish();
    TaskControlBlock *getTask();
};

#endif // SLEEPINGTASK_H
