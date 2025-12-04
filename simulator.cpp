#include "simulator.h"

#include <QColor>
#include <QFile>
#include <QMessageBox>
#include "priop.h"
#include "priopa.h"
#include "roundrobin.h"
#include "srtf.h"
#include <algorithm>

Simulator *Simulator::instance = nullptr;

Simulator::Simulator(Scheduler *scheduler, int quantum, std::vector<TaskControlBlock *> tasks)
{
    this->tasks = tasks;
    this->scheduler = scheduler;
    this->quantum = quantum;
    this->instance = this;
}

Simulator::~Simulator()
{
    delete this->scheduler;
}

Scheduler *getScheduler(QString schedulerName, int alpha, std::vector<QString> *errors)
{
    if (schedulerName == "RR")
    {
        return new RoundRobin();
    }

    if (schedulerName == "SRTF")
    {
        return new SRTF();
    }

    if (schedulerName == "PRIOP") {
        return new PRIOP();
    }

    // PRIOPe is in portuguese
    if (schedulerName == "PRIOPa" || schedulerName == "PRIOPe" || schedulerName == "PRIOPEnv") {
        return new PRIOPa(alpha);
    }

    errors->push_back("Nome de escalonador inválido");
    return nullptr;
}

std::vector<QString> Simulator::load(const QString filePath)
{
    std::vector<QString> errors;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errors.push_back("Não foi possível abrir o arquivo selecionado.");
        return errors;
    }

    QTextStream in(&file);

    QString line = in.readLine();

    if (line.isEmpty())
    {
        errors.push_back("Arquivo de configuração está vazio");
        return errors;
    }

    QStringList values = line.split(";");

    if (values.length() < 2 || values.length() > 3)
    {
        errors.push_back("Formação da sistema está incorreta");
        return errors;
    }

    int alpha = 1;
    if (values.length() == 3)
    {
        alpha = values[2].toInt();
        if (alpha < 1)
        {
            errors.push_back(
                "Somente números inteiros positivos diferentes zero são validos como alpha");
        }
    }

    auto scheduler = getScheduler(values[0], alpha, &errors);

    int quantum = values[1].toInt();
    if (quantum < 1)
    {
        errors.push_back(
            "Somente números inteiros positivos diferentes zero são validos como quantum");
    }

    std::vector<QString> used_ids;
    std::vector<TaskControlBlock *> tcb_list;
    while (!in.atEnd())
    {
        line = in.readLine();
        values = line.split(";");

        if (values.length() < 5 || values.length() > 6) {
            errors.push_back("Formatação de uma tarefa está incorreta");
            continue;
        }

        QString id = values[0];
        if (std::find(used_ids.begin(), used_ids.end(), id) != used_ids.end())
        {
            errors.push_back("ID duplicado");
        }
        else
        {
            used_ids.push_back(id);
        }

        QString colorString = values[1];
        QColor color(colorString);
        if (!color.isValid())
        {
            errors.push_back("Código de cor inválido (use formato Hex #RRGGBB)");
        }

        int start_time = values[2].toInt();
        if (start_time < 0)
        {
            errors.push_back("Valores inteiros positivos são validos como tempo de ingresso");
        }

        int duration = values[3].toInt();
        if (duration < 1)
        {
            errors.push_back("Valores inteiros positivos são validos como tempo de duração");
        }

        bool successfulParse;
        int priority = values[4].toInt(&successfulParse);
        if (!successfulParse)
        {
            errors.push_back("Valores inteiros positivos são validos como prioridade");
        }
        else if (priority < MIN_PRIORITY || priority > MAX_PRIORITY)
        {
            errors.push_back("A prioridade deve ser um valor entre 0 e 99");
        }

        std::vector<MutexEvent *> mutexEvents;
        std::vector<IOEvent *> ioEvents;
        int instant;

        QRegularExpression mutexLockRegex("^ML(\\d+):(\\d+)$");
        QRegularExpression mutexUnlockRegex("^MU(\\d+):(\\d+)$");
        QRegularExpression IORegex("^IO:(\\d+)-(\\d+)$");
        QRegularExpressionMatch match;

        if (values.length() == 6) {
            for (auto &event : values.at(5).split(',')) {
                if (event.startsWith("ML")) {
                    match = mutexLockRegex.match(event);

                    if (match.hasMatch()) {
                        mutexEvents.push_back(new MutexEvent(match.captured(1).toInt(),
                                                             match.captured(2).toInt(),
                                                             MutexEventType::MutexLock));
                    } else {
                        errors.push_back("Evento de Mutex Lock mal formatado");
                        qDebug() << event;
                    }

                    continue;
                }

                if (event.startsWith("MU")) {
                    match = mutexUnlockRegex.match(event);

                    if (match.hasMatch()) {
                        mutexEvents.push_back(new MutexEvent(match.captured(1).toInt(),
                                                             match.captured(2).toInt(),
                                                             MutexEventType::MutexUnlock));
                    } else {
                        errors.push_back("Evento de Mutex Unlock mal formatado");
                        qDebug() << event;
                    }

                    continue;
                }

                if (event.startsWith("IO")) {
                    match = IORegex.match(event);

                    if (match.hasMatch()) {
                        ioEvents.push_back(
                            new IOEvent(match.captured(1).toInt(), match.captured(2).toInt() + 1));
                    } else {
                        errors.push_back("Evento de IO mal formatado");
                        qDebug() << event;
                    }

                    continue;
                }

                errors.push_back("Evento desconhecido");
            }
        }

        if (errors.empty()) {
            tcb_list.push_back(new TaskControlBlock(
                id, color, start_time, duration, priority, mutexEvents, ioEvents));
        }
    }

    file.close();

    if (!errors.empty())
    {
        if (scheduler != nullptr)
        {
            delete scheduler;
        }

        return errors;
    }

    Simulator::instance = new Simulator(scheduler, quantum, tcb_list);

    return errors;
}

