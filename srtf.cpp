#include "srtf.h"
#include <algorithm>

SRTF::SRTF() {}

void SRTF::addTask(TaskControlBlock *task)
{
    if (!task) {
        return;
    }

    this->ready_queue.push_back(task);
}

TaskControlBlock *SRTF::getNextTask()
{
    if (ready_queue.empty()) {
        return nullptr;
    }

    auto it_min = std::min_element(
        this->ready_queue.begin(),
        this->ready_queue.end(),
        [](TaskControlBlock *a, TaskControlBlock *b) {
            return a->getRemainingTime() < b->getRemainingTime();
        });

    TaskControlBlock *next_task = *it_min;

    this->ready_queue.erase(it_min);

    return next_task;
}


void SRTF::sleepTask(QString id)
{
    auto it = std::find_if(this->ready_queue.begin(),
                           this->ready_queue.end(),
                           [id](TaskControlBlock *task) { return task->get_id() == id; });

    if (it == this->ready_queue.end()) {
        return;
    }

    this->sleeping_tasks.push_back(*it);
    this->ready_queue.erase(it);
}

void SRTF::wakeTask(QString id)
{
    auto it = std::find_if(this->sleeping_tasks.begin(),
                           this->sleeping_tasks.end(),
                           [id](TaskControlBlock *task) { return task->get_id() == id; });

    if (it == this->sleeping_tasks.end()) {
        return;
    }

    this->addTask(*it);
    this->sleeping_tasks.erase(it);
}

std::vector<TaskControlBlock *> SRTF::getTasks()
{
    return this->ready_queue;
}
