#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <UI/Screens/MainMenuScreen.h>
#include <Core/Input/InputManager.h>
#include <UI/Manager/MenuManager.h>
#include <Game/Engine/GameEngine.h>
#include <QFontDatabase>
#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QIcon>

class QKeyEvent;
class QMouseEvent;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_actionQuit_triggered();

private:
    QWidget *m_centralWidget;
};

#endif
