#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QString>
#include <vector>
#include "scheduler.h"
#include "taskcontrolblock.h"

#define MAX_PRIORITY 99
#define MIN_PRIORITY 0

class Simulator
{
private:
    Simulator(Scheduler* scheduler, int quantum, std::vector<TaskControlBlock*> tasks);
    ~Simulator();

    static Simulator *instance;

    Scheduler *scheduler;
    int quantum;
    std::vector<TaskControlBlock *> ready_tasks;
    std::vector<TaskControlBlock *> suspended_tasks;
    std::vector<TaskControlBlock *> loaded_tasks;

public:
    static std::vector<QString> load(const QString filePath);
    static void free();
    static Simulator* getInstance();
};

#endif // SIMULATOR_H
