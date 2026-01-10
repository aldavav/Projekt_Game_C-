#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int fontId = QFontDatabase::addApplicationFont(":/fonts/assets/fonts/Orbitron-Bold.ttf");
    if (fontId != -1)
    {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        qApp->setStyleSheet(QString("QWidget { font-family: '%1'; color: #e0e0e0; }").arg(family));
    }

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    setWindowTitle(tr("Command & Conquer"));
    setWindowIcon(QIcon(":/images/assets/images/icon0.png"));

    QPixmap pix(":/images/assets/images/cursor.png");
    QPixmap scaledPix = pix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QCursor customCursor(scaledPix, 0, 0);
    QGuiApplication::setOverrideCursor(customCursor);

    setupBackgroundMusic();

    if (QScreen *screen = QGuiApplication::primaryScreen())
    {
        resize(screen->availableGeometry().size());
        setWindowState(Qt::WindowMaximized);
    }

    MenuManager::getInstance().setMainWindow(m_centralWidget);
    MenuManager::getInstance().pushScreen(new MainMenuScreen(m_centralWidget));

    connect(&GameEngine::getInstance(), &GameEngine::gameStateChanged,
            &MenuManager::getInstance(), &MenuManager::handleGameStateChange);

    setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
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

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
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
