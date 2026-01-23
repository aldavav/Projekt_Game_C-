#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <Game/Camera/Camera.h>
#include <UI/HUD/TacticalHUD.h>
#include <QElapsedTimer>
#include <QObject>
#include <QPointF>

class GameManager : public QObject
{
    Q_OBJECT

public:
    static GameManager &getInstance();

    void update(float deltaTime);

    float getGameTime() const;

    bool isPaused() const;

    Engine::GameSpeed getSpeed() const;

    TacticalHUD *getHUD() const;

    void setSpeed(Engine::GameSpeed speed);

    void togglePause();

    void setPaused(bool paused);

    void handleMouseClick(QPoint screenPos);

    bool hasSelection() const;

    QPointF getSelectedHex() const;

public slots:
    void handleHudButton(int index);

    void handleMinimapNavigation(QPointF worldPos);

private:
    GameManager(QObject *parent = nullptr);

    float m_gameTime = 0.0f;

    float m_timeScale = 1.0f;

    bool m_isPaused = false;

    Engine::GameSpeed m_currentSpeed = Engine::GameSpeed::NORMAL;

    QPointF m_selectedHex;

    bool m_hasSelection = false;

    TacticalHUD *m_hud;
};

#endif
