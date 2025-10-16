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
    qDebug() << "Instância do Simulador criada.";
}

Simulator::~Simulator()
{
    delete this->scheduler;
    qDebug() << "Instância do Simulador destruída.";
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



    }

    file.close();

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
