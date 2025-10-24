#include "simulator.h"

#include <QColor>
#include <QFile>
#include <QMessageBox>
#include "fcfs.h"
#include <algorithm>

Simulator *Simulator::instance = nullptr;

Simulator::Simulator(Scheduler* scheduler, int quantum, std::vector<TaskControlBlock*> tasks)
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

Scheduler *getScheduler(QString schedulerName, std::vector<QString> *errors)
{
    if (schedulerName == "FCFS") {
        return new FCFS();
    }

    errors->push_back("Nome de escalonador inválido");
    return nullptr;
}

std::vector<QString> Simulator::load(const QString filePath)
{
    std::vector<QString> errors;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errors.push_back("Não foi possível abrir o arquivo selecionado.");
        return errors;
    }

    QTextStream in(&file);

    QString line = in.readLine();

    if (line.isEmpty()) {
        errors.push_back("Arquivo de configuração está vazio");
        return errors;
    }

    QStringList values = line.split(";");

    if (values.length() != 2) {
        errors.push_back("Formação da sistema está incorreta");
        return errors;
    }

    auto scheduler = getScheduler(values[0], &errors);
    int quantum = values[1].toInt();

    if (quantum < 1)
    {
        errors.push_back("Somente números inteiros positivos são validos como quantum");
        return errors;
    }

    std::vector<QString> used_ids;
    std::vector<TaskControlBlock *> tcb_list;
    while (!in.atEnd()) {
        line = in.readLine();
        values = line.split(";");

        if (values.length() < 5)
        {
            errors.push_back("Formatação de uma tarefa está incorreta");
            continue;
        }

        QString id = values[0];
        if (std::find(used_ids.begin(), used_ids.end(), id) != used_ids.end()) {
            errors.push_back("ID duplicado");
        } else {
            used_ids.push_back(id);
        }

        bool successfulParse;
        int colorNum = values[1].toInt(&successfulParse);
        if (!successfulParse) {
            errors.push_back("Valores inteiros positivos são validos como cor");
        }
        QColor color(colorNum);

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

        int priority = values[4].toInt(&successfulParse);
        if (!successfulParse) {
            errors.push_back("Valores inteiros positivos são validos como prioridade");
        }
        else if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
            errors.push_back("A prioridade deve ser um valor entre 0 e 99");
        }

        if (errors.empty())
        {
            tcb_list.push_back(new TaskControlBlock(id, color, start_time, duration, priority));
        }
    }

    file.close();

    if (!errors.empty()) {
        return errors;
    }

    Simulator::instance = new Simulator(scheduler, quantum, tcb_list);

    return errors;
}

void Simulator::free()
{
    if (instance == NULL) {
        return;
    }

    delete instance;
}

Simulator* Simulator::getInstance()
{
    Q_ASSERT_X(instance != nullptr, "Simulator::getInstance()", "A instância do simulador não foi criada. Chame Simulator::load() primeiro.");
    return instance;
}

void Simulator::start()
{
    this->time = 0;
    this->runQuantum();
}

void Simulator::runQuantum()
{
    auto running_task = this->scheduler->getNextTask();

    for (int count = this->quantum; count > 0; count--) {
        bool newTaskSyscall = false;

        // TODO: probabily exist a better way to do this
        for (int i = 0; i < this->tasks.size(); i++) {
            if (std::find(this->loaded_tasks.begin(), this->loaded_tasks.end(), i)
                != this->loaded_tasks.end()) {
                continue;
            }

            if (tasks.at(i)->get_start_time() <= this->time) {
                this->scheduler->addTask(tasks[i]);
                this->loaded_tasks.push_back(i);
                newTaskSyscall = true;
            }
        }

        if (newTaskSyscall) {
            break;
        }

        this->time++;
        std::vector<TaskControlBlock *> activeTasks;

        if (running_task != nullptr) {
            running_task->run();
            activeTasks.push_back(running_task);
        }

        for (auto task : this->scheduler->getTasks()) {
            activeTasks.push_back(task);
        }

        this->history.push_back(HistoryData(this->time, running_task, activeTasks));

        if (running_task != nullptr && running_task->hasFinish()) {
            break;
        }
    }

    if (running_task != nullptr && !running_task->hasFinish()) {
        this->scheduler->addTask(running_task);
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
    if (this->tasks.size() != this->loaded_tasks.size()) {
        return false;
    }

    for (auto task : this->tasks) {
        if (!task->hasFinish()) {
            return false;
        }
    }

    return true;
}
