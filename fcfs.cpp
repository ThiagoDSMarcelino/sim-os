#include "fcfs.h"

FCFS::FCFS() {}

void FCFS::addTask(TaskControlBlock *task)
{
    if (!task) {
        return;
    }

    this->ready_queue.push_back(task);
}

TaskControlBlock *FCFS::getNextTask()
{
    if (ready_queue.empty()) {
        return nullptr;
    }

    TaskControlBlock *next_task = this->ready_queue.front();

    this->ready_queue.erase(this->ready_queue.begin());

    return next_task;
}

void FCFS::sleepTask(QString id)
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

void FCFS::wakeTask(QString id)
{
    auto it = std::find_if(this->ready_queue.begin(),
                           this->ready_queue.end(),
                           [id](TaskControlBlock *task) { return task->get_id() == id; });

    if (it == this->ready_queue.end()) {
        return;
    }

    this->addTask(*it);
    this->ready_queue.erase(it);
}

std::vector<TaskControlBlock *> FCFS::getTasks()
{
    return this->ready_queue;
}
