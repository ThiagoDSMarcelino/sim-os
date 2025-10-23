#include "historydata.h"

HistoryData::HistoryData(int instant,
                         TaskControlBlock *runningTaks,
                         std::vector<TaskControlBlock *> activeTasks)
{
    this->instant = instant;
    this->runningTask = runningTaks;
    this->activeTasks = activeTasks;
}

TaskControlBlock *HistoryData::getRunningTask()
{
    return this->runningTask;
}

std::vector<TaskControlBlock *> HistoryData::getActiveTasks()
{
    return this->activeTasks;
}

int HistoryData::getInstant()
{
    return this->instant;
}
