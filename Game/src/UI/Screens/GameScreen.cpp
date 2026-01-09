#include "GameScreen.h"

GameScreen::GameScreen(QWidget *parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{

    setMouseTracking(true);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    /*QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    
    int width = screenGeometry.width();
    int height = screenGeometry.height();

    this->resize(width, height);
    
    this->setMinimumSize(1024, 768);

    Camera::getInstance().setViewportSize(width, height);

    this->setStyleSheet("background-color: #000000;");*/

    /*QWidget *commandPanel = new QWidget(this);
    QVBoxLayout *panelLayout = new QVBoxLayout(commandPanel);
    panelLayout->setAlignment(Qt::AlignTop);
    commandPanel->setFixedWidth(250);
    commandPanel->setStyleSheet("background-color: #222222; color: #CCCCCC; border: 1px solid #444444;");

    QLabel *infoLabel = new QLabel("--- Player Info ---\n\nResources: 0\nUnit Count: 0\n\n--- Commands ---", commandPanel);
    infoLabel->setStyleSheet("color: #00FF00; padding: 10px;");
    panelLayout->addWidget(infoLabel);

    QPushButton *pauseButton = new QPushButton(tr("Pause / Menu"), commandPanel);
    pauseButton->setStyleSheet("QPushButton { background-color: #444444; color: white; padding: 10px; border: 1px solid #999999; }"
                               "QPushButton:hover { background-color: #666666; }");
    connect(pauseButton, &QPushButton::clicked, this, &GameScreen::onPauseClicked);
    panelLayout->addWidget(pauseButton);

    panelLayout->addStretch(1);

    mainLayout->addWidget(commandPanel);*/

    mainLayout->setContentsMargins(0, 0, 0, 0);

    connect(m_updateTimer, &QTimer::timeout, this, &GameScreen::updateGameDisplay);
}

void GameScreen::onEnter()
{
    LOG_INFO("Entering GameScreen. Resetting Camera Bounds.");

    auto &cam = Camera::getInstance();

    cam.setWorldBounds(QRect(0, 0, 10000, 10000));

    cam.setViewportSize(this->width(), this->height());

    m_updateTimer->start(16);
}

void GameScreen::onExit()
{
    LOG_INFO("Exiting GameScreen. Stopping game loop.");
    m_updateTimer->stop();
}

void GameScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawMap(painter);
}

void GameScreen::drawMap(QPainter &painter)
{
    auto &cam = Camera::getInstance();
    auto &map = Map::getInstance();

    const int BASE_TILE = 32;
    float zoom = cam.getZoom();
    int zoomTile = static_cast<int>(BASE_TILE * zoom);

    if (zoomTile <= 0)
        return;

    int startX = static_cast<int>(cam.pos().x() / BASE_TILE) - 1;
    int startY = static_cast<int>(cam.pos().y() / BASE_TILE) - 1;

    int visibleTilesX = (width() / zoomTile) + 2;
    int visibleTilesY = (height() / zoomTile) + 2;

    for (int y = startY; y <= startY + visibleTilesY; ++y)
    {
        for (int x = startX; x <= startX + visibleTilesX; ++x)
        {
            Tile &tile = map.getTileAt(x, y);

            QPoint screenPos = cam.toScreen(x, y, BASE_TILE);
            QRect tileRect(screenPos.x(), screenPos.y(), zoomTile, zoomTile);

            painter.fillRect(tileRect, (tile.type == TileType::GRASS) ? QColor("#2E7D32") : QColor("#4E342E"));

            painter.setPen(QPen(QColor(0, 0, 0, 30)));
            painter.drawRect(tileRect);
        }
    }
}

void GameScreen::wheelEvent(QWheelEvent *event)
{
    float zoomAmount = (event->angleDelta().y() > 0) ? 0.1f : -0.1f;
    Camera::getInstance().adjustZoom(zoomAmount);
    update();
}

void GameScreen::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = true;
        m_lastMousePos = event->pos();
    }
    InputManager::getInstance().onMouseClick(event->button(), event->pos());
}

void GameScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging)
    {
        QPoint delta = event->pos() - m_lastMousePos;

        float moveX = -static_cast<float>(delta.x()) / Camera::getInstance().getZoom();
        float moveY = -static_cast<float>(delta.y()) / Camera::getInstance().getZoom();

        Camera::getInstance().move(moveX, moveY);
        m_lastMousePos = event->pos();
    }
}

void GameScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_isDragging = false;
}

void GameScreen::updateGameDisplay()
{
    update();
}

void GameScreen::onPauseClicked()
{
    LOG_INFO("Returning to Menu.");
    MenuManager::getInstance().setScreen(new MainMenuScreen());
}

void GameScreen::resizeEvent(QResizeEvent *event)
{
    Camera::getInstance().setViewportSize(this->width(), this->height());
    
    AbstractScreen::resizeEvent(event);
}
