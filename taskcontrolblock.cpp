#include "taskcontrolblock.h"

TaskControlBlock::TaskControlBlock(
    QString id, QColor color, int start_time, int duration, int priority)
{
    this->id = id;
    this->color = color;
    this->start_time = start_time;
    this->duration = duration;
    this->priority = priority;
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

int const TaskControlBlock::get_duration()
{
    return this->duration;
}
