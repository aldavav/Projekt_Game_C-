#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_isFullscreen(false)
{
    applyGlobalStyles();

    setWindowTitle(Config::GAME_TITLE);
    setWindowIcon(QIcon(":/images/assets/images/icon.png"));

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    setCursor(AssetManager::getCursor(AssetManager::CursorType::Standard));

    setupBackgroundMusic();

    resize(Config::DEFAULT_WIDTH, Config::DEFAULT_HEIGHT);
    centerOnScreen();
    setWindowState(Qt::WindowMaximized);

    MenuManager::getInstance().setMainWindow(m_centralWidget);
    MenuManager::getInstance().pushScreen(new MainMenuScreen(m_centralWidget));

    connect(&GameEngine::getInstance(), &GameEngine::gameStateChanged,
            this, &MainWindow::updateWindowMetadata);

    connect(&GameEngine::getInstance(), &GameEngine::gameStateChanged,
            &MenuManager::getInstance(), &MenuManager::handleGameStateChange);

    setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::toggleFullscreen()
{
    m_isFullscreen = !m_isFullscreen;
    if (m_isFullscreen)
    {
        showFullScreen();
    }
    else
    {
        showNormal();
        centerOnScreen();
    }
}

void MainWindow::applyCustomCursor(const QString &path)
{
    QPixmap pix(path);
    QPixmap scaled = pix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setCursor(QCursor(scaled, 0, 0));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
    {
        toggleFullscreen();
        return;
    }

    InputManager::getInstance().onKeyPress(event->key());
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    InputManager::getInstance().onKeyRelease(event->key());
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    InputManager::getInstance().onMouseClick(event->button(), event->pos());
    QMainWindow::mousePressEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (m_centralWidget)
    {
        m_centralWidget->setGeometry(0, 0, width(), height());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    TacticalDialog quitDialog(
        "TERMINATE SESSION",
        "Are you sure you want to disconnect from the command uplink?",
        this);

    if (quitDialog.exec() == QDialog::Accepted)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::updateWindowMetadata()
{
    auto &map = Map::getInstance();
    if (!map.getMapName().empty())
    {
        setWindowTitle(QString("%1 - [%2] Seed: %3")
                           .arg(Config::GAME_TITLE)
                           .arg(QString::fromStdString(map.getMapName()))
                           .arg(map.getSeed()));
    }
    else
    {
        setWindowTitle(Config::GAME_TITLE);
    }
}

void MainWindow::setupBackgroundMusic()
{
    m_bgmPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_bgmPlayer->setAudioOutput(m_audioOutput);
    m_bgmPlayer->setSource(QUrl("qrc:/audio/assets/audio/music.mp3"));
    m_audioOutput->setVolume(0.4);
    m_bgmPlayer->setLoops(QMediaPlayer::Infinite);
    m_bgmPlayer->play();
}

void MainWindow::applyGlobalStyles()
{
    AssetManager::loadFonts();

    QFont orbitronFont("Orbitron");
    orbitronFont.setStyleStrategy(QFont::PreferAntialias);

    if (QFontDatabase::families().contains("Orbitron"))
    {
        QApplication::setFont(orbitronFont);
    }

    QFile styleFile(":/styles/assets/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
        styleFile.close();
    }
}

void MainWindow::centerOnScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        QRect screenGeometry = screen->availableGeometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
}
