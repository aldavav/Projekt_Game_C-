#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int fontId = QFontDatabase::addApplicationFont(":/fonts/assets/fonts/Orbitron-Bold.ttf");
    if (fontId != -1)
    {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont gameFont(family);
        QApplication::setFont(gameFont);
        qApp->setStyleSheet(QString("QWidget { font-family: '%1'; }").arg(family));
    }
    else
    {
        qCritical() << "Failed to load font from resources! Check your .qrc path.";
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
    else
    {
        resize(1920, 1080);
    }

    MenuManager::getInstance().setMainWindow(m_centralWidget);
    MenuManager::getInstance().pushScreen(new MainMenuScreen(m_centralWidget));

    connect(&GameEngine::getInstance(), &GameEngine::gameStateChanged,
            &MenuManager::getInstance(), &MenuManager::handleGameStateChange);
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    InputManager::getInstance().onKeyPress(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    InputManager::getInstance().onKeyRelease(event->key());
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    InputManager::getInstance().onMouseClick(event->button(), event->pos());
    QMainWindow::mousePressEvent(event);
}
