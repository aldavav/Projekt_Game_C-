#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <Game/Camera/Camera.h>
#include <UI/HUD/TacticalHUD.h>
#include <QElapsedTimer>
#include <QObject>
#include <QPointF>
#include <Game/Entities/Entity.h>
#include <vector>
#include <memory>

class TacticalHUD;

class GameManager : public QObject
{
    Q_OBJECT

public:
    // Režimy umisťování
    enum class PlacementMode { None, Tank, Soldier, Plane };

    static GameManager &getInstance();

    // Správa entit
    void addEntity(std::unique_ptr<Entity> entity) {
        m_entities.push_back(std::move(entity));
    }
    const std::vector<std::unique_ptr<Entity>>& getEntities() const {
        return m_entities;
    }
    Entity* getEntityAt(const QPoint& hexPos);

    // Správa výběru
    Entity* getSelectedUnit() const { return m_selectedUnit; }
    void setSelectedUnit(Entity* unit) { m_selectedUnit = unit; }

    // Metody pro stavbu (Placement Mode)
    void startPlacement(PlacementMode mode) { m_currentPlacement = mode; }
    bool isPlacing() const { return m_currentPlacement != PlacementMode::None; }
    void cancelPlacement() { m_currentPlacement = PlacementMode::None; }
    PlacementMode getPlacementMode() const { return m_currentPlacement; }

    // Ekonomika
    int getGold() const { return m_gold; }
    void spendGold(int amount) { m_gold -= amount; }
    void addGold(int amount) { m_gold += amount; }

    // Herní smyčka a stav
    void update();
    float getGameTime() const;
    bool isPaused() const;
    Engine::GameSpeed getSpeed() const;
    TacticalHUD *getHUD() const;
    void setSpeed(Engine::GameSpeed speed);
    void togglePause();
    void setPaused(bool paused);

    // Interakce a UI
    void handleMouseClick(QPoint screenPos, bool m_is3D = false);
    bool hasSelection() const;
    QPointF getSelectedHex() const;
    void switchIsDiscoveryActive() { m_isDiscoveryActive = !m_isDiscoveryActive; }
    bool getIsDiscoveryActive() const { return m_isDiscoveryActive; }

public slots:
    void handleHudButton(int index);
    void handleMinimapNavigation(QPointF worldPos);

private:
    explicit GameManager(QObject *parent = nullptr);

    // Čas a simulace
    float m_gameTime = 0.0f;
    float m_timeScale = 1.0f;
    bool m_isPaused = false;
    Engine::GameSpeed m_currentSpeed = Engine::GameSpeed::Normal;

    // UI a výběr
    QPointF m_selectedHex;
    bool m_hasSelection = false;
    TacticalHUD *m_hud = nullptr;
    bool m_isDiscoveryActive = false;

    // Herní data
    std::vector<std::unique_ptr<Entity>> m_entities;
    Entity* m_selectedUnit = nullptr;
    int m_gold = 500;

    // Aktuální stav umisťování
    PlacementMode m_currentPlacement = PlacementMode::None;
};

#endif
