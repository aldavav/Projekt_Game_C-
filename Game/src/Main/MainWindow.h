#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <UI/Screens/MainMenuScreen.h>
#include <Core/Input/InputManager.h>
#include <UI/Manager/MenuManager.h>
#include <Game/Engine/GameEngine.h>
#include <QGuiApplication>
#include <QFontDatabase>
#include <QApplication>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QShowEvent>
#include <QKeyEvent>
#include <QScreen>
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

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_actionQuit_triggered();

private:
    QWidget *m_centralWidget;
};

#endif
