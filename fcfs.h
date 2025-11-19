#ifndef FCFS_H
#define FCFS_H

#include "scheduler.h"
#include <vector>

class FCFS : public Scheduler
{
public:
    FCFS();
    virtual ~FCFS() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;
};

#endif // FCFS_H
