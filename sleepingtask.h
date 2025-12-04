#ifndef SLEEPINGTASK_H
#define SLEEPINGTASK_H

#include "taskcontrolblock.h"

class SleepingTask
{
private:
    TaskControlBlock *task;
    int duration;
    int elepsedTime;

public:
    SleepingTask(TaskControlBlock *task, int duration);
    void runTime();
    void undoRunTime();
    bool hasFinish();
    int getDuration();
    TaskControlBlock *getTask();
};

#endif // SLEEPINGTASK_H
