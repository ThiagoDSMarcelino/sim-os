#include "simulator.h"

#include <QColor>
#include <QFile>
#include <QMessageBox>
#include "prio.h"
#include "prioe.h"
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

    if (schedulerName == "PRIO")
    {
        return new PRIO();
    }

    if (schedulerName == "PROPe")
    {
        return new PRIOe(alpha);
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
        alpha = values[1].toInt();
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

        if (values.length() < 5)
        {
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

        if (errors.empty())
        {
            tcb_list.push_back(new TaskControlBlock(id, color, start_time, duration, priority));
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

        if (running_task != nullptr)
        {
            running_task->run();
            activeTasks.push_back(running_task);
        }

        for (auto task : this->active_tasks)
        {
            activeTasks.push_back(task);
        }

        this->history.push_back(HistoryData(this->time, running_task, activeTasks));

        this->time++;

        if (running_task != nullptr && running_task->hasFinish())
        {
            break;
        }
    }

    if (running_task != nullptr && !running_task->hasFinish())
    {
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
