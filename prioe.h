#ifndef PRIOE_H
#define PRIOE_H

#include "scheduler.h"
#include <vector>

class PRIOe : public Scheduler
{
public:
    PRIOe(int alpha);
    virtual ~PRIOe() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;

private:
    int alpha;
};

#endif // PRIOE_H
