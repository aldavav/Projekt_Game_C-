#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->buttonQuit, &QPushButton::clicked, this, &MainWindow::on_actionQuit_triggered);
    connect(ui->buttonNewGame, &QPushButton::clicked, this, &MainWindow::on_buttonStartGame_clicked);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}




void MainWindow::on_buttonStartGame_clicked()
{
    int index = ui->stackedWidget->indexOf(ui->gameMap);
    if (index != -1) {
        ui->stackedWidget->setCurrentIndex(index);
         this->showFullScreen();
    }
}


void MainWindow::on_actionMenu_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->menu);
     this->showNormal();
}

