#include "TacticalHUD.h"

TacticalHUD::TacticalHUD(QObject *parent)
    : QObject(parent), m_fps(0), m_frameCount(0), m_isPaused(false),
      m_hasSelection(false), m_currentSpeed(GameSpeed::NORMAL)
{
    m_fpsTimer.start();
}

void TacticalHUD::update(float gameTime, bool isPaused, GameSpeed speed)
{
    m_gameTime = gameTime;
    m_isPaused = isPaused;
    m_currentSpeed = speed;

    m_frameCount++;
    if (m_fpsTimer.elapsed() >= 1000)
    {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer.restart();
    }
}

void TacticalHUD::draw(QPainter &painter, int width, int height)
{
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_showDiagnostics)
    {
        drawDiagnostics(painter, width, height);
    }
    drawResourceStats(painter, width, height);

    int mmSize = Config::HUD_MINIMAP_SIZE, mmMargin = Config::HUD_MARGIN;
    int mmX = width - mmSize - mmMargin;
    int mmY = mmMargin;

    if (m_minimapNeedsUpdate || m_minimapCache.isNull())
    {
        if (!m_minimapThrottleTimer.isValid() || m_minimapThrottleTimer.elapsed() > GameConfig::MINIMAP_UPDATE_MS)
        {
            updateMinimapCache(mmSize, width, height);
            m_minimapNeedsUpdate = false;
            m_minimapThrottleTimer.restart();
        }
    }

    painter.drawPixmap(mmX, mmY, m_minimapCache);

    painter.setPen(QPen(QColor(Config::COLOR_TACTICAL_BLUE), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(mmX, mmY, mmSize, mmSize);

    drawDayNightCycle(painter, width, height);

    m_minimapNeedsUpdate = true;
}

void TacticalHUD::drawMinimapCached(QPainter &painter, int width, int height)
{
    int mmSize = Config::HUD_MINIMAP_SIZE, mmMargin = Config::HUD_MARGIN;
    int mmX = width - mmSize - mmMargin;
    int mmY = mmMargin;

    if (m_minimapNeedsUpdate || m_minimapCache.isNull())
    {
        updateMinimapCache(mmSize, width, height);
    }

    painter.drawPixmap(mmX, mmY, m_minimapCache);

    painter.setPen(QPen(Qt::white, 1));
    int viSize = 20;
    painter.drawRect(mmX + (mmSize / 2) - (viSize / 2), mmY + (mmSize / 2) - (viSize / 2), viSize, viSize);
}

bool TacticalHUD::handleMousePress(QMouseEvent *event, int width, int height)
{
    int boxH = 75;
    int margin = 20;
    int rectY = height - boxH - margin;
    int btnY = rectY + 38;
    int startBtnX = margin + 15;
    int btnW = 40, btnH = 25, btnSpacing = 8;

    for (int i = 0; i < 4; ++i)
    {
        QRect btnRect(startBtnX + (i * (btnW + btnSpacing)), btnY, btnW, btnH);
        if (btnRect.contains(event->pos()))
        {
            emit hudButtonClicked(i);
            return true;
        }
    }

    int mmSize = Config::HUD_MINIMAP_SIZE, mmMargin = Config::HUD_MARGIN;
    int mmX = width - mmSize - mmMargin;
    int mmY = mmMargin;
    QRect minimapRect(mmX, mmY, mmSize, mmSize);

    if (minimapRect.contains(event->pos()))
    {
        auto &cam = Camera::getInstance();

        float relX = (event->pos().x() - (mmX + mmSize / 2.0f)) / (float)mmSize;
        float relY = (event->pos().y() - (mmY + mmSize / 2.0f)) / (float)mmSize;

        float viewPortScale = cam.getViewportWidth() * 3.0f;

        QPointF currentCamPos = cam.getCurrentPos();

        float targetWorldX = currentCamPos.x() + (relX * viewPortScale);
        float targetWorldY = currentCamPos.y() + (relY * viewPortScale);

        cam.setTargetRawPos(QPointF(targetWorldX, targetWorldY));

        return true;
    }

    return false;
}

void TacticalHUD::setSelection(QPointF hexCoords, bool hasSelection)
{
    m_selectedHex = hexCoords;
    m_hasSelection = hasSelection;
}

void TacticalHUD::drawResourceStats(QPainter &painter, int width, int height)
{
    int rightBoxW = Config::HUD_STATS_WIDTH * 2;
    int rightBoxH = 110;
    int padding = 15;
    QRect statsBox(width - rightBoxW - padding, height - rightBoxH - padding, rightBoxW, rightBoxH);

    painter.setBrush(QColor(0, 5, 10, 190));
    painter.setPen(QPen(QColor(Config::COLOR_TACTICAL_BLUE), 1));
    painter.drawRoundedRect(statsBox, 2, 2);
    drawScanlines(painter, statsBox);

    int textX = statsBox.x() + 12;
    int textY = statsBox.y() + 22;
    int spacing = 20;
    painter.setFont(QFont("Consolas", 10, QFont::Bold));

    if (m_hasSelection)
    {
        int sq = static_cast<int>(m_selectedHex.x());
        int sr = static_cast<int>(m_selectedHex.y());
        Tile &tile = Map::getInstance().getTileAt(sq, sr);

        painter.setPen(QColor(Config::COLOR_TACTICAL_BLUE));
        painter.drawText(textX, textY, ">> TACTICAL INTEL");

        painter.setPen(Qt::white);
        painter.setFont(QFont("Consolas", 9));
        painter.drawText(textX, textY + spacing, "CLASSIFICATION: " + getTileTypeName(tile.type).toUpper());

        painter.setPen(QColor(200, 200, 200, 200));
        QString statusLine;
        if (tile.type == TileType::WATER)
        {
            painter.setPen(QColor(Config::COLOR_WATER_INFO));
            statusLine = "ANALYSIS: H2O Sourced - Cooling Available";
        }
        else if (tile.type == TileType::MOUNTAIN)
        {
            painter.setPen(QColor(200, 100, 50));
            statusLine = "ANALYSIS: High Mineral Density Detected";
        }
        else if (tile.type == TileType::GRASS)
        {
            painter.setPen(QColor(Config::COLOR_HABITABLE));
            statusLine = "ANALYSIS: Bio-Habitable Terrain";
        }
        else
        {
            statusLine = "ANALYSIS: Standard Surface Composition";
        }

        painter.drawText(textX, textY + (spacing * 2), statusLine);

        painter.setPen(QColor(Config::COLOR_TACTICAL_BLUE));
        painter.drawText(textX, textY + (spacing * 3.2), "STATUS: SECURE");
    }
    else
    {
        painter.setPen(QColor(100, 100, 100));
        painter.drawText(textX, textY, ">> SYSTEM STANDBY");
        painter.setFont(QFont("Consolas", 8));
        painter.drawText(textX, textY + spacing, "NO ACTIVE SELECTION DETECTED...");
        painter.drawText(textX, textY + spacing * 2, "AWAITING TARGET COORDINATES");
    }
}

void TacticalHUD::drawMinimap(QPainter &painter, int width, int height)
{
    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    int mmSize = Config::HUD_MINIMAP_SIZE, mmMargin = Config::HUD_MARGIN;
    int mmX = width - mmSize - mmMargin;
    int mmY = mmMargin;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(QColor("#444444"), 2));
    painter.drawRect(mmX, mmY, mmSize, mmSize);

    painter.save();
    painter.setClipRect(mmX, mmY, mmSize, mmSize);

    const int MM_RANGE = GameConfig::MINIMAP_RANGE;
    float dotSize = static_cast<float>(mmSize) / (MM_RANGE * 2);

    QPointF camCenter = cam.screenToWorld(QPoint(width / 2, height / 2));
    const float size = 32.0f;
    float cq = (2.0f / 3.0f * camCenter.x()) / size;
    float cr = (-1.0f / 3.0f * camCenter.x() + std::sqrt(3.0f) / 3.0f * camCenter.y()) / size;

    for (int q = cq - MM_RANGE; q <= cq + MM_RANGE; ++q)
    {
        for (int r = cr - MM_RANGE; r <= cr + MM_RANGE; ++r)
        {
            Tile &tile = map.getTileAt(q, r);
            if (!tile.discovered)
                continue;

            QColor dotColor;
            switch (tile.type)
            {
            case TileType::WATER:
                dotColor = QColor("#1976D2");
                break;
            case TileType::GRASS:
                dotColor = QColor("#388E3C");
                break;
            case TileType::MOUNTAIN:
                dotColor = QColor("#757575");
                break;
            default:
                dotColor = QColor("#555555");
                break;
            }

            float worldX = (1.5f * q);
            float worldY = (std::sqrt(3.0f) / 2.0f * q + std::sqrt(3.0f) * r);
            float centerX = (1.5f * cq);
            float centerY = (std::sqrt(3.0f) / 2.0f * cq + std::sqrt(3.0f) * cr);

            float screenX = (worldX - centerX) * dotSize;
            float screenY = (worldY - centerY) * dotSize;

            painter.setBrush(dotColor);
            painter.setPen(Qt::NoPen);
            painter.drawRect(QRectF(mmX + (mmSize / 2) + screenX, mmY + (mmSize / 2) + screenY, dotSize + 1.0f, dotSize + 1.0f));
        }
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 1));
    int viSize = 15;
    painter.drawRect(mmX + (mmSize / 2) - (viSize / 2), mmY + (mmSize / 2) - (viSize / 2), viSize, viSize);
    painter.restore();
}

