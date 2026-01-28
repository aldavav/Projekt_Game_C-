#include "GameManager.h"
#include <Game/Map/Map.h>
#include <cmath>

GameManager &GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

GameManager::GameManager(QObject *parent)
    : QObject(parent),
    m_gameTime(0.0f),
    m_timeScale(1.0f),
    m_isPaused(false),
    m_currentSpeed(Engine::GameSpeed::Normal)
{
    m_hud = new TacticalHUD(this);
    connect(m_hud, &TacticalHUD::hudButtonClicked, this, &GameManager::handleHudButton);
    connect(m_hud, &TacticalHUD::minimapClicked, this, &GameManager::handleMinimapNavigation);
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

    // Přepočet kamery na hex pro odhalování mapy
    int q = std::round((2.0f / 3.0f * worldPos.x()) / size);
    int r = std::round((-1.0f / 3.0f * worldPos.x() + std::sqrt(3.0f) / 3.0f * worldPos.y()) / size);

    Map::getInstance().revealRadiusWithCleanup(q, r, Config::World::REVEAL_RADIUS);

    if (m_hud) {
        m_hud->update(m_gameTime, m_isPaused, m_currentSpeed);
    }

    float dt = 1.0f / 60.0f; // Nebo skutečný čas

    // 1. Update všech
    for (auto& e : m_entities) {
        e->update(dt);
    }

    // 2. Odstranění těch, co mají health <= 0
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
                                    [](const std::unique_ptr<Entity>& e) {
                                        return !e->isAlive();
                                    }), m_entities.end());
}

Entity* GameManager::getEntityAt(const QPoint& hexPos) {
    auto &cam = Camera::getInstance();
    for (auto& entity : m_entities) {
        if (!entity) continue;

        QPointF pos = entity->getPosition();
        const float size = Config::World::BASE_TILE_SIZE;

        // Přepočet world pozice entity na axiální souřadnice q, r
        float q = (2.0f / 3.0f * pos.x()) / size;
        float r = (-1.0f / 3.0f * pos.x() + std::sqrt(3.0f) / 3.0f * pos.y()) / size;

        // Zaokrouhlení na nejbližší hex
        QPointF rounded = cam.hexRound(q, r);
        QPoint entityHex(static_cast<int>(rounded.x()), static_cast<int>(rounded.y()));

        if (entityHex == hexPos) {
            return entity.get();
        }
    }
    return nullptr;
}

// --- Gettery a Settery ---

float GameManager::getGameTime() const { return m_gameTime; }
bool GameManager::isPaused() const { return m_isPaused; }
Engine::GameSpeed GameManager::getSpeed() const { return m_currentSpeed; }
TacticalHUD *GameManager::getHUD() const { return m_hud; }

void GameManager::setPaused(bool paused) { m_isPaused = paused; }
void GameManager::togglePause() { m_isPaused = !m_isPaused; }

void GameManager::setSpeed(Engine::GameSpeed speed)
{
    m_currentSpeed = speed;
    switch (m_currentSpeed)
    {
    case Engine::GameSpeed::Slow:   m_timeScale = Config::Simulation::SPEED_SLOW; break;
    case Engine::GameSpeed::Normal: m_timeScale = Config::Simulation::SPEED_NORMAL; break;
    case Engine::GameSpeed::Fast:   m_timeScale = Config::Simulation::SPEED_FAST; break;
    }
}

void GameManager::handleMouseClick(QPoint screenPos, bool m_is3D)
{
    auto &cam = Camera::getInstance();
    QPoint currentHover = cam.screenToHex(screenPos, m_is3D);

    m_selectedHex = QPointF(currentHover.x(), currentHover.y());
    m_hasSelection = true;
    if (m_hud) m_hud->setSelection(m_selectedHex, true);
}

bool GameManager::hasSelection() const { return m_hasSelection; }
QPointF GameManager::getSelectedHex() const { return m_selectedHex; }

void GameManager::handleHudButton(int index)
{
    if (index == 0) togglePause();
    else {
        m_isPaused = false;
        if (index == 1) setSpeed(Engine::GameSpeed::Slow);
        else if (index == 2) setSpeed(Engine::GameSpeed::Normal);
        else if (index == 3) setSpeed(Engine::GameSpeed::Fast);
    }
}

void GameManager::handleMinimapNavigation(QPointF worldPos)
{
    Map::getInstance().clearAllDiscovered();
    Camera::getInstance().setTargetPos(worldPos);
}
