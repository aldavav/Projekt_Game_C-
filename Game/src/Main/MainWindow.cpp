#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_isFullscreen(false)
{
    ConfigManager::getInstance().loadConfiguration();
    auto &cfg = ConfigManager::getInstance().getSettings();
    QString langCode = (cfg.languageIndex == 1) ? "cz" : "en";
    GameSettingsManager::getInstance().setLanguage(langCode);

    applyGlobalStyles();
    setWindowTitle(Config::GAME_TITLE);
    setWindowIcon(QIcon(Config::PATH_ICON));

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    MenuManager::getInstance().setMainWindow(m_centralWidget);

    auto *loading = new LoadingScreen();
    MenuManager::getInstance().setScreen(loading);

    QTimer::singleShot(100, loading, [this, loading]()
                       {
        loading->setStatus(tr("LOADING ASSETS..."));
        loading->setProgress(30);
        
        setCursor(AssetManager::getCursor(Asset::CursorType::Standard));
        setupBackgroundMusic();

        QTimer::singleShot(Config::LOADING_STEP_DELAY, loading, [this, loading]() {
            loading->setStatus(tr("CONFIGURING DISPLAY..."));
            loading->setProgress(70);
            
            applyDisplaySettings();
            setupDisplayConnections();

            QTimer::singleShot(Config::LOADING_STEP_DELAY, loading, [this, loading]() {
                loading->setStatus(tr("SYSTEM READY"));
                loading->setProgress(100);

                QTimer::singleShot(Config::LOADING_FINAL_PAUSE, loading, [this]() {
                    auto &config = ConfigManager::getInstance();
                    
                    if (!config.getSettings().legalAccepted) {
                        auto loadLegalDoc = [](const QString &path) -> QString {
                            QFile file(path);
                            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                                return QString::fromUtf8(file.readAll());
                            }
                            return "CRITICAL ERROR: DOCUMENT NOT FOUND.";
                        };

                        QString combinedLegal = QString(
                            "<h3>%1</h3><p>%2</p>"
                            "<br><hr><br>"
                            "<h3>%3</h3><p>%4</p>")
                            .arg(tr("END USER LICENSE AGREEMENT"), 
                                 loadLegalDoc(Config::PATH_EULA), 
                                 tr("TERMS OF SERVICE"), 
                                 loadLegalDoc(Config::PATH_TOS));

                        InformationDialog dialog(tr("LEGAL PROTOCOLS"), combinedLegal, this);
                        
                        if (dialog.exec() == QDialog::Accepted) {
                            config.getSettings().legalAccepted = true;
                            config.saveConfiguration();
                            MenuManager::getInstance().setScreen(new MenuScreen(m_centralWidget));
                        } else {
                            QCoreApplication::exit(0);
                        }
                    } else {
                        MenuManager::getInstance().setScreen(new MenuScreen(m_centralWidget));
                    }
                });
            });
        }); });

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
        setWindowTitle(QString("%1 - %2")
                           .arg(Config::GAME_TITLE)
                           .arg(QString::fromStdString(map.getMapName())));
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
    m_bgmPlayer->setSource(QUrl(Config::PATH_BGM));

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

    QFile styleFile(Config::PATH_STYLES);
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