void TacticalHUD::drawDayNightCycle(QPainter &painter, int width, int height)
{
    float timeOffset = 45.0f;
    float cycleVal = (std::sin((m_gameTime + timeOffset) * (2.0f * M_PI / 60.0f)) * 0.5f) + 0.5f;

    QString phaseText;
    QColor phaseColor;
    if (cycleVal < 0.2f)
    {
        phaseText = "NOON";
        phaseColor = QColor(GameConfig::COLOR_PHASE_NOON);
    }
    else if (cycleVal < 0.4f)
    {
        phaseText = "AFTERNOON";
        phaseColor = QColor(GameConfig::COLOR_PHASE_AFTERNOON);
    }
    else if (cycleVal < 0.6f)
    {
        phaseText = "EVENING";
        phaseColor = QColor(GameConfig::COLOR_PHASE_EVENING);
    }
    else if (cycleVal < 0.8f)
    {
        phaseText = "NIGHT";
        phaseColor = QColor(GameConfig::COLOR_PHASE_NIGHT);
    }
    else
    {
        phaseText = "MIDNIGHT";
        phaseColor = QColor(GameConfig::COLOR_PHASE_MIDNIGHT);
    }

    int margin = 20;
    int boxW = 230, boxH = 75;
    int rectX = margin, rectY = height - boxH - margin;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(phaseColor, 2));
    painter.drawRect(rectX, rectY, boxW, boxH);

    painter.save();
    painter.translate(rectX + 20, rectY + 20);
    painter.rotate(cycleVal * 360.0f);
    painter.setBrush(phaseColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(-6, -6, 12, 12);
    painter.restore();

    painter.setPen(Qt::white);
    painter.drawText(rectX + 40, rectY + 25, phaseText);

    QString labels[] = {"||", "x0.5", "x1.0", "x2.0"};
    int btnW = 42, btnH = 25, btnSpacing = 6;
    for (int i = 0; i < 4; ++i)
    {
        QRect btnRect(rectX + 15 + (i * (btnW + btnSpacing)), rectY + 38, btnW, btnH);
        bool isActive = (i == 0 && m_isPaused) ||
                        (i == 1 && !m_isPaused && m_currentSpeed == GameSpeed::SLOW) ||
                        (i == 2 && !m_isPaused && m_currentSpeed == GameSpeed::NORMAL) ||
                        (i == 3 && !m_isPaused && m_currentSpeed == GameSpeed::FAST);

        painter.setBrush(isActive ? phaseColor.darker(150) : QColor(50, 50, 50));
        painter.setPen(QPen(isActive ? Qt::white : Qt::gray, isActive ? 2 : 1));
        painter.drawRect(btnRect);
        painter.setPen(Qt::white);
        painter.drawText(btnRect, Qt::AlignCenter, labels[i]);
    }

    if (m_isPaused)
    {
        painter.setPen(Qt::red);
        painter.drawText(rectX + boxW - 65, rectY + 25, "PAUSED");
    }
}

