#ifndef PRIOE_H
#define PRIOE_H

#include "scheduler.h"
#include <vector>

class PRIOe : public Scheduler
{
public:
    PRIOe();
    virtual ~PRIOe() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;
};

#endif // PRIOE_H
