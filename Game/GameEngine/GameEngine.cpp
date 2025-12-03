#include "GameEngine.h"
#include <QDateTime>

GameEngine &GameEngine::getInstance()
{
    static GameEngine instance;
    return instance;
}

GameEngine::GameEngine(QObject *parent)
    : QObject(parent),
      m_isRunning(false),
      m_currentState(GAME_MENU)
{
    connect(&m_gameTimer, &QTimer::timeout, this, &GameEngine::gameLoopTick);
    m_gameTimer.setInterval(16);
    m_lastUpdateTime = 0;
    initializeSystems();
}

GameEngine::~GameEngine()
{
}

void GameEngine::startGame()
{
    if (m_isRunning)
    {
        qDebug() << "Game is already running.";
        return;
    }
    qDebug() << "Starting Game Engine...";
    m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    m_isRunning = true;
    m_currentState = GAME_RUNNING;
    emit gameStateChanged(m_currentState);
    m_gameTimer.start();
}

void GameEngine::stopGame()
{
    if (!m_isRunning)
    {
        qDebug() << "Game is already stopped.";
        return;
    }
    qDebug() << "Stopping Game Engine.";
    m_gameTimer.stop();
    m_isRunning = false;
    m_currentState = GAME_OVER;
    emit gameStateChanged(m_currentState);
}

void GameEngine::gameLoopTick()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    float deltaTime = (currentTime - m_lastUpdateTime) / 1000.0f;
    m_lastUpdateTime = currentTime;
    emit gameLoopUpdate(deltaTime);
}

void GameEngine::initializeSystems()
{
}

void GameEngine::updateSystems(float deltaTime)
{
}

void GameEngine::handlePlayerInput(const QString &inputCommand)
{
    qDebug() << "Engine received command:" << inputCommand;
}
