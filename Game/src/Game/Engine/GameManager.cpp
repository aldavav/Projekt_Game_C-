#include "GameManager.h"

GameManager &GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

void GameManager::update()
{
    if (!m_isPaused)
    {
        m_gameTime += (Config::Simulation::DELTA_TIME * m_timeScale);
    }

    auto &cam = Camera::getInstance();
    cam.update(m_gameTime);

    QPointF worldPos = cam.getCurrentPos();
    const float size = Config::World::BASE_TILE_SIZE;

    int q = std::round((2.0f / 3.0f * worldPos.x()) / size);
    int r = std::round((-1.0f / 3.0f * worldPos.x() + std::sqrt(3.0f) / 3.0f * worldPos.y()) / size);

    Map::getInstance().revealRadiusWithCleanup(q, r, Config::World::REVEAL_RADIUS);

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

Engine::GameSpeed GameManager::getSpeed() const
{
    return m_currentSpeed;
}

TacticalHUD *GameManager::getHUD() const
{
    return m_hud;
}

void GameManager::setSpeed(Engine::GameSpeed speed)
{
    m_currentSpeed = speed;
    switch (m_currentSpeed)
    {
    case Engine::GameSpeed::SLOW:
        m_timeScale = Config::Simulation::SPEED_SLOW;
        break;
    case Engine::GameSpeed::NORMAL:
        m_timeScale = Config::Simulation::SPEED_NORMAL;
        break;
    case Engine::GameSpeed::FAST:
        m_timeScale = Config::Simulation::SPEED_FAST;
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

void GameManager::handleMouseClick(QPoint screenPos)
{
    auto &cam = Camera::getInstance();
    QPointF worldPos = cam.screenToWorld(screenPos);

    const float size = Config::World::BASE_TILE_SIZE;
    float q = (2.0f / 3.0f * worldPos.x()) / size;
    float r = (-1.0f / 3.0f * worldPos.x() + std::sqrt(3.0f) / 3.0f * worldPos.y()) / size;

    QPoint selection = cam.hexRound(q, r).toPoint();
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
            setSpeed(Engine::GameSpeed::SLOW);
        else if (index == 2)
            setSpeed(Engine::GameSpeed::NORMAL);
        else if (index == 3)
            setSpeed(Engine::GameSpeed::FAST);
    }
}

void GameManager::handleMinimapNavigation(QPointF worldPos)
{
    Map::getInstance().clearAllDiscovered();
    Camera::getInstance().setTargetPos(worldPos);
}

GameManager::GameManager(QObject *parent)
    : QObject(parent),
      m_gameTime(0.0f),
      m_timeScale(1.0f),
      m_isPaused(false),
      m_currentSpeed(Engine::GameSpeed::NORMAL)
{
    m_hud = new TacticalHUD(this);

    connect(m_hud, &TacticalHUD::hudButtonClicked, this, &GameManager::handleHudButton);
    connect(m_hud, &TacticalHUD::minimapClicked, this, &GameManager::handleMinimapNavigation);
}
