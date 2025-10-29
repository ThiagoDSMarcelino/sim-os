#ifndef SRTF_H
#define SRTF_H

#include "scheduler.h"
#include <vector>

class SRTF : public Scheduler
{
public:
    SRTF();
    virtual ~SRTF() override = default;

    TaskControlBlock *getNextTask() override;
    void addTask(TaskControlBlock *task) override;
    void sleepTask(QString id) override;
    void wakeTask(QString id) override;
    std::vector<TaskControlBlock *> getTasks() override;


private:
    std::vector<TaskControlBlock *> ready_queue;
    std::vector<TaskControlBlock *> sleeping_tasks;

};

#endif // SRTF_H
