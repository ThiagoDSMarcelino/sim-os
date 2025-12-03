#include "priopa.h"

PRIOPa::PRIOPa(int alpha)
{
    this->alpha = alpha;
}

size_t PRIOPa::getNextTask(std::vector<TaskControlBlock *> tasks)
{
    auto it_max = std::max_element(tasks.begin(),
                                   tasks.end(),
                                   [](TaskControlBlock *a, TaskControlBlock *b) {
                                       return a->getDynamicPriority() < b->getDynamicPriority();
                                   });

    size_t nextTaskIndex = std::distance(tasks.begin(), it_max);

    for (size_t i = 0; i < tasks.size(); i++) {
        tasks.at(i)->incresseDynamicPriority(this->alpha);
    }

    tasks.at(nextTaskIndex)->resetDynamicPriority();

    return nextTaskIndex;
}
