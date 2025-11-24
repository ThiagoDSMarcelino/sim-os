#include "taskcontrolblock.h"
#include <QGuiApplication>

TaskControlBlock::TaskControlBlock(
    QString id, QColor color, int start_time, int duration, int priority)
{
    this->id = id;
    this->color = color;
    this->startTime = start_time;
    this->duration = duration;
    this->priority = priority;
    this->dynamicPriority = priority;
    this->elepsedTime = 0;
}

QString const TaskControlBlock::getId()
{
    return this->id;
}

int const TaskControlBlock::getPriority()
{
    return this->priority;
}

QColor const TaskControlBlock::getColor()
{
    return this->color;
}

int const TaskControlBlock::getStartTime()
{
    return this->startTime;
}

void TaskControlBlock::run()
{
    if (this->hasFinish()) {
        qDebug() << "task already finished";
        return;
    }

    this->elepsedTime++;
}

bool TaskControlBlock::hasFinish()
{
    return this->duration == this->elepsedTime;
}

int const TaskControlBlock::getRemainingTime()
{
    return this->duration - this->elepsedTime;
}

int const TaskControlBlock::getDynamicPriority()
{
    return this->dynamicPriority;
}

void TaskControlBlock::incresseDynamicPriority(int alpha)
{
    if (alpha < 1) {
        qDebug() << "invalid value for alpha";
    }

    this->dynamicPriority += alpha;

    if (this->dynamicPriority > MAX_PRIORITY) {
        this->dynamicPriority = MAX_PRIORITY;
    }
}

void TaskControlBlock::resetDynamicPriority()
{
    this->dynamicPriority = this->priority;
}
