#include "ioevent.h"

IOEvent::IOEvent(int instant, int duration)
{
    this->instant = instant;
    this->duration = duration;
}

int IOEvent::getDuration()
{
    return this->duration;
}

int IOEvent::getInstant()
{
    return this->instant;
}
