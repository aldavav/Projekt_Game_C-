#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QTimer>
#include <QDebug>
#include <QtCore/qobjectdefs.h>
#include <QString>

class GameState;
class MapManager;
class UnitManager;

class GameEngine : public QObject
{
    Q_OBJECT

public:
    static GameEngine &getInstance();

    GameEngine(const GameEngine &) = delete;

    void operator=(const GameEngine &) = delete;

    void startGame();

    void stopGame();

signals:
    void gameLoopUpdate(float deltaTime);

    void gameStateChanged(int newState);

    void unitAdded(int unitId);

public slots:
    void handlePlayerInput(const QString &inputCommand);

private slots:
    void gameLoopTick();

private:
    explicit GameEngine(QObject *parent = nullptr);

    ~GameEngine() override;

    QTimer m_gameTimer;

    qint64 m_lastUpdateTime;

    bool m_isRunning;

    int m_currentState;

    enum GameStates
    {
        GAME_MENU,
        GAME_LOADING,
        GAME_RUNNING,
        GAME_PAUSED,
        GAME_OVER
    };

    void initializeSystems();

    void updateSystems(float deltaTime);
};

#endif
