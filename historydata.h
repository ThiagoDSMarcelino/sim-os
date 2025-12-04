#ifndef HISTORYDATA_H
#define HISTORYDATA_H

#include "taskcontrolblock.h"
#include <tuple>
#include <vector>

class HistoryData
{
private:
    TaskControlBlock *runningTask;
    std::vector<TaskControlBlock *> activeTasks;
    int instant;
    std::vector<std::tuple<TaskControlBlock *, int>> wakedTasks;

public:
    HistoryData(int instant,
                TaskControlBlock *runningTaks,
                std::vector<TaskControlBlock *> activeTasks,
                std::vector<std::tuple<TaskControlBlock *, int>> wakedTasks);

    TaskControlBlock *getRunningTask();
    std::vector<TaskControlBlock *> getActiveTasks();
    std::vector<std::tuple<TaskControlBlock *, int>> getWakedTasks();
    int getInstant();
};

#endif // HISTORYDATA_H
