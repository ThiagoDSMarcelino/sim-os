#ifndef PRIOP_H
#define PRIOP_H

#include "scheduler.h"
#include <vector>
#include <algorithm>

class PRIOP : public Scheduler
{
public:
    PRIOP();
    virtual ~PRIOP() override = default;

    TaskControlBlock *getNextTask() override;
    void addTask(TaskControlBlock *task) override;
    void sleepTask(QString id) override;
    void wakeTask(QString id) override;
    std::vector<TaskControlBlock *> getTasks() override;

private:
    std::vector<TaskControlBlock *> ready_queue;
    std::vector<TaskControlBlock *> sleeping_tasks;
};

#endif // PRIOP_H
