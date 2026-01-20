#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <Core/Input/InputManager.h>
#include <Core/Config/GameConfig.h>
#include <Core/Config/GameState.h>
#include <Game/Entities/Entity.h>
#include <Core/Config/Config.h>
#include <Game/Camera/Camera.h>
#include <qcoreapplication.h>
#include <Game/Map/Map.h>
#include <QSettings>
#include <QObject>
#include <QString>
#include <QTimer>
#include <chrono>
#include <memory>
#include <vector>
#include <QDir>

class Entity;

class GameEngine : public QObject
{
    Q_OBJECT

public:
    static GameEngine &getInstance();

    GameEngine(const GameEngine &) = delete;

    GameEngine &operator=(const GameEngine &) = delete;

    void startGame();

    void stopGame();

    void triggerEndGame(bool victory);

    void setupMatch(QString mapName, uint32_t seed);

    void saveCurrentMatch();

    void loadMatch(const QString &mapName);

    void setState(GameState::State newState);

    GameState::State getState() const { return m_currentState; }

    bool didPlayerWin() const { return m_playerWon; }

signals:
    void gameLoopUpdate(float deltaTime);

    void gameStateChanged(int newState);

public slots:
    void handlePlayerInput(const QString &inputCommand);

private slots:
    void gameLoopTick();

private:
    explicit GameEngine(QObject *parent = nullptr);

    virtual ~GameEngine() override = default;

    void initializeSystems();

    void updateSimulation(float fixedStep);

    void updateCameraMovement(float fixedStep);

    GameState::State m_currentState = GameState::STATE_MENU;

    bool m_isRunning = false;

    bool m_playerWon = false;

    QString m_currentMapName = GameConfig::World::DEFAULT_MAP_NAME;

    uint32_t m_currentSeed = GameConfig::World::DEFAULT_SEED;

    QTimer m_gameTimer;

    std::chrono::steady_clock::time_point m_lastTime;

    float m_accumulator = 0.0f;

    std::vector<std::unique_ptr<Entity>> m_entities;
};

#endif
