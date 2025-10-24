#ifndef FCFS_H
#define FCFS_H

#include "scheduler.h"
#include <vector>

class FCFS : public Scheduler
{
public:
    FCFS();
    virtual ~FCFS() override = default;

    TaskControlBlock *getNextTask() override;
    void addTask(TaskControlBlock *task) override;
    void sleepTask(QString id) override;
    void wakeTask(QString id) override;
    std::vector<TaskControlBlock *> getTasks() override;

private:
    std::vector<TaskControlBlock *> ready_queue;
    std::vector<TaskControlBlock *> sleeping_tasks;
};

#endif // FCFS_H
