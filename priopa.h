#ifndef PRIOPA_H
#define PRIOPA_H

#include "scheduler.h"
#include <vector>

// Priority Preemptive Aging
class PRIOPa : public Scheduler
{
public:
    PRIOPa(int alpha);
    virtual ~PRIOPa() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;

private:
    int alpha;
};

#endif // PRIOPA_H
