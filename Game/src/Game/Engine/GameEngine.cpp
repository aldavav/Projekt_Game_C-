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

    if (m_currentMapName.isEmpty())
    {
        setupMatch(Config::Gameplay::DEFAULT_MISSION_NAME,
                   QRandomGenerator::global()->generate(),
                   0, 0);
    }

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

    Map::getInstance().initializeNewMap(
        mapName.toStdString(),
        static_cast<Engine::Difficulty>(difficulty),
        seed,
        mapType);
}

void GameEngine::saveCurrentMatch()
{
    QString saveRoot = QCoreApplication::applicationDirPath() + Config::Paths::SAVE_DIR_NAME;
    QString mapFolder = saveRoot + "/" + m_currentMapName;

    QDir dir;
    if (!dir.exists(mapFolder))
        dir.mkpath(mapFolder);

    QString savePath = mapFolder + "/" + Config::Paths::INITIAL_SAVE_FILENAME;
    QSettings saveFile(savePath, QSettings::IniFormat);

    QPointF camPos = Camera::getInstance().getCurrentPos();

    saveFile.beginGroup("Metadata");
    saveFile.setValue("Version", Config::System::VERSION);
    saveFile.setValue("Seed", m_currentSeed);
    saveFile.setValue("Difficulty", m_difficulty);
    saveFile.setValue("MapType", m_mapType);
    saveFile.setValue("CamX", camPos.x());
    saveFile.setValue("CamY", camPos.y());
    saveFile.setValue("Timestamp", QDateTime::currentDateTime().toSecsSinceEpoch());
    saveFile.endGroup();

    saveFile.sync();
}

void GameEngine::loadMatch(const QString &mapName)
{
    m_currentMapName = mapName;
    QString path = QCoreApplication::applicationDirPath() +
                   Config::Paths::SAVE_DIR_NAME + "/" +
                   mapName + "/" +
                   Config::Paths::INITIAL_SAVE_FILENAME;

    if (!QFile::exists(path))
        return;

    QSettings saveFile(path, QSettings::IniFormat);

    uint32_t seed = saveFile.value("Metadata/Seed").toUInt();
    int diff = saveFile.value("Metadata/Difficulty", 0).toInt();
    int type = saveFile.value("Metadata/MapType", 0).toInt();

    setupMatch(mapName, seed, diff, type);

    float cx = saveFile.value("Metadata/CamX", 0.0f).toFloat();
    float cy = saveFile.value("Metadata/CamY", 0.0f).toFloat();
    Camera::getInstance().setTargetPos(QPointF(cx, cy));

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
        QPointF delta = velocity * Config::World::CAMERA_KEYBOARD_SPEED * fixedStep;
        cam.move(delta.x(), delta.y());
    }
}
