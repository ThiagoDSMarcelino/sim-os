#include "srtf.h"
#include <algorithm>

SRTF::SRTF() {}
size_t SRTF::getNextTask(std::vector<TaskControlBlock *> tasks)
{
    auto it_min = std::min_element(tasks.begin(),
                                   tasks.end(),
                                   [](TaskControlBlock *a, TaskControlBlock *b) {
                                       return a->getRemainingTime() < b->getRemainingTime();
                                   });

    return std::distance(tasks.begin(), it_min);
}
