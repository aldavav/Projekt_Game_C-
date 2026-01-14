#include "GameEngine.h"
#include <qcoreapplication.h>
#include <qdir.h>
#include <qsettings.h>

static constexpr float TICK_RATE = 1.0f / 30.0f;

GameEngine &GameEngine::getInstance()
{
    static GameEngine instance;
    return instance;
}

GameEngine::GameEngine(QObject *parent)
    : QObject(parent), m_currentState(STATE_MENU)
{
    connect(&m_gameTimer, &QTimer::timeout, this, &GameEngine::gameLoopTick);
    m_gameTimer.setInterval(16);
    initializeSystems();
}

void GameEngine::startGame()
{
    if (m_isRunning)
        return;

    LOG_INFO("Engine: Initializing World...");
    Map::getInstance().initializeNewMap("Default Campaign");

    m_lastTime = std::chrono::steady_clock::now();
    m_accumulator = 0.0f;
    m_isRunning = true;
    m_currentState = STATE_RUNNING;

    emit gameStateChanged(m_currentState);
    m_gameTimer.start();
    saveCurrentMatch();
}

void GameEngine::saveCurrentMatch()
{
    QString saveRoot = QCoreApplication::applicationDirPath() + "/saves";
    QString mapFolder = saveRoot + "/" + m_currentMapName;

    QDir dir;
    if (!dir.exists(mapFolder))
    {
        dir.mkpath(mapFolder); // Creates /saves/MapName/
    }

    QString savePath = mapFolder + "/initial_state.sav";

    // Perform binary or INI save here
    QSettings saveFile(savePath, QSettings::IniFormat);
    saveFile.setValue("Seed", m_currentSeed);
    saveFile.setValue("Timestamp", QDateTime::currentDateTime().toString());
    saveFile.sync();

    LOG_INFO("Game auto-saved at: " + savePath);
}

void GameEngine::loadMatch(QString selectedMap)
{
}

void GameEngine::stopGame()
{
    if (!m_isRunning)
        return;

    m_gameTimer.stop();
    m_isRunning = false;
    m_currentState = STATE_GAMEOVER;

    emit gameStateChanged(m_currentState);
}

void GameEngine::setupMatch(QString mapName, uint32_t seed)
{
}

void GameEngine::gameLoopTick()
{
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - m_lastTime;
    m_lastTime = currentTime;

    float frameTime = elapsed.count();
    if (frameTime > 0.25f)
        frameTime = 0.25f;

    m_accumulator += frameTime;

    while (m_accumulator >= TICK_RATE)
    {
        updateSimulation(TICK_RATE);
        m_accumulator -= TICK_RATE;
    }

    emit gameLoopUpdate(frameTime);
}

void GameEngine::updateSimulation(float fixedStep)
{
    auto &input = InputManager::getInstance();
    auto &cam = Camera::getInstance();

    const float scrollSpeed = 800.0f;
    float distance = scrollSpeed * fixedStep;
    bool moved = false;

    if (input.isKeyPressed(Qt::Key_W))
    {
        cam.move(0, -scrollSpeed * fixedStep);
        moved = true;
    }
    if (input.isKeyPressed(Qt::Key_S))
    {
        cam.move(0, distance);
        moved = true;
    }
    if (input.isKeyPressed(Qt::Key_A))
    {
        cam.move(-distance, 0);
        moved = true;
    }
    if (input.isKeyPressed(Qt::Key_D))
    {
        cam.move(distance, 0);
        moved = true;
    }

    if (moved)
    {
    }

    while (input.hasPendingCommands())
    {
        CommandPtr cmd = input.getNextCommand();
        if (cmd)
            cmd->execute(*this);
    }

    for (const auto &entity : m_entities)
    {
        if (entity)
        {
            entity->update(fixedStep);
        }
    }

    cam.update(fixedStep);
}

void GameEngine::initializeSystems()
{
    LOG_INFO("Systems Initializing...");
}

void GameEngine::handlePlayerInput(const QString &inputCommand)
{
}

void GameEngine::setState(State newState)
{
    if (m_currentState == newState)
        return;

    m_currentState = newState;

    // Log the state change for debugging
    LOG_INFO("Engine State Transition: " + QString::number(static_cast<int>(newState)));

    // Notify MenuManager and MainWindow
    emit gameStateChanged(static_cast<int>(m_currentState));
}

void GameEngine::triggerEndGame(bool victory)
{
    m_playerWon = victory;
    setState(STATE_GAMEOVER);

    // Stop internal game systems here (timers, pathfinding, etc.)
    // StopGameLoop();
}
