#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <UI/Screens/GameOverScreen.h>
#include <Game/Engine/GameEngine.h>
#include <UI/Screens/MenuScreen.h>
#include <Core/Common/GameTypes.h>
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

public:
    static MenuManager &getInstance();

    void setMainWindow(QWidget *window);
    
    void setScreen(AbstractScreen *screen);

    void pushScreen(AbstractScreen *screen);

    void popScreen();

    void updateMetadata();

public slots:
    void handleGameStateChange(int newState);

private:
    explicit MenuManager(QObject *parent = nullptr);

    void updateScreenVisibility();

    QStack<AbstractScreen *> m_screenStack;

    QWidget *m_mainWindow;
};

#endif
