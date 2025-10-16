#include "taskcontrolblock.h"

TaskControlBlock::TaskControlBlock(QString id, QString color, int start_time, int duration, int priority)
{
    this->id = id;
    this->color = color;
    this->start_time = start_time;
    this->duration = duration;
    this->priority = priority;
}

QString TaskControlBlock::get_id()
{
    return this->id;
}

int TaskControlBlock::get_priority()
{
    return this->priority;
}

const QString *TaskControlBlock::get_color()
{
    return &this->color;
}

int TaskControlBlock::get_start_time()
{
    return this->start_time;
}

int TaskControlBlock::get_duration()
{
    return this->duration;
}
