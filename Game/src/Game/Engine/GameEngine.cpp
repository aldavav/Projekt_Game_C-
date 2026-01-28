#include "GameEngine.h"

GameEngine &GameEngine::getInstance()
{
    static GameEngine instance;
    return instance;
}

void GameEngine::startGame()
{
    if (m_isRunning)
        return;

    Map::getInstance().initializeNewMap(
        m_currentMapName.toStdString(),
        static_cast<Engine::Difficulty>(m_difficulty),
        m_currentSeed,
        m_mapType);

    m_lastTime = std::chrono::steady_clock::now();
    m_accumulator = 0.0f;
    m_isRunning = true;
    setState(Engine::GameState::Running);

    m_gameTimer.start();

    saveCurrentMatch();
}

void GameEngine::stopGame()
{
    if (!m_isRunning)
        return;

    saveCurrentMatch();
    m_gameTimer.stop();
    m_isRunning = false;
    m_currentState = Engine::GameState::GameOver;

    emit gameStateChanged(static_cast<int>(m_currentState));
}

void GameEngine::triggerEndGame(bool victory)
{
    m_playerWon = victory;
    setState(Engine::GameState::GameOver);
}

void GameEngine::setupMatch(QString mapName, uint32_t seed, int difficulty, int mapType)
{
    m_currentMapName = mapName;
    m_currentSeed = seed;
    m_difficulty = difficulty;
    m_mapType = mapType;

    Camera::getInstance().setTargetPos(QPointF(0,0));
    Map::getInstance().initializeNewMap(
        mapName.toStdString(),
        static_cast<Engine::Difficulty>(difficulty),
        seed,
        mapType);
}

void GameEngine::saveCurrentMatch()
{
    QString worldDir = QCoreApplication::applicationDirPath() +
                       Config::Paths::SAVE_DIR_NAME + "/" +
                       m_currentMapName;

    QDir().mkpath(worldDir);
    QString filePath = worldDir + "/level.dat";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&file);

    out.setVersion(QDataStream::Qt_6_0);

    out << (quint32)0x48455847;
    out << Config::System::VERSION;

    out << m_currentSeed;
    out << (qint32)m_difficulty;
    out << (qint32)m_mapType;

    auto &cam = Camera::getInstance();
    QPointF currentHex = cam.worldToHex(cam.getCurrentPos());

    out << currentHex;

    file.close();
}

void GameEngine::loadMatch(const QString &mapName)
{
    QString filePath = QCoreApplication::applicationDirPath() +
                       Config::Paths::SAVE_DIR_NAME + "/" +
                       mapName + "/level.dat";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);

    quint32 magic;
    QString fileVersion;
    in >> magic;
    in >> fileVersion;

    if (magic != 0x48455847)
    {
        return;
    }

    if (fileVersion != Config::System::VERSION)
    {
        return;
    }

    uint32_t seed;
    qint32 diff, type;
    QPointF savedHexPos;

    in >> seed;
    in >> diff;
    in >> type;
    in >> savedHexPos;

    file.close();

    setupMatch(mapName, seed, diff, type);

    Camera::getInstance().setTargetPos(savedHexPos);

    m_lastTime = std::chrono::steady_clock::now();
    m_accumulator = 0.0f;
    m_isRunning = true;
    setState(Engine::GameState::Running);
    m_gameTimer.start();
}

void GameEngine::setState(Engine::GameState newState)
{
    if (m_currentState == newState)
        return;

    m_currentState = newState;

    emit gameStateChanged(static_cast<int>(m_currentState));
}

void GameEngine::handlePlayerInput(const QString &inputCommand)
{
    auto &input = InputManager::getInstance();
}

void GameEngine::gameLoopTick()
{
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - m_lastTime;
    m_lastTime = currentTime;

    float frameTime = elapsed.count();

    if (frameTime > Config::System::MAX_FRAME_TIME)
        frameTime = Config::System::MAX_FRAME_TIME;

    m_accumulator += frameTime;

    while (m_accumulator >= Config::Simulation::TICK_RATE)
    {
        updateSimulation(Config::Simulation::TICK_RATE);
        m_accumulator -= Config::Simulation::TICK_RATE;
    }

    emit gameLoopUpdate(frameTime);
}

GameEngine::GameEngine(QObject *parent)
    : QObject(parent), m_currentState(Engine::GameState::Menu)
{
    connect(&m_gameTimer, &QTimer::timeout, this, &GameEngine::gameLoopTick);

    m_gameTimer.setInterval(Config::System::TIMER_INTERVAL_MS);
    initializeSystems();
}

void GameEngine::initializeSystems()
{
    InputManager::getInstance();
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
    Camera::getInstance().update(fixedStep);

    m_autoSaveAccumulator += fixedStep;
    if (m_autoSaveAccumulator >= 60.0f)
    {
        saveCurrentMatch();
        m_autoSaveAccumulator = 0.0f;
    }

    m_cleanupAccumulator += fixedStep;
    if (m_cleanupAccumulator >= 5.0f)
    {
        m_cleanupAccumulator = 0.0f;
    }

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
}

void GameEngine::updateCameraMovement(float fixedStep)
{
    auto &input = InputManager::getInstance();
    auto &cam = Camera::getInstance();
    auto &gm = GameManager::getInstance();

    QPointF velocity(0, 0);

    if (input.isKeyPressed(Qt::Key_Up) || input.isKeyPressed(Qt::Key_W))
        velocity.setY(-1);
    if (input.isKeyPressed(Qt::Key_Down) || input.isKeyPressed(Qt::Key_S))
        velocity.setY(1);
    if (input.isKeyPressed(Qt::Key_Left) || input.isKeyPressed(Qt::Key_A))
        velocity.setX(-1);
    if (input.isKeyPressed(Qt::Key_Right) || input.isKeyPressed(Qt::Key_D))
        velocity.setX(1);

    if (!velocity.isNull())
    {
        float speedMultiplier = (gm.getSpeed() == Engine::GameSpeed::Fast) ? 2.0f : 1.0f;
        QPointF delta = velocity * Config::World::CAMERA_KEYBOARD_SPEED * fixedStep * speedMultiplier;
        cam.move(delta.x(), delta.y());
    }
}
