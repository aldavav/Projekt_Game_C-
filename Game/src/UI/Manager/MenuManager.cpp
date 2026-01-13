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

MenuManager::~MenuManager()
{
}

void MenuManager::setMainWindow(QWidget *window)
{
    if (m_mainWindow)
    {
    }

    m_mainWindow = window;

    if (!m_mainWindow->layout())
    {
        m_mainWindow->setLayout(new QVBoxLayout(m_mainWindow));
    }

}

void MenuManager::pushScreen(AbstractScreen *screen)
{
    if (!m_mainWindow)
    {
        delete screen;
        return;
    }

    if (!m_screenStack.isEmpty())
    {
        m_screenStack.top()->onExit();
    }

    m_mainWindow->layout()->addWidget(screen);
    m_screenStack.push(screen);

    updateScreenVisibility();
    screen->onEnter();

}

void MenuManager::popScreen()
{
    if (m_screenStack.isEmpty())
    {
        return;
    }

    AbstractScreen *oldScreen = m_screenStack.pop();
    oldScreen->onExit();

    m_mainWindow->layout()->removeWidget(oldScreen);
    delete oldScreen;

    updateScreenVisibility();

    if (!m_screenStack.isEmpty())
    {
        m_screenStack.top()->onEnter();
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
    const int GAME_MENU = 0;
    const int GAME_RUNNING = 2;
    const int GAME_PAUSED = 3;
    const int GAME_OVER = 4;


    switch (newState)
    {
    case GAME_MENU:
        break;
    case GAME_RUNNING:
        if (!m_screenStack.isEmpty())
        {
            popScreen();
        }
        break;
    case GAME_PAUSED:
        break;
    case GAME_OVER:
        break;
    default:
        break;
    }
}
