#ifndef IOEVENT_H
#define IOEVENT_H

class IOEvent
{
private:
    int instant;
    int duration;

public:
    IOEvent(int instant, int duration);
    int getInstant();
    int getDuration();
};

#endif // IOEVENT_H
