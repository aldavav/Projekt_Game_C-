#include "GameScreen.h"

GameScreen::GameScreen(QWidget *parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{
    m_cloudTexture.load(":/images/assets/images/cloud.png");
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    connect(m_updateTimer, &QTimer::timeout, this, &GameScreen::updateGameDisplay);
}

void GameScreen::onEnter()
{
    auto &cam = Camera::getInstance();
    cam.setWorldBounds(QRect(-2000, -1500, 3500, 3000));
    cam.setViewportSize(this->width(), this->height());

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    m_updateTimer->start(16);
}

void GameScreen::onExit()
{
    m_updateTimer->stop();
}

void GameScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawMap(painter);

    GameManager::getInstance().getHUD()->draw(painter, width(), height());
}

void GameScreen::mousePressEvent(QMouseEvent *event)
{
    auto &gm = GameManager::getInstance();

    if (gm.getHUD()->handleMousePress(event, width(), height()))
    {
        update();
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = true;
        m_lastMousePos = event->pos();

        QPointF clickedHex = Camera::getInstance().screenToWorld(event->pos());
        gm.handleMouseClick(clickedHex);
    }
    update();
}

void GameScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging)
    {
        QPoint delta = event->pos() - m_lastMousePos;

        float zoom = Camera::getInstance().getZoom();
        float moveX = -static_cast<float>(delta.x()) / zoom;
        float moveY = -static_cast<float>(delta.y()) / zoom;

        Camera::getInstance().move(moveX, moveY);
        m_lastMousePos = event->pos();
    }

    m_hoveredHex = Camera::getInstance().screenToWorld(event->pos());
    update();
}

void GameScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_isDragging = false;
}

void GameScreen::wheelEvent(QWheelEvent *event)
{
    float zoomAmount = (event->angleDelta().y() > 0) ? 0.1f : -0.1f;
    Camera::getInstance().adjustZoom(zoomAmount);
    update();
}

void GameScreen::resizeEvent(QResizeEvent *event)
{
    Camera::getInstance().setViewportSize(this->width(), this->height());
    AbstractScreen::resizeEvent(event);
}

void GameScreen::keyPressEvent(QKeyEvent *event)
{
    m_pressedKeys.insert(event->key());
}

void GameScreen::keyReleaseEvent(QKeyEvent *event)
{
    m_pressedKeys.remove(event->key());
}

void GameScreen::updateGameDisplay()
{
    auto &cam = Camera::getInstance();
    float zoom = cam.getZoom();

    float speed = 8.0f / zoom;

    if (m_pressedKeys.contains(Qt::Key_W))
        cam.move(0, -speed);
    if (m_pressedKeys.contains(Qt::Key_S))
        cam.move(0, speed);
    if (m_pressedKeys.contains(Qt::Key_A))
        cam.move(-speed, 0);
    if (m_pressedKeys.contains(Qt::Key_D))
        cam.move(speed, 0);

    GameManager::getInstance().update(0.016f);
    update();
}

