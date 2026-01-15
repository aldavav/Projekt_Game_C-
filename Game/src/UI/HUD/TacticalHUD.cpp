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
    drawSelectionBox(painter, height);
    drawResourceStats(painter, width);
    drawMinimap(painter, width, height);
    drawDayNightCycle(painter, width, height);
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

    int mmSize = 150;
    int mmMargin = 15;
    int mmX = width - mmSize - mmMargin;
    int mmY = height - mmSize - mmMargin;
    QRect minimapRect(mmX, mmY, mmSize, mmSize);

    if (minimapRect.contains(event->pos()))
    {
        float localX = event->pos().x() - (mmX + mmSize / 2);
        float localY = event->pos().y() - (mmY + mmSize / 2);

        const int MM_RANGE = 40;
        float dotSize = static_cast<float>(mmSize) / (MM_RANGE * 2);

        float q = (2.0f / 3.0f * localX) / dotSize;
        float r = (-1.0f / 3.0f * localX + std::sqrt(3.0f) / 3.0f * localY) / dotSize;

        auto &cam = Camera::getInstance();
        QPointF camCenter = cam.screenToWorld(QPoint(width / 2, height / 2));
        emit minimapClicked(QPointF(camCenter.x() + q, camCenter.y() + r));
        return true;
    }

    return false;
}

void TacticalHUD::drawSelectionBox(QPainter &painter, int height)
{
    int totalSeconds = static_cast<int>(m_gameTime);
    QString timeStr = QString("Game time: %1:%2")
                          .arg(totalSeconds / 60, 2, 10, QChar('0'))
                          .arg(totalSeconds % 60, 2, 10, QChar('0'));

    int boxW = 220;
    int boxH = m_hasSelection ? 130 : 100;
    QRect boxRect(10, 10, boxW, boxH);

    painter.setBrush(QColor(0, 0, 10, 180));
    painter.setPen(QPen(QColor("#4FC3F7"), 1));
    painter.drawRoundedRect(boxRect, 5, 5);

    drawScanlines(painter, boxRect);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 10, QFont::Bold));
    int startX = 20, startY = 30, lineSpacing = 20;

    painter.drawText(startX, startY, "FPS: " + QString::number(m_fps));
    painter.drawText(startX, startY + lineSpacing, timeStr);

    if (m_hasSelection)
    {
        int sq = static_cast<int>(m_selectedHex.x());
        int sr = static_cast<int>(m_selectedHex.y());
        Tile &tile = Map::getInstance().getTileAt(sq, sr);

        painter.setPen(QColor("#4FC3F7"));
        painter.drawText(startX, startY + (lineSpacing * 2.5), "--- SELECTION ---");
        painter.setPen(Qt::white);
        painter.drawText(startX, startY + (lineSpacing * 3.5), QString("Coords: %1, %2").arg(sq).arg(sr));
        painter.drawText(startX, startY + (lineSpacing * 4.5), "Type: " + getTileTypeName(tile.type));
    }
    else
    {
        painter.setPen(Qt::gray);
        painter.drawText(startX, startY + (lineSpacing * 3), "No Selection");
    }
}

void TacticalHUD::setSelection(QPointF hexCoords, bool hasSelection)
{
    m_selectedHex = hexCoords;
    m_hasSelection = hasSelection;
}

void TacticalHUD::drawResourceStats(QPainter &painter, int width)
{
    auto &map = Map::getInstance();
    auto &stats = map.getStats();

    int rightBoxW = 180, rightBoxH = 120, padding = 10;
    QRect statsBox(width - rightBoxW - padding, padding, rightBoxW, rightBoxH);

    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(statsBox, 5, 5);

    drawScanlines(painter, statsBox);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 9, QFont::Bold));

    int textX = statsBox.x() + 10;
    int textY = statsBox.y() + 25;
    int spacing = 18;

    painter.drawText(textX, textY, "RESOURCES DISCOVERED");

    painter.setPen(QColor("#FFD600"));
    painter.drawText(textX, textY + spacing, "Gold Ore:  " + QString::number(stats.oreCount));
    painter.setPen(QColor("#81C784"));
    painter.drawText(textX, textY + spacing * 2, "Habitable: " + QString::number(stats.grassCount));
    painter.setPen(QColor("#64B5F6"));
    painter.drawText(textX, textY + spacing * 3, "Water:     " + QString::number(stats.waterCount));

    painter.setPen(QColor(200, 200, 200, 150));
    painter.setFont(QFont("Consolas", 8));
    painter.drawText(textX, textY + spacing * 4 + 5, "Seed: " + QString::number(map.getSeed()));
}

void TacticalHUD::drawMinimap(QPainter &painter, int width, int height)
{
    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    int mmSize = 150, mmMargin = 15;
    int mmX = width - mmSize - mmMargin;
    int mmY = height - mmSize - mmMargin;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(QColor("#444444"), 2));
    painter.drawRect(mmX, mmY, mmSize, mmSize);

    painter.save();
    painter.setClipRect(mmX, mmY, mmSize, mmSize);

    const int MM_RANGE = 40;
    float dotSize = static_cast<float>(mmSize) / (MM_RANGE * 2);

    QPointF camCenter = cam.screenToWorld(QPoint(width / 2, height / 2));
    int cq = static_cast<int>(camCenter.x());
    int cr = static_cast<int>(camCenter.y());

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
        phaseColor = QColor("#FFD600");
    }
    else if (cycleVal < 0.4f)
    {
        phaseText = "AFTERNOON";
        phaseColor = QColor("#FFA000");
    }
    else if (cycleVal < 0.6f)
    {
        phaseText = "EVENING";
        phaseColor = QColor("#FF5722");
    }
    else if (cycleVal < 0.8f)
    {
        phaseText = "NIGHT";
        phaseColor = QColor("#3F51B5");
    }
    else
    {
        phaseText = "MIDNIGHT";
        phaseColor = QColor("#1A237E");
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
    {
        m_minimapCache = QPixmap(size, size);
    }

    m_minimapCache.fill(QColor(0, 0, 0, 200));
    QPainter cachePainter(&m_minimapCache);
    cachePainter.setRenderHint(QPainter::Antialiasing, false);

    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    const int MM_RANGE = 40;
    float dotSize = static_cast<float>(size) / (MM_RANGE * 2);

    QPointF camCenter = cam.screenToWorld(QPoint(width / 2, height / 2));
    int cq = static_cast<int>(camCenter.x());
    int cr = static_cast<int>(camCenter.y());

    for (int q = cq - MM_RANGE; q <= cq + MM_RANGE; ++q)
    {
        for (int r = cr - MM_RANGE; r <= cr + MM_RANGE; ++r)
        {
            Tile &tile = map.getTileAt(q, r);
            if (!tile.discovered)
                continue;

            QColor dotColor = (tile.type == TileType::WATER) ? QColor("#1976D2") : (tile.type == TileType::GRASS)  ? QColor("#388E3C")
                                                                               : (tile.type == TileType::MOUNTAIN) ? QColor("#757575")
                                                                                                                   : QColor("#555555");

            float worldX = (1.5f * q);
            float worldY = (0.866f * q + 1.732f * r);
            float centerX = (1.5f * cq);
            float centerY = (0.866f * cq + 1.732f * cr);

            float screenX = (size / 2.0f) + (worldX - centerX) * dotSize;
            float screenY = (size / 2.0f) + (worldY - centerY) * dotSize;

            cachePainter.fillRect(QRectF(screenX, screenY, dotSize + 1.0f, dotSize + 1.0f), dotColor);
        }
    }
    m_minimapNeedsUpdate = false;
}
