#ifndef HISTORYDATA_H
#define HISTORYDATA_H

#include "taskcontrolblock.h"
#include <vector>

class HistoryData
{
private:
    TaskControlBlock *runningTask;
    std::vector<TaskControlBlock *> activeTasks;
    int instant;

public:
    HistoryData(int instant,
                TaskControlBlock *runningTaks,
                std::vector<TaskControlBlock *> activeTasks);

    TaskControlBlock *getRunningTask();
    std::vector<TaskControlBlock *> getActiveTasks();
    int getInstant();
};

#endif // HISTORYDATA_H
