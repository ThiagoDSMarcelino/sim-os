#ifndef SJF_H
#define SJF_H

#include "scheduler.h"
#include <vector>

class SJF : public Scheduler
{
public:
    SJF();
    virtual ~SJF() override = default;

    TaskControlBlock *getNextTask() override;
    void addTask(TaskControlBlock *task) override;
    void sleepTask(QString id) override;
    void wakeTask(QString id) override;
    std::vector<TaskControlBlock *> getTasks() override;


private:
    std::vector<TaskControlBlock *> ready_queue;
    std::vector<TaskControlBlock *> sleeping_tasks;

};

#endif // SJF_H
