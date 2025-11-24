#ifndef PRIO_H
#define PRIO_H

#include "scheduler.h"
#include <vector>

class PRIO : public Scheduler
{
public:
    PRIO();
    virtual ~PRIO() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;
};

#endif // PRIO_H
