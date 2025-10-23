#include "simulator.h"

#include <QColor>
#include <QFile>
#include <QMessageBox>
#include "fcfs.h"
#include <algorithm>

Simulator *Simulator::instance = nullptr;

Simulator::Simulator(Scheduler* scheduler, int quantum, std::vector<TaskControlBlock*> tasks)
{
    this->running_task = nullptr;
    this->tasks = tasks;
    this->scheduler = scheduler;
    this->quantum = quantum;
    this->instance = this;
}

Simulator::~Simulator()
{
    delete this->scheduler;
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

    // TODO: get scheduler
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

    auto scheduler = new FCFS();

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
    if (this->running_task != nullptr) {
        this->scheduler->addTask(this->running_task);
    }

    // TODO: probabily exist a better way to do this
    for (int i = 0; i < this->tasks.size(); i++) {
        if (std::find(this->loaded_tasks.begin(), this->loaded_tasks.end(), i)
            != this->loaded_tasks.end()) {
            continue;
        }

        if (tasks[i]->get_start_time() <= this->time) {
            this->scheduler->addTask(tasks[i]);
            this->loaded_tasks.push_back(i);
        }
    }

    this->running_task = this->scheduler->getNextTask();
    this->time += this->quantum;

    this->history.push_back(HistoryData(time, this->tasks[1], {this->tasks[0], this->tasks[1]}));
}

const std::vector<TaskControlBlock *> Simulator::getTasks()
{
    return this->tasks;
}

TaskControlBlock *Simulator::getRunningTask()
{
    return this->running_task;
}

int Simulator::getTime()
{
    return this->time;
}

std::vector<HistoryData> Simulator::getHistory()
{
    return this->history;
}
