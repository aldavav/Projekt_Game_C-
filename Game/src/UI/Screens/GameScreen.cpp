#include "GameScreen.h"
#include <UI/Widgets/TacticalDialog.h>
#include <UI/Manager/MenuManager.h>

GameScreen::GameScreen(QWidget *parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{
    setObjectName("gameScreen");
    m_cloudTexture.load(Config::PATH_CLOUD_TEXTURE);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    connect(m_updateTimer, &QTimer::timeout, this, &GameScreen::updateGameDisplay);
}

void GameScreen::onEnter()
{
    auto &cam = Camera::getInstance();
    cam.setWorldBounds(Config::WORLD_BOUNDS);
    cam.setViewportSize(this->width(), this->height());

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    m_updateTimer->start(GameConfig::FRAME_MS);
}

void GameScreen::onExit()
{
    m_updateTimer->stop();
}

void GameScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    drawMap(painter);

    if (auto *hud = GameManager::getInstance().getHUD())
    {
        hud->draw(painter, width(), height());
    }
}

void GameScreen::mousePressEvent(QMouseEvent *event)
{
    auto &gm = GameManager::getInstance();

    if (gm.getHUD()->handleMousePress(event, width(), height()))
    {
        event->accept();
        update();
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        gm.handleMouseClick(event->pos());
    }

    update();
}

void GameScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging)
    {
        QPoint delta = event->pos() - m_lastMousePos;
        float zoom = Camera::getInstance().getZoom();

        Camera::getInstance().move(-delta.x() / zoom, -delta.y() / zoom);
        m_lastMousePos = event->pos();
    }

    update();
}

void GameScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_isDragging = false;
}

void GameScreen::wheelEvent(QWheelEvent *event)
{
    float zoomAmount = (event->angleDelta().y() > 0) ? GameConfig::ZOOM_STEP : -GameConfig::ZOOM_STEP;
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
    if (event->key() == Qt::Key_Escape)
    {
        TacticalDialog pauseMenu(tr("UPLINK SUSPENDED"),
                                 tr("PAUSE ACTIVE. DISCONNECT FROM THE CURRENT THEATER?"),
                                 this);

        if (pauseMenu.exec() == QDialog::Accepted)
        {
            auto *loading = new LoadingScreen(this->parentWidget());
            MenuManager::getInstance().setScreen(loading);

            QTimer::singleShot(100, loading, [loading]()
                               {
                loading->setStatus(tr("ARCHIVING MISSION DATA..."));
                loading->setProgress(50);
                
                GameEngine::getInstance().saveCurrentMatch();

                loading->setStatus(tr("UPLINK TERMINATED"));
                loading->setProgress(100);
                
                QTimer::singleShot(Config::LOADING_FINAL_PAUSE, [loading]() {
                    auto* mainMenu = new MenuScreen(loading->parentWidget());
                    MenuManager::getInstance().setScreen(mainMenu);
                });
            });
            return;
        }
    }
    else if (event->key() == Qt::Key_M)
    {
        m_is3D = !m_is3D;
        update();
    }
    if (event->key() == Qt::Key_F3)
    {
        auto &gm = GameManager::getInstance();
        if (gm.getHUD())
        {
            gm.getHUD()->toggleDiagnostics();
            update();
        }
    }
    else
    {
        m_pressedKeys.insert(event->key());
    }
}

void GameScreen::keyReleaseEvent(QKeyEvent *event)
{
    m_pressedKeys.remove(event->key());
}

void GameScreen::updateGameDisplay()
{
    auto &cam = Camera::getInstance();
    float zoom = cam.getZoom();

    float speed = GameConfig::CAMERA_BASE_SPEED / zoom;

    static float lastZoom = -1.0f;
    if (std::abs(zoom - lastZoom) > 0.01f)
    {
        lastZoom = zoom;
    }

    if (m_pressedKeys.contains(Qt::Key_W))
        cam.move(0, -speed);
    if (m_pressedKeys.contains(Qt::Key_S))
        cam.move(0, speed);
    if (m_pressedKeys.contains(Qt::Key_A))
        cam.move(-speed, 0);
    if (m_pressedKeys.contains(Qt::Key_D))
        cam.move(speed, 0);

    GameManager::getInstance().update(GameConfig::DELTA_TIME);
    update();
}

void GameScreen::drawMap(QPainter &painter)
{
    auto &cam = Camera::getInstance();
    auto &gm = GameManager::getInstance();

    QPoint mousePos = mapFromGlobal(QCursor::pos());
    QPoint currentHover = cam.screenToHex(mousePos, m_is3D);

    if (auto *hud = gm.getHUD())
    {
        hud->setDiagnosticsData(mousePos, cam.screenToWorld(mousePos, m_is3D), currentHover);
    }

    m_is3D ? drawMap3D(painter, currentHover) : drawMap2D(painter, currentHover);

    drawClouds(painter, cam, gm.getGameTime(), cam.getZoom());
}

