#ifndef PRIOP_H
#define PRIOP_H

#include "scheduler.h"
#include <vector>

class PRIOP : public Scheduler
{
public:
    PRIOP();
    virtual ~PRIOP() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;
};

#endif // PRIOP_H