void TacticalHUD::drawDiagnostics(QPainter &painter, int width, int height)
{
    int boxW = 250;
    int boxH = 100;
    int padding = Config::HUD_MARGIN;

    QRect diagRect(padding, padding, boxW, boxH);

    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(QPen(QColor(Config::COLOR_TACTICAL_BLUE), 1));
    painter.drawRoundedRect(diagRect, 2, 2);
    drawScanlines(painter, diagRect);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 9));

    int x = diagRect.x() + 10;
    int y = diagRect.y() + 20;
    int step = 18;

    painter.setPen(m_fps < 30 ? Qt::red : QColor(Config::COLOR_TACTICAL_BLUE));
    painter.drawText(x, y, QString("FPS: %1").arg(m_fps));

    painter.setPen(Qt::white);
    painter.drawText(x, y + step, QString("SCREEN XY: [%1, %2]").arg(m_mouseScreenPos.x()).arg(m_mouseScreenPos.y()));

    painter.drawText(x, y + step * 2, QString("WORLD  XY: [%1, %2]").arg(static_cast<int>(m_mouseWorldPos.x())).arg(static_cast<int>(m_mouseWorldPos.y())));

    painter.setPen(QColor(Config::COLOR_HABITABLE));
    painter.drawText(x, y + step * 3, QString("TILE   QR: %1, %2").arg(m_hoveredHex.x()).arg(m_hoveredHex.y()));
}