void GameScreen::drawMap3D(QPainter &painter, QPoint currentHover)
{
    auto &cam = Camera::getInstance();
    auto &map = Map::getInstance();
    const float BASE_TILE = GameConfig::BASE_TILE_SIZE;
    float zoom = cam.getZoom();

    QPointF tl = cam.screenToWorld(QPoint(0, 0), true);
    QPointF br = cam.screenToWorld(QPoint(width(), height()), true);

    struct TileData
    {
        int q, r;
        QPoint pos;
        float h;
    };
    std::vector<TileData> visibleTiles;

    int minQ = static_cast<int>(floor((2.0f / 3.0f * tl.x()) / BASE_TILE)) - 2;
    int maxQ = static_cast<int>(ceil((2.0f / 3.0f * br.x()) / BASE_TILE)) + 2;

    for (int q = minQ; q <= maxQ; ++q)
    {
        int minR = static_cast<int>(floor((-1.0f / 3.0f * q * 1.5f * BASE_TILE + 0.577f * tl.y()) / BASE_TILE)) - 2;
        int maxR = static_cast<int>(ceil((-1.0f / 3.0f * q * 1.5f * BASE_TILE + 0.577f * br.y()) / BASE_TILE)) + 2;

        for (int r = minR; r <= maxR; ++r)
        {
            Tile &tile = map.getTileAt(q, r);
            if (!tile.discovered)
                continue;

            QPoint screenPos = cam.toScreen(q, r, BASE_TILE, true);
            if (screenPos.x() < -100 || screenPos.x() > width() + 100 ||
                screenPos.y() < -100 || screenPos.y() > height() + 100)
                continue;

            float h = GameConfig::HEIGHT_OFFSET;
            if (tile.type == TileType::MOUNTAIN)
                h = GameConfig::MOUNTAIN_HEIGHT;
            else if (tile.type == TileType::GRASS)
                h = GameConfig::GRASS_HEIGHT;
            else if (tile.type == TileType::DIRT)
                h = GameConfig::DIRT_HEIGHT;
            else if (tile.type == TileType::WATER)
                h = GameConfig::WATER_HEIGHT;

            visibleTiles.push_back({q, r, screenPos, h * zoom});
        }
    }

    std::sort(visibleTiles.begin(), visibleTiles.end(), [](const TileData &a, const TileData &b)
              { return a.pos.y() < b.pos.y(); });

    float radius = (BASE_TILE * zoom) * GameConfig::HEX_VISUAL_SCALE;
    for (const auto &d : visibleTiles)
    {
        bool hovered = (d.q == currentHover.x() && d.r == currentHover.y());
        bool selected = (GameManager::getInstance().hasSelection() &&
                         d.q == GameManager::getInstance().getSelectedHex().x() &&
                         d.r == GameManager::getInstance().getSelectedHex().y());

        QColor color = getTileVisualColor(map.getTileAt(d.q, d.r), GameManager::getInstance().getGameTime());
        if (selected)
            color = color.lighter(150);
        else if (hovered)
            color = color.lighter(120);

        drawHexagon(painter, d.pos, radius, color, d.h);
    }
}

void GameScreen::drawMap2D(QPainter &painter, QPoint currentHover)
{
    auto &cam = Camera::getInstance();
    auto &gm = GameManager::getInstance();
    auto &map = Map::getInstance();
    const float BASE_TILE = GameConfig::BASE_TILE_SIZE;

    QPointF topLeft = cam.screenToWorld(QPoint(0, 0), false);
    QPointF bottomRight = cam.screenToWorld(QPoint(width(), height()), false);

    int minQ = static_cast<int>(floor((2.0f / 3.0f * topLeft.x()) / BASE_TILE)) - 2;
    int maxQ = static_cast<int>(ceil((2.0f / 3.0f * bottomRight.x()) / BASE_TILE)) + 2;

    float zoom = cam.getZoom();
    float visualRadius = (BASE_TILE * zoom) * GameConfig::HEX_VISUAL_SCALE;
    QColor selectionColor = property("selectionColor").value<QColor>();

    for (int q = minQ; q <= maxQ; ++q)
    {
        int minR = static_cast<int>(floor((-1.0f / 3.0f * q * 1.5f * BASE_TILE + 0.577f * topLeft.y()) / BASE_TILE)) - 2;
        int maxR = static_cast<int>(ceil((-1.0f / 3.0f * q * 1.5f * BASE_TILE + 0.577f * bottomRight.y()) / BASE_TILE)) + 2;

        for (int r = minR; r <= maxR; ++r)
        {
            Tile &tile = map.getTileAt(q, r);
            if (!tile.discovered)
                continue;

            QPoint screenPos = cam.toScreen(q, r, BASE_TILE, false);
            if (screenPos.x() < -50 || screenPos.x() > width() + 50 ||
                screenPos.y() < -50 || screenPos.y() > height() + 50)
                continue;

            bool isSelected = (gm.hasSelection() && q == gm.getSelectedHex().x() && r == gm.getSelectedHex().y());
            bool isHovered = (q == currentHover.x() && r == currentHover.y());

            QColor tileColor = getTileVisualColor(tile, gm.getGameTime());

            if (isSelected)
            {
                painter.setBrush(tileColor.lighter(150));
                painter.setPen(QPen(selectionColor, 2));
            }
            else if (isHovered)
            {
                painter.setBrush(tileColor.lighter(120));
                painter.setPen(QPen(QColor(255, 255, 255, 180), 1));
            }
            else
            {
                painter.setBrush(tileColor);
                painter.setPen(QPen(QColor(0, 0, 0, 40), 1));
            }

            drawHexagon(painter, screenPos, visualRadius);
        }
    }
}

