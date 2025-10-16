#ifndef TASKCONTROLBLOCK_H
#define TASKCONTROLBLOCK_H

#include <string>
#include <vector>

class TaskControlBlock
{
public:
    TaskControlBlock(int id, std::string color, int start_time, int duration, int priority, std::vector<int> syscalls);
    int get_id();
    int get_priority();
    const std::string *get_color();
    int get_start_time();
    int get_duration();

private:
    int id;
    std::string color;
    int start_time;
    int duration;
    int priority;
    std::vector<int> syscalls;
};

#endif // TASKCONTROLBLOCK_H
