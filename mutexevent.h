#ifndef MUTEXEVENT_H
#define MUTEXEVENT_H

#include "mutexeventtype.h"

class MutexEvent
{
private:
    MutexEventType type;
    int instant;
    int id;

public:
    MutexEvent(int id, int instant, MutexEventType type);
    MutexEventType getType();
    int getInstant();
    int getId();
};

#endif // MUTEXEVENT_H
