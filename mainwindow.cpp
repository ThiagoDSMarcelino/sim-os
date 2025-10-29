#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QGuiApplication>
#include <QMessageBox>
#include <QScreen>
#include "algorithm"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    ui->startSimulationButton->setDisabled(true);
    ui->fullSimulationButton->setDisabled(true);

    ui->graphicsView->setScene(new QGraphicsScene(this));

    this->resize(1024, 768);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;

    this->move(x, y);
}

MainWindow::~MainWindow()
{
    delete ui;
    Simulator::free();
}

void MainWindow::on_selectFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Abrir Arquivo"),
                                                    QDir::homePath(),
                                                    tr("*.txt"));

    ui->messagesList->clear();

    if (filePath.isEmpty()) {
        ui->messagesList->addItem("Nenhum arquivo selecionado.");
        return;
    }

    auto errors = Simulator::load(filePath);

    if (errors.empty()) {
        ui->messagesList->addItem("Configuração carregada com sucesso!");
        ui->startSimulationButton->setDisabled(false);
        ui->fullSimulationButton->setDisabled(false);
        this->simulator = Simulator::getInstance();
        return;
    }

    for (auto error : errors) {
        ui->messagesList->addItem(error);
    }
}

void MainWindow::on_startSimulationButton_clicked()
{
    if (!this->simulator) {
        return;
    }

    ui->stackedWidget->setCurrentIndex(1);

    this->simulator->start();

    updateGanttChart();
}

void MainWindow::on_nextQuantumButton_clicked()
{
    if (!this->simulator || this->simulator->hasFinished()) {
        return;
    }

    this->simulator->runQuantum();

    updateGanttChart();

    if (this->simulator->hasFinished()) {
        ui->nextQuantumButton->setDisabled(true);
    }
}

void MainWindow::on_fullSimulationButton_clicked()
{
    if (!this->simulator) {
        return;
    }

    ui->stackedWidget->setCurrentIndex(1);

    this->simulator->start();

    while (!this->simulator->hasFinished()) {
        this->simulator->runQuantum();
    }

    updateGanttChart();
}

void MainWindow::updateGanttChart()
{
    if (!this->simulator) return;

    QGraphicsScene *scene = this->ui->graphicsView->scene();
    scene->clear();

    const int BOX_SIZE = 30;
    const int TASK_SPACING = 40;
    const int TIME_HEADER = 20;

    auto tasks = this->simulator->getTasks();
    auto history = this->simulator->getHistory();

    int taskYPosition = TIME_HEADER;

    // Set tasks ids at the left of the graph
    for (int i = tasks.size() - 1; i >= 0; i--) {
        QGraphicsTextItem* taskName = scene->addText(tasks.at(i)->get_id());
        taskName->setPos(-50, taskYPosition + 5);
        taskYPosition += TASK_SPACING;
    }

    int time = 0;

    // Draw the tasks blocks
    for (auto history : this->simulator->getHistory()) {
        taskYPosition = TIME_HEADER;

        for (int i = tasks.size() - 1; i >= 0; i--) {
            QPen boxPen(Qt::gray);
            QBrush boxBrush;

            auto currentId = tasks.at(i)->get_id();
            auto activeTasks = history.getActiveTasks();
            auto it = std::find_if(activeTasks.begin(),
                                   activeTasks.end(),
                                   [currentId](TaskControlBlock *task) {
                                       return task->get_id() == currentId;
                                   });

            if (it == activeTasks.end()) {
                taskYPosition += TASK_SPACING;
                continue;
            }

            QString runningTaskID = history.getRunningTask() ? history.getRunningTask()->get_id()
                                                             : "";

            if (tasks.at(i)->get_id() == runningTaskID) {
                boxPen.setColor(Qt::black);
                boxBrush.setColor(tasks.at(i)->get_color());
                boxBrush.setStyle(Qt::SolidPattern);
            } else {
                boxBrush.setStyle(Qt::NoBrush);
            }

            scene->addRect(time * BOX_SIZE, taskYPosition, BOX_SIZE, BOX_SIZE, boxPen, boxBrush);
            taskYPosition += TASK_SPACING;
        }

        QGraphicsTextItem *timeText = scene->addText(QString::number(history.getInstant()));
        timeText->setPos(time * BOX_SIZE, 0);
        time++;
    }

    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_saveButton_clicked()
{
    QString filePath
        = QFileDialog::getSaveFileName(this,
                                       tr("Salvar Gráfico como Imagem"),
                                       QDir::homePath(),
                                       tr("Imagens PNG (*.png);;Imagens JPEG (*.jpg *.jpeg)"));

    if (filePath.isEmpty()) {
        return;
    }

    QPixmap pixmap = this->ui->graphicsView->grab();

    if (!pixmap.save(filePath)) {
        QMessageBox::warning(this,
                             tr("Erro ao Salvar"),
                             tr("Não foi possível salvar a imagem no local especificado."));
    }
}
