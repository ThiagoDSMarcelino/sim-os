#ifndef EVENT_H
#define EVENT_H

#include "eventtype.h"

class Event
{
private:
    EventType type;
    int instant;
    int id;

public:
    Event(int id, int instant, EventType type);
    EventType getType();
    int getInstant();
    int getId();
};

#endif // EVENT_H
