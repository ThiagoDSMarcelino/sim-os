#include "simulator.h"

#include <QFile>
#include <QMessageBox>

Simulator *Simulator::instance = nullptr;

Simulator::Simulator(Scheduler* scheduler, int quantum, std::vector<TaskControlBlock*> tasks)
{
    this->loaded_tasks = tasks;
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

        QString color = values[1];

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

    Simulator::instance = new Simulator(nullptr, quantum, tcb_list);

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

Simulator* Simulator::getInstance()
{
    Q_ASSERT_X(instance != nullptr, "Simulator::getInstance()", "A instância do simulador não foi criada. Chame Simulator::load() primeiro.");
    return instance;
}

void Simulator::start()
{
    this->time = 0;
}

void Simulator::runQuantum()
{
    this->time += this->quantum;
}
