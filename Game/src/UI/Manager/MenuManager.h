#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <UI/Screens/GameOverScreen.h>
#include <Game/Engine/GameEngine.h>
#include <UI/Screens/MenuScreen.h>
#include <Main/MainWindow.h>
#include <qstackedlayout.h>
#include <QLayout>
#include <QObject>
#include <QString>
#include <QWidget>
#include <QStack>
#include <QDebug>

class AbstractScreen;
class GameEngine;
class QWidget;

class MenuManager : public QObject
{
    Q_OBJECT

private:
    explicit MenuManager(QObject *parent = nullptr);

    QStack<AbstractScreen *> m_screenStack;

    QWidget *m_mainWindow;

    void updateScreenVisibility();

public:
    static MenuManager &getInstance();

    MenuManager(const MenuManager &) = delete;

    void operator=(const MenuManager &) = delete;

    MenuManager(MenuManager &&) = delete;

    MenuManager &operator=(MenuManager &&) = delete;

    void setMainWindow(QWidget *window);

    void pushScreen(AbstractScreen *screen);

    void popScreen();

    void setScreen(AbstractScreen *screen);

    void updateMetadata();

public slots:
    void handleGameStateChange(int newState);
};

#endif
