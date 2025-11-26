#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Command & Conquer"));
    QIcon icon(":/images/assets/icon0.png");
    setWindowIcon(icon);

    // Connecting menu buttons to their specific slots
    connect(ui->buttonNewGame, &QPushButton::clicked, this, &MainWindow::on_buttonStartGame_clicked);
    connect(ui->buttonLoadGame, &QPushButton::clicked, this, &MainWindow::on_buttonLoadGame_clicked);
    connect(ui->buttonSettings, &QPushButton::clicked, this, &MainWindow::on_buttonSettings_clicked);
    connect(ui->buttonHelp, &QPushButton::clicked, this, &MainWindow::on_buttonHelp_clicked);
    connect(ui->buttonCredits, &QPushButton::clicked, this, &MainWindow::on_buttonCredits_clicked);
    connect(ui->buttonQuit, &QPushButton::clicked, this, &MainWindow::on_actionQuit_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- Menu Slot Implementations ---

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_buttonStartGame_clicked()
{
    qDebug() << "Start Game button clicked. Starting new game...";
    // Example: Switch to the game map view in the stacked widget
}

void MainWindow::on_buttonLoadGame_clicked()
{
    qDebug() << "Load Game button clicked. Loading screen should appear.";
    // TODO: Implement loading logic or show a load game dialog/page.
}

void MainWindow::on_buttonSettings_clicked()
{
    qDebug() << "Settings button clicked. Opening settings screen.";
    // Example: Navigate to a settings page in the stacked widget
    // ui->stackedWidget->setCurrentWidget(ui->settingsPage);
}

void MainWindow::on_buttonHelp_clicked()
{
    qDebug() << "Help button clicked. Opening help documentation.";
    // TODO: Implement showing a help screen or pop-up.
}

void MainWindow::on_buttonCredits_clicked()
{
    qDebug() << "Credits button clicked. Opening credits sequence.";
    // TODO: Implement showing the credits page.
}

// This function now handles returning to the menu screen.
void MainWindow::on_actionMenu_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->menu);
    this->showNormal();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        qDebug() << "ESC key pressed. Quitting application.";
        QApplication::quit();
    }
    else if (event->key() == Qt::Key_Up)
    {
        qDebug() << "Up Arrow pressed. Moving North.";
    }
    else if (event->key() == Qt::Key_Space)
    {
        qDebug() << "Space bar pressed. Firing!";
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        qDebug() << "Enter key pressed. Confirming action.";
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
