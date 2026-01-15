#include "GameManager.h"

GameManager &GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

void GameManager::update(float deltaTime)
{
    if (!m_isPaused)
    {
        m_gameTime += (deltaTime * m_timeScale);
    }

    auto &cam = Camera::getInstance();
    cam.update(deltaTime);

    QPointF worldPos = cam.getCurrentPos();

    float q = (2.0f / 3.0f * worldPos.x()) / 32.0f;
    float r = (-1.0f / 3.0f * worldPos.x() + std::sqrt(3.0f) / 3.0f * worldPos.y()) / 32.0f;

    Map::getInstance().revealRadius(std::round(q), std::round(r), 6);

    m_hud->update(m_gameTime, m_isPaused, m_currentSpeed);
}

float GameManager::getGameTime() const
{
    return m_gameTime;
}

bool GameManager::isPaused() const
{
    return m_isPaused;
}

GameSpeed GameManager::getSpeed() const
{
    return m_currentSpeed;
}

TacticalHUD *GameManager::getHUD() const
{
    return m_hud;
}

void GameManager::setSpeed(GameSpeed speed)
{
    m_currentSpeed = speed;
    switch (m_currentSpeed)
    {
    case GameSpeed::SLOW:
        m_timeScale = 0.5f;
        break;
    case GameSpeed::NORMAL:
        m_timeScale = 1.0f;
        break;
    case GameSpeed::FAST:
        m_timeScale = 2.0f;
        break;
    }
}

void GameManager::togglePause()
{
    m_isPaused = !m_isPaused;
}

void GameManager::setPaused(bool paused)
{
    m_isPaused = paused;
}

void GameManager::handleMouseClick(QPointF worldPos)
{
    const float size = 32.0f;
    float q = (std::sqrt(3.0f) / 3.0f * worldPos.x() - 1.0f / 3.0f * worldPos.y()) / size;
    float r = (2.0f / 3.0f * worldPos.y()) / size;

    QPoint selection = Camera::getInstance().hexRound(q, r).toPoint();

    m_selectedHex = QPointF(selection.x(), selection.y());
    m_hasSelection = true;

    m_hud->setSelection(m_selectedHex, true);
}

bool GameManager::hasSelection() const
{
    return m_hasSelection;
}

QPointF GameManager::getSelectedHex() const
{
    return m_selectedHex;
}

void GameManager::handleHudButton(int index)
{
    if (index == 0)
    {
        togglePause();
    }
    else
    {
        m_isPaused = false;
        if (index == 1)
            setSpeed(GameSpeed::SLOW);
        else if (index == 2)
            setSpeed(GameSpeed::NORMAL);
        else if (index == 3)
            setSpeed(GameSpeed::FAST);
    }
}

void GameManager::handleMinimapNavigation(QPointF worldPos)
{
    Camera::getInstance().setTargetPos(worldPos);
}

GameManager::GameManager(QObject *parent)
    : QObject(parent),
      m_gameTime(0.0f),
      m_timeScale(1.0f),
      m_isPaused(false),
      m_currentSpeed(GameSpeed::NORMAL)
{
    m_hud = new TacticalHUD(this);

    connect(m_hud, &TacticalHUD::hudButtonClicked, this, &GameManager::handleHudButton);
    connect(m_hud, &TacticalHUD::minimapClicked, this, &GameManager::handleMinimapNavigation);
}
