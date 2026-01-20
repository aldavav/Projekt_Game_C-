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

    Map::getInstance().initializeNewMap(GameConfig::DEFAULT_MAP_NAME.toStdString());

    m_lastTime = std::chrono::steady_clock::now();
    m_accumulator = 0.0f;
    m_isRunning = true;
    setState(GameState::STATE_RUNNING);

    m_gameTimer.start();
    saveCurrentMatch();
}

void GameEngine::stopGame()
{
    if (!m_isRunning)
        return;

    m_gameTimer.stop();
    m_isRunning = false;
    m_currentState = GameState::STATE_GAMEOVER;

    emit gameStateChanged(m_currentState);
}

void GameEngine::triggerEndGame(bool victory)
{
    m_playerWon = victory;
    setState(GameState::STATE_GAMEOVER);
}

void GameEngine::setupMatch(QString mapName, uint32_t seed)
{
    m_currentMapName = mapName;
    m_currentSeed = seed;

    Map::getInstance().initializeNewMap(mapName.toStdString(), GameConfig::DEFAULT_DIFFICULTY);
}

void GameEngine::saveCurrentMatch()
{
    QString saveRoot = QCoreApplication::applicationDirPath() + Config::SAVE_FOLDER_NAME;
    QString mapFolder = saveRoot + "/" + m_currentMapName;

    QDir dir;
    if (!dir.exists(mapFolder))
        dir.mkpath(mapFolder);

    QString savePath = mapFolder + Config::INITIAL_SAVE_FILENAME;

    QSettings saveFile(savePath, QSettings::IniFormat);
    saveFile.setValue("Metadata/Seed", m_currentSeed);
    saveFile.setValue("Metadata/Timestamp", QDateTime::currentDateTime().toSecsSinceEpoch());

    saveFile.sync();
}

void GameEngine::loadMatch(const QString &mapName)
{
    m_currentMapName = mapName;
    QString path = QCoreApplication::applicationDirPath() + "/saves/" + mapName + "/initial_state.sav";

    QSettings saveFile(path, QSettings::IniFormat);
    uint32_t seed = saveFile.value("Seed").toUInt();

    setupMatch(mapName, seed);
    setState(GameState::STATE_RUNNING);
}

void GameEngine::setState(GameState::State newState)
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

    if (frameTime > Config::MAX_FRAME_TIME)
        frameTime = Config::MAX_FRAME_TIME;

    m_accumulator += frameTime;

    while (m_accumulator >= GameConfig::TICK_RATE)
    {
        updateSimulation(GameConfig::TICK_RATE);
        m_accumulator -= GameConfig::TICK_RATE;
    }

    emit gameLoopUpdate(frameTime);
}

GameEngine::GameEngine(QObject *parent)
    : QObject(parent), m_currentState(GameState::STATE_MENU)
{
    connect(&m_gameTimer, &QTimer::timeout, this, &GameEngine::gameLoopTick);

    m_gameTimer.setInterval(Config::TIMER_INTERVAL_MS);
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
        QPointF delta = velocity * GameConfig::CAMERA_KEYBOARD_SPEED * fixedStep;
        cam.move(delta.x(), delta.y());
    }
}
