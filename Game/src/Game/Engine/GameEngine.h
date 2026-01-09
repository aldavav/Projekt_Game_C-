#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <Core/Logger/LoggerMacros.h>
#include <Core/Input/InputManager.h>
#include <Game/Entities/Entity.h>
#include <Game/View/Camera.h>
#include <Game/Map/Map.h>
#include <QObject>
#include <QTimer>
#include <chrono>

class GameEngine : public QObject
{
    Q_OBJECT

public:
    static GameEngine &getInstance();

    void startGame();

    void stopGame();

    GameEngine(const GameEngine &) = delete;

    void operator=(const GameEngine &) = delete;

signals:
    void gameLoopUpdate(float deltaTime);

    void gameStateChanged(int newState);

public slots:
    void handlePlayerInput(const QString &inputCommand);

private slots:
    void gameLoopTick();

private:
    explicit GameEngine(QObject *parent = nullptr);

    ~GameEngine() override = default;

    void initializeSystems();

    void updateSimulation(float fixedStep);

    QTimer m_gameTimer;

    std::chrono::steady_clock::time_point m_lastTime;

    float m_accumulator = 0.0f;

    bool m_isRunning = false;

    int m_currentState;

    std::vector<std::unique_ptr<Entity>> m_entities;

    enum GameStates
    {
        GAME_MENU,
        GAME_RUNNING,
        GAME_OVER
    };
};

#endif
