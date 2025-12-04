#include "mutexevent.h"

MutexEvent::MutexEvent(int id, int instant, MutexEventType type)
{
    this->id = id;
    this->type = type;
    this->instant = instant;
}

int MutexEvent::getId()
{
    return this->id;
}

MutexEventType MutexEvent::getType()
{
    return this->type;
}

int MutexEvent::getInstant()
{
    return this->instant;
}
