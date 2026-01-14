#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_isFullscreen(false)
{

    ConfigManager::getInstance().loadConfiguration();

    applyGlobalStyles();
    setWindowTitle(Config::GAME_TITLE);
    setWindowIcon(QIcon(":/images/assets/images/icon.png"));

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    setCursor(AssetManager::getCursor(AssetManager::CursorType::Standard));

    setupBackgroundMusic();

    setupDisplayConnections();

    QTimer::singleShot(0, this, [this]()
                       { applyDisplaySettings(); });

    MenuManager::getInstance().setMainWindow(m_centralWidget);
    MenuManager::getInstance().pushScreen(new MenuScreen(m_centralWidget));

    auto &cfg = ConfigManager::getInstance().getSettings();
    QString langCode = (cfg.languageIndex == 1) ? "cz" : "en";
    GameSettingsManager::getInstance().setLanguage(langCode);

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

    updateMusicVolume();

    m_bgmPlayer->setLoops(QMediaPlayer::Infinite);
    m_bgmPlayer->play();

    connect(&AudioSettingsManager::getInstance(), &AudioSettingsManager::volumesChanged,
            this, &MainWindow::updateMusicVolume);
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

void MainWindow::updateMusicVolume()
{
    auto &cfg = ConfigManager::getInstance().getSettings();

    float masterScale = cfg.masterVol / 100.0f;
    float musicBase = cfg.musicVol / 100.0f;

    float finalVol = masterScale * musicBase;

    m_audioOutput->setVolume(finalVol);
}

void MainWindow::setupDisplayConnections()
{
    connect(&DisplaySettingsManager::getInstance(), &DisplaySettingsManager::displayChanged,
            this, &MainWindow::applyDisplaySettings);
}

void MainWindow::applyDisplaySettings()
{
    auto &cfg = ConfigManager::getInstance().getSettings();

    this->hide();

    if (cfg.windowModeIndex == 0)
    {
        setWindowFlags(Qt::Window);
        this->show();
        this->showFullScreen();
    }
    else if (cfg.windowModeIndex == 1)
    {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

        this->show();
        this->showMaximized();

        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen)
        {
            this->setGeometry(screen->geometry());
        }
    }
    else
    {
        setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

        QStringList res = DisplaySettingsManager::getInstance().getAvailableResolutions();
        if (cfg.resolutionIndex < res.size())
        {
            QStringList parts = res[cfg.resolutionIndex].split('x');
            this->resize(parts[0].toInt(), parts[1].toInt());
        }

        this->show();
        this->showNormal();
        centerOnScreen();
    }
}
