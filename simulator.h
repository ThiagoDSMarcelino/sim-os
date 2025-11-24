#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QString>
#include "historydata.h"
#include "scheduler.h"
#include "taskcontrolblock.h"
#include <vector>

class Simulator
{
private:
    Simulator(Scheduler *scheduler, int quantum, std::vector<TaskControlBlock *> tasks);
    ~Simulator();

    static Simulator *instance;

    Scheduler *scheduler;
    int quantum;
    std::vector<TaskControlBlock *> tasks;
    std::vector<size_t> loaded_tasks;
    std::vector<TaskControlBlock *> active_tasks;

    unsigned long long time;

    std::vector<HistoryData> history;
    TaskControlBlock *getRunningTask();

    bool loadTasks();

public:
    static std::vector<QString> load(const QString filePath);
    static void free();
    static Simulator *getInstance();
    void start();
    void runQuantum();
    const std::vector<TaskControlBlock *> getTasks();
    int getTime();
    std::vector<HistoryData> getHistory();
    bool const hasFinished();
};

#endif // SIMULATOR_H
