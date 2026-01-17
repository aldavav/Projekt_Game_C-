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
        dir.mkpath(mapFolder);
    }

    QString savePath = mapFolder + "/initial_state.sav";

    QSettings saveFile(savePath, QSettings::IniFormat);
    saveFile.setValue("Seed", m_currentSeed);
    saveFile.setValue("Timestamp", QDateTime::currentDateTime().toString());
    saveFile.sync();

    LOG_INFO("Game auto-saved at: " + savePath);
}

void GameEngine::loadMatch(const QString &mapName)
{
    m_currentMapName = mapName;
    QString path = QCoreApplication::applicationDirPath() + "/saves/" + mapName + "/initial_state.sav";

    QSettings saveFile(path, QSettings::IniFormat);
    uint32_t seed = saveFile.value("Seed").toUInt();

    setupMatch(mapName, seed);
    setState(STATE_RUNNING);

    LOG_INFO("Match successfully reconstructed from " + path);
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
    while (input.hasPendingCommands())
    {
        if (CommandPtr cmd = input.getNextCommand())
        {
            cmd->execute(*this);
        }
    }

    updateCameraMovement(fixedStep);

    for (auto it = m_entities.begin(); it != m_entities.end();)
    {
        if (*it)
        {
            (*it)->update(fixedStep);
            ++it;
        }
        else
        {
            it = m_entities.erase(it);
        }
    }

    Camera::getInstance().update(fixedStep);
}

void GameEngine::updateCameraMovement(float fixedStep)
{
    auto &input = InputManager::getInstance();
    auto &cam = Camera::getInstance();

    QPointF velocity(0, 0);
    const float speed = 800.0f;

    if (input.isKeyPressed(static_cast<int>(Input::KeyCode::UP_ARROW)))
        velocity.setY(-1);
    if (input.isKeyPressed(static_cast<int>(Input::KeyCode::DOWN_ARROW)))
        velocity.setY(1);
    if (input.isKeyPressed(static_cast<int>(Input::KeyCode::LEFT_ARROW)))
        velocity.setX(-1);
    if (input.isKeyPressed(static_cast<int>(Input::KeyCode::RIGHT_ARROW)))
        velocity.setX(1);

    if (!velocity.isNull())
    {
        QPointF delta = velocity * speed * fixedStep;
        cam.move(delta.x(), delta.y());
    }
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

    LOG_INFO("Engine State Transition: " + QString::number(static_cast<int>(newState)));

    emit gameStateChanged(static_cast<int>(m_currentState));
}

void GameEngine::triggerEndGame(bool victory)
{
    m_playerWon = victory;
    setState(STATE_GAMEOVER);
}
