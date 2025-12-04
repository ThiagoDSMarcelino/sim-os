#include "sleepingtask.h"
#include <QGuiApplication>

SleepingTask::SleepingTask(TaskControlBlock *task, int duration)
{
    this->task = task;
    this->duration = duration;
    this->elepsedTime = 0;
}

void SleepingTask::runTime()
{
    if (this->elepsedTime >= this->duration) {
        qDebug() << "Essa tarefa ja deveria ter acordado";
        return;
    }

    this->elepsedTime++;
}

void SleepingTask::undoRunTime()
{
    if (this->elepsedTime < 0) {
        qDebug() << "Essa tarefa ja esta no inicio do spleeping";
        return;
    }

    this->elepsedTime--;
}

bool SleepingTask::hasFinish()
{
    return this->duration == this->elepsedTime;
}

int SleepingTask::getDuration()
{
    return this->duration;
}

TaskControlBlock *SleepingTask::getTask()
{
    return this->task;
}