void GameScreen::drawMap(QPainter &painter)
{
    auto &cam = Camera::getInstance();
    auto &map = Map::getInstance();
    auto &gm = GameManager::getInstance();

    float gameTime = gm.getGameTime();
    float dayCycle = (std::sin(gameTime * (2.0f * M_PI / 60.0f)) * 0.5f) + 0.5f;
    float zoom = cam.getZoom();
    const float BASE_TILE = 32.0f;
    const int VIEW_RADIUS = 20;

    QPointF centerCoord = cam.screenToWorld(QPoint(width() / 2, height() / 2));
    int cq = static_cast<int>(centerCoord.x());
    int cr = static_cast<int>(centerCoord.y());

    float baseVisualRadius = (BASE_TILE * zoom) * 0.98f;
    float gTime = gm.getGameTime();

    for (int q = cq - VIEW_RADIUS; q <= cq + VIEW_RADIUS; ++q)
    {
        int r_start = std::max(cr - VIEW_RADIUS, cr - q + cq - VIEW_RADIUS);
        int r_end = std::min(cr + VIEW_RADIUS, cr - q + cq + VIEW_RADIUS);

        for (int r = r_start; r <= r_end; ++r)
        {
            QPoint screenPos = cam.toScreen(q, r, BASE_TILE);

            if (screenPos.x() < -100 || screenPos.x() > width() + 100 ||
                screenPos.y() < -100 || screenPos.y() > height() + 100)
                continue;

            Tile &tile = map.getTileAt(q, r);
            QColor tileColor = getTileVisualColor(tile, gTime);

            bool isHovered = (q == m_hoveredHex.x() && r == m_hoveredHex.y());
            bool isSelected = (gm.hasSelection() && q == gm.getSelectedHex().x() && r == gm.getSelectedHex().y());

            if (!tile.discovered)
            {
                painter.setBrush(QColor(20, 20, 25));
                painter.setPen(Qt::NoPen);
            }
            else
            {
                if (isSelected)
                {
                    painter.setBrush(tileColor.lighter(160));
                    painter.setPen(QPen(Qt::white, 2));
                }
                else if (isHovered)
                {
                    int pulse = 170 + static_cast<int>(std::sin(gTime * 3.0f) * 30);
                    painter.setPen(QPen(QColor(255, 255, 0, pulse), 2));
                    painter.setBrush(tileColor.lighter(115));
                }
                else
                {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(tileColor);
                }
            }
            drawHexagon(painter, screenPos, baseVisualRadius);
        }
    }
    drawClouds(painter, cam, gameTime, zoom, dayCycle);
}

void GameScreen::drawHexagon(QPainter &painter, QPoint center, float radius)
{
    if (std::abs(radius - m_lastRadius) > 0.01f)
    {
        m_cachedHex.clear();
        for (int i = 0; i < 6; ++i)
        {
            float angle_rad = (M_PI / 180.0f) * (60.0f * i);
            m_cachedHex << QPointF(radius * cos(angle_rad), radius * sin(angle_rad));
        }
        m_lastRadius = radius;
    }

    painter.save();
    painter.translate(center);
    painter.drawPolygon(m_cachedHex);
    painter.restore();
}

void GameScreen::drawClouds(QPainter &painter, Camera &cam, float gameTime, float zoom, float dayCycle)
{
    if (m_cloudTexture.isNull())
        return;

    float centerX = width() / 2.0f;
    float centerY = height() / 2.0f;
    QPointF camPos = cam.getCurrentPos();

    for (int i = 0; i < 50; ++i)
    {

        float speedVar = 5.0f + (std::abs(std::sin(i * 1.618f)) * 15.0f);
        float xOffset = i * 937.0f;
        float yOffset = std::fmod(i * 563.0f, 4000.0f) - 2000.0f;

        float worldX = (gameTime * speedVar) + xOffset;
        float worldY = yOffset + (std::sin(gameTime * 0.2f + i) * 100.0f);

        float range = 4000.0f;
        float relativeX = std::fmod(worldX - camPos.x() + range, range * 2.0f) - range;
        float relativeY = std::fmod(worldY - camPos.y() + range, range * 2.0f) - range;

        float screenX = centerX + (relativeX * zoom);
        float screenY = centerY + (relativeY * zoom);

        float baseAlpha = 0.3f + (std::abs(std::cos(i * 0.7f)) * 0.3f);
        float nightFactor = 1.0f - (dayCycle * 0.6f);
        painter.setOpacity(baseAlpha * nightFactor);

        float scale = (4.0f * (0.5f + std::abs(std::sin(i * 2.5f)))) * zoom;
        float w = m_cloudTexture.width() * scale;
        float h = m_cloudTexture.height() * scale;

        painter.drawPixmap(QRectF(screenX - w / 2, screenY - h / 2, w, h), m_cloudTexture, m_cloudTexture.rect());
    }
    painter.setOpacity(1.0f);
}

QColor GameScreen::getTileVisualColor(const Tile &tile, float gameTime)
{
    switch (tile.type)
    {
    case TileType::WATER:
    {
        float wave = std::sin(GameManager::getInstance().getGameTime() * 2.0f) * 0.5f + 0.5f;
        return QColor::fromRgb(25 + wave * 10, 118 + wave * 30, 210 + wave * 40);
    }
    case TileType::GRASS:
        return QColor("#388E3C");
    case TileType::MOUNTAIN:
        return QColor("#757575");
    case TileType::DIRT:
        return QColor("#D2B48C");
    default:
        return QColor("#2E7D32");
    }
}
