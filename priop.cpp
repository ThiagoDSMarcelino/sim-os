#include "priop.h"

PRIOP::PRIOP() {}


void PRIOP::addTask(TaskControlBlock *task)
{
    if (!task) {
        return;
    }
    this->ready_queue.push_back(task);
}

TaskControlBlock *PRIOP::getNextTask()
{
    if (ready_queue.empty()) {
        return nullptr;
    }

    auto it_max = std::max_element(
        this->ready_queue.begin(),
        this->ready_queue.end(),
        [](TaskControlBlock *a, TaskControlBlock *b) {
            return a->get_priority() < b->get_priority();
        });

    TaskControlBlock *next_task = *it_max;
    this->ready_queue.erase(it_max);

    return next_task;
}


void PRIOP::sleepTask(QString id)
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


void PRIOP::wakeTask(QString id)
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


std::vector<TaskControlBlock *> PRIOP::getTasks()
{
    return this->ready_queue;
}
