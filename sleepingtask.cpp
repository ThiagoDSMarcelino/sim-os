#include "sleepingtask.h"
#include <QGuiApplication>

SleepingTask::SleepingTask(TaskControlBlock *task, int duration)
{
    this->task = task;
    this->duration = duration;
}

void SleepingTask::runTime()
{
    if (this->duration <= 0) {
        qDebug() << "Essa tarefa ja deveria ter acordado";
        return;
    }

    this->duration--;
}

bool SleepingTask::hasFinish()
{
    return this->duration == 0;
}
TaskControlBlock *SleepingTask::getTask()
{
    return this->task;
}
