#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QKeyEvent>
#include <QPushButton>
#include <QDebug>
#include <QIcon>
#include <QTimer>
#include <QShowEvent>

void MainWindow::setupButton(QPushButton *button, bool disabled = false)
{
    if (button)
    {
        button->setCursor(Qt::PointingHandCursor);
        if (disabled)
        {
            button->setDisabled(true);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Command & Conquer"));
    QIcon icon(":/images/assets/icon0.png");
    setWindowIcon(icon);

    setupButton(ui->buttonNewGame);
    connect(ui->buttonNewGame, &QPushButton::clicked, this, &MainWindow::on_buttonStartGame_clicked);

    setupButton(ui->buttonLoadGame, true);
    connect(ui->buttonLoadGame, &QPushButton::clicked, this, &MainWindow::on_buttonLoadGame_clicked);

    setupButton(ui->buttonSettings, true);
    connect(ui->buttonSettings, &QPushButton::clicked, this, &MainWindow::on_buttonSettings_clicked);

    setupButton(ui->buttonHelp, true);
    connect(ui->buttonHelp, &QPushButton::clicked, this, &MainWindow::on_buttonHelp_clicked);

    setupButton(ui->buttonCredits, true);
    connect(ui->buttonCredits, &QPushButton::clicked, this, &MainWindow::on_buttonCredits_clicked);

    setupButton(ui->buttonQuit);
    connect(ui->buttonQuit, &QPushButton::clicked, this, &MainWindow::on_actionQuit_triggered);
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
    qDebug() << "Start Game button clicked. Starting new game...";
}

void MainWindow::on_buttonLoadGame_clicked()
{
    qDebug() << "Load Game button clicked. Loading screen should appear.";
}

void MainWindow::on_buttonSettings_clicked()
{
    qDebug() << "Settings button clicked. Opening settings screen.";
}

void MainWindow::on_buttonHelp_clicked()
{
    qDebug() << "Help button clicked. Opening help documentation.";
}

void MainWindow::on_buttonCredits_clicked()
{
    qDebug() << "Credits button clicked. Opening credits sequence.";
}

void MainWindow::on_actionMenu_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->menu);
    this->showNormal();
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    QTimer::singleShot(50, this, [this]()
                       {
        if (ui->buttonNewGame) {
            ui->buttonNewGame->setFocus();
            qDebug() << "Initial focus set to New Game button.";
        } });
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
        return;
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        QWidget *focused_widget = QApplication::focusWidget();
        QPushButton *focused_button = qobject_cast<QPushButton *>(focused_widget);

        if (focused_button && focused_button->isEnabled())
        {
            qDebug() << "Enter pressed on button:" << focused_button->text();
            focused_button->click();
            return;
        }
    }

    QMainWindow::keyPressEvent(event);
}
