#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Command & Conquer"));
    QIcon icon(":/images/icon1.png");
    setWindowIcon(icon);

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
    if (index != -1)
    {
        ui->stackedWidget->setCurrentIndex(index);
        this->showFullScreen();
    }
}

void MainWindow::on_actionMenu_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->menu);
    this->showNormal();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Check if a specific key was pressed
    if (event->key() == Qt::Key_Escape)
    {
        qDebug() << "ESC key pressed. Quitting application.";
        QApplication::quit();
    }
    // Check for arrow keys
    else if (event->key() == Qt::Key_Up)
    {
        qDebug() << "Up Arrow pressed. Moving North.";
    }
    // Check for another key
    else if (event->key() == Qt::Key_Space)
    {
        qDebug() << "Space bar pressed. Firing!";
    }
    else
    {
        // For any other key, call the base class implementation
        QMainWindow::keyPressEvent(event);
    }
}