void Simulator::free()
{
    if (instance == NULL)
    {
        return;
    }

    delete instance;
}

Simulator *Simulator::getInstance()
{
    Q_ASSERT_X(instance != nullptr, "Simulator::getInstance()", "A instância do simulador não foi criada. Chame Simulator::load() primeiro.");
    return instance;
}

void Simulator::start()
{
    this->time = 0;
    this->runQuantum();
}

TaskControlBlock *Simulator::getRunningTask()
{
    if (this->active_tasks.size() == 0)
    {
        return nullptr;
    }

    size_t nextTaskIndex = this->scheduler->getNextTask(this->active_tasks);

    auto nextTask = this->active_tasks.at(nextTaskIndex);

    this->active_tasks.erase(this->active_tasks.begin() + nextTaskIndex);

    return nextTask;
}

bool Simulator::loadTasks()
{
    bool anyTaskLoaded = false;

    for (int i = 0; i < this->tasks.size(); i++)
    {
        auto it = std::find(this->loaded_tasks.begin(), this->loaded_tasks.end(), i);

        if (it != this->loaded_tasks.end())
        {
            continue;
        }

        if (tasks.at(i)->getStartTime() <= this->time)
        {
            this->loaded_tasks.push_back(i);
            this->active_tasks.push_back(tasks[i]);
            anyTaskLoaded = true;
        }
    }

    return anyTaskLoaded;
}

void Simulator::runQuantum()
{
    this->loadTasks();

    TaskControlBlock *running_task = this->getRunningTask();
    bool eventFailure = false;
    bool ioSyscall = false;

    for (int count = this->quantum; count > 0; count--)
    {
        if (count < this->quantum)
        {
            bool hasNewTaskSysCall = this->loadTasks();

            if (hasNewTaskSysCall)
            {
                break;
            }
        }

        std::vector<TaskControlBlock *> activeTasks;

        if (running_task != nullptr) {
            for (auto event : running_task->getMutexEvents()) {
                qDebug() << "Event ID: " << event->getId()
                         << " | Task ID: " << running_task->getId();
                auto mutex = this->getMutex(event->getId());

                if (event->getType() == MutexEventType::MutexLock) {
                    if (!mutex->lock(running_task)) {
                        qDebug() << "Mutex lock failure";
                        eventFailure = true;
                        break;
                    }

                    qDebug() << "Mutex lock success";

                    continue;
                }

                if (event->getType() == MutexEventType::MutexUnlock) {
                    qDebug() << "Mutex unlock";

                    auto unblockedTask = mutex->unlock();

                    if (unblockedTask != nullptr) {
                        this->active_tasks.push_back(unblockedTask);
                    }

                    continue;
                }
            }

            auto ioEvents = running_task->getIOEvents();
            if (!ioEvents.empty()) {
                int maxDuration = 0;

                for (auto event : ioEvents) {
                    if (maxDuration < event->getDuration()) {
                        maxDuration = event->getDuration();
                    }
                }

                this->sleepingTasks.push_back(new SleepingTask(running_task, maxDuration));
                qDebug() << "Task ID: " << running_task->getId() << " | Duration: " << maxDuration;
                ioSyscall = true;
            }

            if (eventFailure) {
                break;
            }

            running_task->run();
            activeTasks.push_back(running_task);
        }

        for (auto task : this->active_tasks) {
            activeTasks.push_back(task);
        }

        auto wakedTasks = this->updateSleepingTasks();

        this->history.push_back(HistoryData(this->time, running_task, activeTasks, wakedTasks));

        this->time++;

        if (ioSyscall || eventFailure) {
            break;
        }

        if (running_task != nullptr && running_task->hasFinish()) {
            break;
        }
    }

    if (running_task != nullptr && !running_task->hasFinish() && !eventFailure && !ioSyscall) {
        this->active_tasks.push_back(running_task);
    }
}

