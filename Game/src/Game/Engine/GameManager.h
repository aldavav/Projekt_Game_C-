#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <Game/Camera/Camera.h>
#include <UI/HUD/TacticalHUD.h>
#include <QElapsedTimer>
#include <QObject>
#include <QPointF>

class TacticalHUD;
class QPoint;

class GameManager : public QObject
{
    Q_OBJECT

public:
    static GameManager &getInstance();

    void update();

    float getGameTime() const { return m_gameTime; }

    bool isPaused() const { return m_isPaused; }

    Engine::GameSpeed getSpeed() const { return m_currentSpeed; }

    TacticalHUD *getHUD() const { return m_hud; }

    bool hasSelection() const { return m_hasSelection; }

    QPointF getSelectedHex() const { return m_selectedHex; }

    bool getIsDiscoveryActive() const { return m_isDiscoveryActive; }

    void setSpeed(Engine::GameSpeed speed);

    void togglePause();

    void setPaused(bool paused);

    void switchIsDiscoveryActive() { m_isDiscoveryActive = !m_isDiscoveryActive; }

    void handleMouseClick(QPoint screenPos, bool is3D = false);

    GameManager(const GameManager &) = delete;

    GameManager &operator=(const GameManager &) = delete;

    GameManager(GameManager &&other) noexcept = delete;

    GameManager &operator=(GameManager &&other) noexcept = delete;

public slots:
    void handleHudButton(int index);

    void handleMinimapNavigation(QPointF worldPos);

private:
    explicit GameManager(QObject *parent = nullptr);

    float m_gameTime = 0.0f;

    float m_timeScale = 1.0f;

    bool m_isPaused = false;

    bool m_isDiscoveryActive = false;

    Engine::GameSpeed m_currentSpeed = Engine::GameSpeed::Normal;

    QPointF m_selectedHex;

    bool m_hasSelection = false;

    TacticalHUD *m_hud = nullptr;
};

#endif
