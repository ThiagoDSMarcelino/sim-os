#include "historydata.h"

HistoryData::HistoryData(int instant,
                         TaskControlBlock *runningTaks,
                         std::vector<TaskControlBlock *> activeTasks,
                         std::vector<std::tuple<TaskControlBlock *, int>> wakedTasks)
{
    this->instant = instant;
    this->runningTask = runningTaks;
    this->activeTasks = activeTasks;
    this->wakedTasks = wakedTasks;
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

std::vector<std::tuple<TaskControlBlock *, int>> HistoryData::getWakedTasks()
{
    return this->wakedTasks;
}