const std::vector<TaskControlBlock *> Simulator::getTasks()
{
    return this->tasks;
}

int Simulator::getTime()
{
    return this->time;
}

std::vector<HistoryData> Simulator::getHistory()
{
    return this->history;
}

bool const Simulator::hasFinished()
{
    if (this->tasks.size() != this->loaded_tasks.size())
    {
        return false;
    }

    for (auto task : this->tasks)
    {
        if (!task->hasFinish())
        {
            return false;
        }
    }

    return true;
}

Mutex *Simulator::getMutex(int id)
{
    auto [iter, inserted] = this->mutexes.try_emplace(id, new Mutex());

    return iter->second;
}

std::vector<std::tuple<TaskControlBlock *, int>> Simulator::updateSleepingTasks()
{
    std::vector<std::tuple<TaskControlBlock *, int>> wakedTasks;

    for (auto it = this->sleepingTasks.begin(); it != this->sleepingTasks.end();) {
        auto sleepingTask = *it;

        sleepingTask->runTime();

        if (!sleepingTask->hasFinish()) {
            it++;
            continue;
        }

        this->active_tasks.push_back(sleepingTask->getTask());
        it = this->sleepingTasks.erase(it);

        wakedTasks.push_back(std::make_tuple(sleepingTask->getTask(), sleepingTask->getDuration()));
    }

    return wakedTasks;
}

void Simulator::undoQuantun()
{
    this->time--;
    auto historyData = this->history.back();
    this->history.pop_back();

    auto runningTask = historyData.getRunningTask();

    runningTask->undoRun();

    for (auto event : runningTask->getMutexEvents()) {
        auto mutex = this->getMutex(event->getId());

        if (event->getType() == MutexEventType::MutexLock) {
            auto unblockedTask = mutex->unlock();

            if (unblockedTask != nullptr) {
                this->active_tasks.push_back(unblockedTask);
            }

            continue;
        }

        if (event->getType() == MutexEventType::MutexUnlock) {
            if (!mutex->lock(runningTask)) {
                qDebug() << "ERROR: Mutex lock failure";
                break;
            }

            continue;
        }
    }

    auto ioEvents = runningTask->getIOEvents();
    if (!ioEvents.empty()) {
        for (auto it = this->sleepingTasks.begin(); it != this->sleepingTasks.end();) {
            auto sleepingTask = *it;

            if (sleepingTask->getTask()->getId() == runningTask->getId()) {
                it = this->sleepingTasks.erase(it);
            }
        }
    }

    for (auto spleepingTask : this->sleepingTasks) {
        spleepingTask->undoRunTime();
    }

    for (auto pair : historyData.getWakedTasks()) {
        auto task = std::get<0>(pair);
        auto duration = std::get<1>(pair);

        for (auto it = this->active_tasks.begin(); it != this->active_tasks.end();) {
            auto activeTask = *it;

            if (activeTask->getId() == task->getId()) {
                this->active_tasks.erase(it);
                continue;
            }

            it++;
        }

        this->sleepingTasks.push_back(new SleepingTask(task, duration - 1));
    }

    for (auto task : this->active_tasks) {
        if (task->getStartTime() == historyData.getInstant()) {
            this->loaded_tasks.pop_back();
        }
    }

    if (runningTask->getStartTime() == historyData.getInstant()) {
        return;
    }

    bool taskAlreadyInActiveTasks = false;
    for (auto task : this->active_tasks) {
        if (task->getId() == runningTask->getId()) {
            taskAlreadyInActiveTasks = true;
            break;
        }
    }

    if (taskAlreadyInActiveTasks) {
        return;
    }

    this->active_tasks.push_back(runningTask);
}
