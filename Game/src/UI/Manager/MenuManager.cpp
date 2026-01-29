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
        AbstractScreen *previousScreen = m_screenStack.top();
        AbstractScreen *newFreshScreen = nullptr;

        if (qobject_cast<MenuScreen *>(previousScreen))
        {
            newFreshScreen = new MenuScreen(m_mainWindow);
        }

        if (newFreshScreen)
        {
            m_screenStack.pop();
            m_screenStack.push(newFreshScreen);

            layout->removeWidget(previousScreen);
            previousScreen->deleteLater();

            layout->addWidget(newFreshScreen);
            layout->setCurrentWidget(newFreshScreen);
            newFreshScreen->onEnter();
        }
        else
        {
            layout->setCurrentWidget(previousScreen);
            previousScreen->onEnter();
        }
    }
}

void MenuManager::updateMetadata()
{
    if (m_mainWindow)
    {
        auto *dw = qobject_cast<MainWindow *>(m_mainWindow->window());
        if (dw)
            dw->updateWindowMetadata();
    }
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
    AbstractScreen *nextScreen = nullptr;

    if (newState == static_cast<int>(Engine::GameState::Menu))
    {
        nextScreen = new MenuScreen(m_mainWindow);
    }
    else if (newState == static_cast<int>(Engine::GameState::GameOver))
    {
        bool winStatus = GameEngine::getInstance().didPlayerWin();
        nextScreen = new GameOverScreen(winStatus, m_mainWindow);
    }

    if (nextScreen)
    {
        setScreen(nextScreen);
    }
}
