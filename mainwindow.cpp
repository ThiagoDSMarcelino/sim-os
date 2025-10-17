#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    ui->startSimulationButton->setDisabled(true);

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
        this->simulator = Simulator::getInstance();
        return;
    }

    for (auto error : errors) {
        ui->messagesList->addItem(error);
    }
}

void MainWindow::on_startSimulationButton_clicked()
{
    if (!this->simulator) return;

    ui->stackedWidget->setCurrentIndex(1);

    updateGanttChart();
}


void MainWindow::on_nextQuantumButton_clicked()
{
    if (!this->simulator) return;

    this->simulator->runQuantum();

    updateGanttChart();
}

void MainWindow::updateGanttChart()
{
    if (!this->simulator) return;

    QGraphicsScene* scene = ui->graphicsView->scene();
    scene->clear();

    const int BOX_SIZE = 30;
    const int TASK_SPACING = 40;
    const int TIME_HEADER = 20;
    const QColor RUNNING_COLOR = Qt::cyan;

    int tasksSize = 5;
    int currentTime = 20;

    int taskYPosition = TIME_HEADER;

    for (int i = 0; i < tasksSize; i++)
    {
        QGraphicsTextItem* taskName = scene->addText("t1");
        taskName->setPos(-50, taskYPosition + 5);
        taskYPosition += TASK_SPACING;
    }

    for (int time = 0; time < currentTime; ++time) {
        taskYPosition = TIME_HEADER;
        int runningTaskID = time % tasksSize;

        for (int i = 0; i < tasksSize; i++) {
            int taskID = i;

            QPen boxPen(Qt::gray);
            QBrush boxBrush;

            if (taskID == runningTaskID) {
                boxPen.setColor(Qt::black);
                boxBrush.setColor(RUNNING_COLOR);
                boxBrush.setStyle(Qt::SolidPattern);
            } else {
                boxBrush.setStyle(Qt::NoBrush);
            }

            scene->addRect(time * BOX_SIZE, taskYPosition, BOX_SIZE, BOX_SIZE, boxPen, boxBrush);
            taskYPosition += TASK_SPACING;
        }
    }

    for (int time = 0; time < currentTime; time++) {
        QGraphicsTextItem* timeText = scene->addText(QString::number(time));
        timeText->setPos(time * BOX_SIZE, 0);
    }

    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}
