#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QRegularExpression>
#include <QString>
#include "historydata.h"
#include "mutex.h"
#include "scheduler.h"
#include "taskcontrolblock.h"
#include <map>
#include <vector>

class Simulator
{
private:
    Simulator(Scheduler *scheduler, int quantum, std::vector<TaskControlBlock *> tasks);
    ~Simulator();

    static Simulator *instance;

    Scheduler *scheduler;
    int quantum;
    std::vector<TaskControlBlock *> tasks;
    std::vector<size_t> loaded_tasks;
    std::vector<TaskControlBlock *> active_tasks;

    unsigned long long time;

    std::vector<HistoryData> history;
    TaskControlBlock *getRunningTask();

    bool loadTasks();

    std::map<int, Mutex *> mutexes;

    Mutex *getMutex(int id);

public:
    static std::vector<QString> load(const QString filePath);
    static void free();
    static Simulator *getInstance();
    void start();
    void runQuantum();
    const std::vector<TaskControlBlock *> getTasks();
    int getTime();
    std::vector<HistoryData> getHistory();
    bool const hasFinished();
    void sleepTask(QString id);
    void wakeTask(QString id);
};

#endif // SIMULATOR_H
