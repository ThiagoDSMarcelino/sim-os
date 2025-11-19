#ifndef SRTF_H
#define SRTF_H

#include "scheduler.h"
#include <vector>

class SRTF : public Scheduler
{
public:
    SRTF();
    virtual ~SRTF() override = default;

    size_t getNextTask(std::vector<TaskControlBlock *> tasks) override;
};

#endif // SRTF_H
