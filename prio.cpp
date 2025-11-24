#include "prio.h"

PRIO::PRIO() {}

size_t PRIO::getNextTask(std::vector<TaskControlBlock *> tasks)
{
    auto it_max = std::max_element(tasks.begin(),
                                   tasks.end(),
                                   [](TaskControlBlock *a, TaskControlBlock *b) {
                                       return a->getPriority() < b->getPriority();
                                   });

    return std::distance(tasks.begin(), it_max);
}
