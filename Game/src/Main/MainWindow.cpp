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
    // here will be cleanup code later
    QApplication::quit();
}