QString TacticalHUD::getTileTypeName(TileType type)
{
    switch (type)
    {
    case TileType::WATER:
        return "Water";
    case TileType::DIRT:
        return "Dirt";
    case TileType::GRASS:
        return "Grass";
    case TileType::MOUNTAIN:
        return "Mountain";
    default:
        return "Unknown";
    }
}

void TacticalHUD::drawScanlines(QPainter &painter, QRect rect)
{
    painter.setPen(QColor(255, 255, 255, 15));
    for (int y = rect.y(); y < rect.y() + rect.height(); y += 3)
    {
        painter.drawLine(rect.x(), y, rect.x() + rect.width(), y);
    }
}

void TacticalHUD::updateMinimapCache(int size, int width, int height)
{
    if (m_minimapCache.size() != QSize(size, size))
        m_minimapCache = QPixmap(size, size);

    m_minimapCache.fill(QColor(10, 10, 10, 200));
    QPainter cachePainter(&m_minimapCache);

    auto &map = Map::getInstance();

    const int WORLD_BOUNDS = GameConfig::WORLD_BOUNDS;
    const float tileS = GameConfig::BASE_TILE_SIZE;

    auto &cam = Camera::getInstance();
    QPointF camPos = cam.getCurrentPos();

    int viewPortWidth = cam.getViewportWidth() * GameConfig::SCANLINE_SPACING;

    for (int q = -WORLD_BOUNDS * 2; q <= WORLD_BOUNDS * 2; ++q)
    {
        int rStart = -WORLD_BOUNDS - (q / 2) - 10;
        int rEnd = WORLD_BOUNDS - (q / 2) + 10;

        for (int r = rStart; r <= rEnd; ++r)
        {
            Tile &tile = map.getTileAt(q, r);

            QColor dotColor;
            if (tile.type == TileType::WATER)
                dotColor = QColor("#1976D2");
            else if (tile.type == TileType::GRASS)
                dotColor = QColor("#388E3C");
            else if (tile.type == TileType::MOUNTAIN)
                dotColor = QColor("#757575");
            else if (tile.type == TileType::DIRT)
                dotColor = QColor("#795548");
            else
                continue;

            float worldX = tileS * (1.5f * q);
            float worldY = tileS * (0.866f * q + 1.732f * r);

            float mmX = (size / 2.0f) + ((worldX - camPos.x()) / viewPortWidth) * size;
            float mmY = (size / 2.0f) + ((worldY - camPos.y()) / viewPortWidth) * size;

            if (mmX >= 0 && mmX < size && mmY >= 0 && mmY < size)
            {
                cachePainter.fillRect(QRectF(mmX, mmY, 2, 2), dotColor);
            }
        }
    }

    float zoom = cam.getZoom();
    float viewW = (width / zoom) / viewPortWidth * size;
    float viewH = (height / zoom) / viewPortWidth * size;

    cachePainter.setPen(QPen(Qt::white, 1));
    cachePainter.drawRect(QRectF((size - viewW) / 2.0f, (size - viewH) / 2.0f, viewW, viewH));
}
