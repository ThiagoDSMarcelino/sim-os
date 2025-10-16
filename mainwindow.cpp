#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include "simulator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->startSimulationButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Abrir Arquivo"),
        QDir::homePath(),
        tr("*.txt")
        );

    ui->messagesList->clear();

    if (filePath.isEmpty()) {
        ui->messagesList->addItem("Nenhum arquivo selecionado.");
        return;
    }

    auto errors = Simulator::load(filePath);

    if (errors.empty()) {
        ui->messagesList->addItem("Configuração carregada com sucesso!");
        ui->startSimulationButton->setDisabled(false);
        return;
    }

    for (auto error : errors) {
        ui->messagesList->addItem(error);
    }
}

