#include "mutex.h"
#include <QGuiApplication>

Mutex::Mutex() {}

bool Mutex::lock(TaskControlBlock *task)
{
    if (this->free) {
        this->free = false;
        return true;
    }

    this->blockedTasks.push(task);

    return false;
}

TaskControlBlock *Mutex::unlock()
{
    if (this->free) {
        qDebug() << "Possivel erro";
        return nullptr;
    }

    this->free = true;

    if (this->blockedTasks.empty()) {
        return nullptr;
    }

    auto task = this->blockedTasks.front();
    this->blockedTasks.pop();

    return task;
}
