#ifndef RoundRobin_H
#define RoundRobin_H

#include "scheduler.h"
#include <vector>

class RoundRobin : public Scheduler
{
public:
    RoundRobin();
    virtual ~RoundRobin() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;
};

#endif // RoundRobin_H