void GameScreen::drawHexagon(QPainter &painter, QPoint center, float radius, QColor color, float height)
{
    auto getPt = [&](int i, float z)
    {
        float ang = (M_PI / 3.0f) * i;

        return QPointF(center.x() + radius * cos(ang),
                       center.y() + (radius * sin(ang) * 0.5f) + z);
    };

    painter.setPen(Qt::NoPen);

    for (int i = 1; i <= 3; ++i)
    {
        QPolygonF side;
        side << getPt(i, -height) << getPt(i + 1, -height) << getPt(i + 1, 0) << getPt(i, 0);
        painter.setBrush(color.darker(130 + (i * 10)));
        painter.drawPolygon(side);
    }

    for (int i = 4; i <= 6; ++i)
    {
        QPolygonF side;
        int nextIdx = (i + 1) % 6;
        side << getPt(i % 6, -height) << getPt(nextIdx, -height)
             << getPt(nextIdx, 0) << getPt(i % 6, 0);

        painter.setBrush(color.darker(250));
        painter.drawPolygon(side);
    }

    QPolygonF top;
    for (int i = 0; i < 6; ++i)
        top << getPt(i, -height);
    painter.setBrush(color);
    painter.setPen(QPen(QColor(0, 0, 0, 40), 1));
    painter.drawPolygon(top);
}

void GameScreen::drawHexagon(QPainter &painter, QPoint center, float radius)
{
    if (std::abs(radius - m_lastRadius) > 0.01f)
    {
        m_cachedHex.clear();
        for (int i = 0; i < 6; ++i)
        {
            float ang = (M_PI / 3.0f) * i;
            m_cachedHex << QPointF(radius * cos(ang), radius * sin(ang));
        }
        m_lastRadius = radius;
    }
    painter.drawPolygon(m_cachedHex.translated(center));
}

void GameScreen::drawClouds(QPainter &painter, Camera &cam, float gameTime, float zoom)
{
    if (m_cloudTexture.isNull())
        return;

    float centerX = width() / 2.0f;
    float centerY = height() / 2.0f;
    QPointF camPos = cam.getCurrentPos();

    float range = GameConfig::CLOUD_MAX_RANGE;
    float halfRange = range / 2.0f;

    for (int i = 0; i < 50; ++i)
    {
        float speedVar = GameConfig::CLOUD_SPEED_BASE + (std::abs(std::sin(i * 1.618f)) * GameConfig::CLOUD_SPEED_MULT);

        float xOffset = i * 937.0f;
        float yOffset = std::fmod(i * 563.0f, range) - halfRange;

        float worldX = (gameTime * speedVar) + xOffset;
        float worldY = yOffset + (std::sin(gameTime * 0.2f + i) * 100.0f);

        float relativeX = std::fmod(worldX - camPos.x() + range, range * 2.0f) - range;
        float relativeY = std::fmod(worldY - camPos.y() + range, range * 2.0f) - range;

        float screenX = centerX + (relativeX * zoom);
        float screenY = centerY + (relativeY * zoom);

        float baseAlpha = 0.3f + (std::abs(std::cos(i * 0.7f)) * 0.3f);
        float dayCycle = (std::sin((gameTime + 45.0f) * (2.0f * M_PI / 60.0f)) * 0.5f) + 0.5f;
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
        float wave = std::sin(gameTime * GameConfig::WATER_WAVE_SPEED) * 0.5f + 0.5f;
        return m_waterColor.lighter(GameConfig::WATER_BRIGHTNESS_BASE + wave * GameConfig::WATER_BRIGHTNESS_SWING);
    }
    case TileType::GRASS:
        return m_grassColor;
    case TileType::MOUNTAIN:
        return m_mountainColor;
    case TileType::DIRT:
        return m_dirtColor;
    default:
        return m_grassColor;
    }
}
