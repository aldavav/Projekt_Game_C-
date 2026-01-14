#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <Game/Engine/GameEngine.h>
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

class AbstractScreen : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractScreen(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual ~AbstractScreen() = default;

    virtual void onEnter() = 0;

    virtual void onExit() = 0;
};

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

public slots:
    void handleGameStateChange(int newState);
};

#endif
