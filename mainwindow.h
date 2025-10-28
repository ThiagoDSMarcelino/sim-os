#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectFileButton_clicked();

    void on_startSimulationButton_clicked();

    void on_nextQuantumButton_clicked();

    void on_fullSimulationButton_clicked();

private:
    Ui::MainWindow *ui;

    Simulator *simulator;

    void updateGanttChart();
};
#endif // MAINWINDOW_H
