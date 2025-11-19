#include "priop.h"

PRIOP::PRIOP() {}

size_t PRIOP::getNextTask(std::vector<TaskControlBlock *> tasks)
{
    auto it_max = std::max_element(tasks.begin(),
                                   tasks.end(),
                                   [](TaskControlBlock *a, TaskControlBlock *b) {
                                       return a->get_priority() < b->get_priority();
                                   });

    return std::distance(tasks.begin(), it_max);
}
