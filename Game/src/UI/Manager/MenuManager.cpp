#include <UI/Screens/GameOverScreen.h>
#include <UI/Screens/MenuScreen.h>
#include "MenuManager.h"

MenuManager &MenuManager::getInstance()
{
    static MenuManager instance;
    return instance;
}

MenuManager::MenuManager(QObject *parent)
    : QObject(parent),
      m_mainWindow(nullptr)
{
    connect(&GameEngine::getInstance(), &GameEngine::gameStateChanged,
            this, &MenuManager::handleGameStateChange);
}

void MenuManager::setMainWindow(QWidget *window)
{
    m_mainWindow = window;

    if (!m_mainWindow->layout())
    {
        auto *stacked = new QStackedLayout(m_mainWindow);
        stacked->setStackingMode(QStackedLayout::StackOne);
        m_mainWindow->setLayout(stacked);
    }
}

void MenuManager::pushScreen(AbstractScreen *screen)
{
    if (!m_mainWindow || !screen)
        return;

    if (!m_screenStack.isEmpty())
    {
        m_screenStack.top()->onExit();
    }

    auto *layout = qobject_cast<QStackedLayout *>(m_mainWindow->layout());
    layout->addWidget(screen);
    layout->setCurrentWidget(screen);

    m_screenStack.push(screen);
    screen->onEnter();
}

void MenuManager::popScreen()
{
    if (m_screenStack.isEmpty())
        return;

    AbstractScreen *oldScreen = m_screenStack.pop();
    oldScreen->onExit();

    auto *layout = qobject_cast<QStackedLayout *>(m_mainWindow->layout());
    layout->removeWidget(oldScreen);
    oldScreen->deleteLater();

    if (!m_screenStack.isEmpty())
    {
        AbstractScreen *nextScreen = m_screenStack.top();
        layout->setCurrentWidget(nextScreen);
        nextScreen->onEnter();
    }
}

void MenuManager::setScreen(AbstractScreen *screen)
{
    while (!m_screenStack.isEmpty())
    {
        AbstractScreen *oldScreen = m_screenStack.pop();

        if (m_mainWindow && m_mainWindow->layout())
        {
            m_mainWindow->layout()->removeWidget(oldScreen);
        }

        oldScreen->hide();
        oldScreen->deleteLater();
    }

    pushScreen(screen);
}

void MenuManager::updateScreenVisibility()
{
    for (int i = 0; i < m_screenStack.size(); ++i)
    {
        m_screenStack[i]->setVisible(i == m_screenStack.size() - 1);
    }
}

void MenuManager::handleGameStateChange(int newState)
{
    switch (newState)
    {
    case STATE_MENU:
        setScreen(new MenuScreen(m_mainWindow));
        break;

    case STATE_RUNNING:
        break;

    case STATE_GAMEOVER:
    {
        bool winStatus = GameEngine::getInstance().didPlayerWin();
        setScreen(new GameOverScreen(winStatus, m_mainWindow));
        break;
    }
    }
}
