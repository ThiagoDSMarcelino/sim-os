#include "taskcontrolblock.h"
#include <QGuiApplication>

TaskControlBlock::TaskControlBlock(
    QString id, QColor color, int start_time, int duration, int priority)
{
    this->id = id;
    this->color = color;
    this->start_time = start_time;
    this->duration = duration;
    this->priority = priority;
    this->elepsedTime = 0;
}

QString const TaskControlBlock::get_id()
{
    return this->id;
}

int const TaskControlBlock::get_priority()
{
    return this->priority;
}

QColor const TaskControlBlock::get_color()
{
    return this->color;
}

int const TaskControlBlock::get_start_time()
{
    return this->start_time;
}

void TaskControlBlock::run()
{
    if (this->hasFinish()) {
        qDebug() << "tarefa já foi finalizada";
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
