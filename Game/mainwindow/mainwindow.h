#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../GameEngine/GameEngine.h"
#include "../MenuManager/MenuManager.h"
#include "../InputManager/InputManager.h"
#include <QApplication>
#include <QMainWindow>
#include <QMouseEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QShowEvent>
#include <QKeyEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;
};

#endif
