#include "event.h"

Event::Event(int id, int instant, EventType type)
{
    this->id = id;
    this->type = type;
    this->instant = instant;
}

int Event::getId()
{
    return this->id;
}

EventType Event::getType()
{
    return this->type;
}

int Event::getInstant()
{
    return this->instant;
}
