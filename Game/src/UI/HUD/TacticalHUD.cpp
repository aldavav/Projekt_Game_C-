#include "TacticalHUD.h"

TacticalHUD::TacticalHUD(QObject *parent)
    : QObject(parent), m_fps(0), m_frameCount(0), m_isPaused(false),
      m_hasSelection(false), m_currentSpeed(Engine::GameSpeed::Normal)
{
    m_fpsTimer.start();
}

void TacticalHUD::update(float gameTime, bool isPaused, Engine::GameSpeed speed)
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
    auto &gm = GameManager::getInstance();
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_showDiagnostics)
    {
        drawDiagnostics(painter, width, height);
    }

    drawResourceStats(painter, width, height);

    int mmSize = Config::UI::HUD_MINIMAP_SIZE;
    int mmMargin = Config::UI::HUD_MARGIN;
    int mmX = width - mmSize - mmMargin;
    int mmY = mmMargin;
    m_minimapBox = QRect(mmX, mmY, mmSize, mmSize);

    QPixmap mmCache = m_minimapProvider.getMinimap(mmSize, width, height, gm.getIsDiscoveryActive());
    painter.drawPixmap(mmX, mmY, mmCache);

    painter.setPen(QPen(QColor(Config::UI::COLOR_TACTICAL_BLUE), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(m_minimapBox);

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

    if (m_homeButtonRect.contains(event->pos()))
    {
        emit homeButtonClicked();
        return true;
    }

    if (m_minimapBox.contains(event->pos()))
    {
        QPointF targetWorldPos = m_minimapProvider.screenToWorld(event->pos(), m_minimapBox);
        emit minimapClicked(targetWorldPos);
        return true;
    }

    return false;
}

void TacticalHUD::setSelection(QPointF hexCoords, bool hasSelection)
{
    m_selectedHex = hexCoords;
    m_hasSelection = hasSelection;
}

void TacticalHUD::setDiagnosticsData(const QPoint &hoveredHex, const QPointF &mouseWorldPos, const QPoint &mouseScreenPos, uint32_t mapSeed)
{
    m_hoveredHex = hoveredHex;
    m_mouseWorldPos = mouseWorldPos;
    m_mouseScreenPos = mouseScreenPos;
    m_mapSeed = mapSeed;
}

void TacticalHUD::drawResourceStats(QPainter &painter, int width, int height)
{
    int rightBoxW = Config::UI::HUD_STATS_WIDTH * 2;
    int rightBoxH = 130;
    int padding = 15;
    QRect statsBox(width - rightBoxW - padding, height - rightBoxH - padding, rightBoxW, rightBoxH);

    painter.setBrush(QColor(0, 5, 10, 190));
    painter.setPen(QPen(QColor(Config::UI::COLOR_TACTICAL_BLUE), 1));
    painter.drawRoundedRect(statsBox, 2, 2);
    drawScanlines(painter, statsBox);

    int totalSeconds = static_cast<int>(m_gameTime);
    int hours = (totalSeconds / 3600);
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    QString timeString = QString("%1:%2:%3")
                             .arg(hours, 2, 10, QChar('0'))
                             .arg(minutes, 2, 10, QChar('0'))
                             .arg(seconds, 2, 10, QChar('0'));

    int textX = statsBox.x() + 12;
    int textY = statsBox.y() + 22;
    int spacing = 20;

    painter.setFont(QFont("Consolas", 9, QFont::Bold));
    painter.setPen(QColor(Config::UI::COLOR_TACTICAL_BLUE));
    painter.drawText(textX, textY, "MISSION TIME:");

    painter.setPen(Qt::white);

    painter.drawText(statsBox.right() - 80, textY, timeString);

    painter.setPen(QPen(QColor(Config::UI::COLOR_TACTICAL_BLUE), 1));
    painter.drawLine(textX, textY + 8, statsBox.right() - 12, textY + 8);

    textY += spacing + 5;

    painter.setFont(QFont("Consolas", 10, QFont::Bold));

    if (m_hasSelection)
    {
        int sq = static_cast<int>(m_selectedHex.x());
        int sr = static_cast<int>(m_selectedHex.y());
        World::Tile &tile = Map::getInstance().getTileAt(sq, sr);

        painter.setPen(QColor(Config::UI::COLOR_TACTICAL_BLUE));
        painter.drawText(textX, textY, ">> TACTICAL INTEL");

        painter.setPen(Qt::white);
        painter.setFont(QFont("Consolas", 9));
        painter.drawText(textX, textY + spacing, "CLASSIFICATION: " + getTileTypeName(tile.type).toUpper());

        painter.setPen(QColor(200, 200, 200, 200));
        QString statusLine;
        if (tile.type == World::TileType::Water)
        {
            painter.setPen(QColor(Config::UI::COLOR_WATER_INFO));
            statusLine = "ANALYSIS: H2O Sourced - Cooling Available";
        }
        else if (tile.type == World::TileType::Mountain)
        {
            painter.setPen(QColor(200, 100, 50));
            statusLine = "ANALYSIS: High Mineral Density Detected";
        }
        else if (tile.type == World::TileType::Grass)
        {
            painter.setPen(QColor(Config::UI::COLOR_HABITABLE));
            statusLine = "ANALYSIS: Bio-Habitable Terrain";
        }
        else
        {
            statusLine = "ANALYSIS: Standard Surface Composition";
        }

        painter.drawText(textX, textY + (spacing * 2), statusLine);

        painter.setPen(QColor(Config::UI::COLOR_TACTICAL_BLUE));
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

void TacticalHUD::drawDayNightCycle(QPainter &painter, int width, int height)
{
    float timeOffset = 45.0f;
    float cycleVal = (std::sin((m_gameTime + timeOffset) * (2.0f * M_PI / 60.0f)) * 0.5f) + 0.5f;

    QString phaseText;
    QColor phaseColor;
    if (cycleVal < 0.2f)
    {
        phaseText = "NOON";
        phaseColor = QColor(Config::UI::COLOR_PHASE_NOON);
    }
    else if (cycleVal < 0.4f)
    {
        phaseText = "AFTERNOON";
        phaseColor = QColor(Config::UI::COLOR_PHASE_AFTERNOON);
    }
    else if (cycleVal < 0.6f)
    {
        phaseText = "EVENING";
        phaseColor = QColor(Config::UI::COLOR_PHASE_EVENING);
    }
    else if (cycleVal < 0.8f)
    {
        phaseText = "NIGHT";
        phaseColor = QColor(Config::UI::COLOR_PHASE_NIGHT);
    }
    else
    {
        phaseText = "MIDNIGHT";
        phaseColor = QColor(Config::UI::COLOR_PHASE_MIDNIGHT);
    }

    int margin = 20;
    int boxW = 285, boxH = 75;
    int rectX = margin, rectY = height - boxH - margin;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(phaseColor, 2));
    painter.drawRect(rectX, rectY, boxW, boxH);
    drawScanlines(painter, QRect(rectX, rectY, boxW, boxH));

    painter.save();
    painter.translate(rectX + 20, rectY + 20);
    painter.rotate(cycleVal * 360.0f);
    painter.setBrush(phaseColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(-6, -6, 12, 12);
    painter.restore();

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 9, QFont::Bold));
    painter.drawText(rectX + 40, rectY + 25, phaseText);

    QString labels[] = {"||", "x0.5", "x1.0", "x2.0"};
    int btnW = 42, btnH = 25, btnSpacing = 6;
    int currentX = rectX + 15;

    for (int i = 0; i < 4; ++i)
    {
        QRect btnRect(currentX + (i * (btnW + btnSpacing)), rectY + 38, btnW, btnH);

        bool isActive = (i == 0 && m_isPaused) ||
                        (i == 1 && !m_isPaused && m_currentSpeed == Engine::GameSpeed::Slow) ||
                        (i == 2 && !m_isPaused && m_currentSpeed == Engine::GameSpeed::Normal) ||
                        (i == 3 && !m_isPaused && m_currentSpeed == Engine::GameSpeed::Fast);

        painter.setBrush(isActive ? phaseColor.darker(150) : QColor(50, 50, 50));
        painter.setPen(QPen(isActive ? Qt::white : Qt::gray, isActive ? 2 : 1));
        painter.drawRect(btnRect);
        painter.setPen(Qt::white);
        painter.drawText(btnRect, Qt::AlignCenter, labels[i]);

        if (i == 3)
            currentX = btnRect.right();
    }

    m_homeButtonRect = QRect(currentX + 12, rectY + 38, btnW, btnH);

    painter.setBrush(QColor(40, 45, 60));
    painter.setPen(QPen(QColor(Config::UI::COLOR_TACTICAL_BLUE), 1));
    painter.drawRect(m_homeButtonRect);

    painter.setPen(QPen(Qt::white, 1.5));
    painter.setBrush(Qt::NoBrush);

    int cx = m_homeButtonRect.center().x();
    int cy = m_homeButtonRect.center().y();

    QPolygon house;
    house << QPoint(cx - 7, cy + 5)
          << QPoint(cx - 7, cy - 1)
          << QPoint(cx, cy - 7)
          << QPoint(cx + 7, cy - 1)
          << QPoint(cx + 7, cy + 5);

    painter.drawPolyline(house);
    painter.drawLine(cx - 7, cy + 5, cx + 7, cy + 5);

    if (m_isPaused)
    {
        painter.setPen(Qt::red);
        painter.drawText(rectX + boxW - 65, rectY + 25, "PAUSED");
    }
}

void TacticalHUD::drawDiagnostics(QPainter &painter, int width, int height)
{
    int boxW = 250;
    int boxH = 118;
    int padding = Config::UI::HUD_MARGIN;

    QRect diagRect(padding, padding, boxW, boxH);

    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(QPen(QColor(Config::UI::COLOR_TACTICAL_BLUE), 1));
    painter.drawRoundedRect(diagRect, 2, 2);
    drawScanlines(painter, diagRect);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 9));

    int x = diagRect.x() + 10;
    int y = diagRect.y() + 20;
    int step = 18;

    painter.setPen(m_fps < 30 ? Qt::red : QColor(Config::UI::COLOR_TACTICAL_BLUE));
    painter.drawText(x, y, QString("FPS: %1").arg(m_fps));

    painter.setPen(Qt::white);
    painter.drawText(x, y + step, QString("SCREEN XY: [%1, %2]").arg(m_mouseScreenPos.x()).arg(m_mouseScreenPos.y()));
    painter.drawText(x, y + step * 2, QString("WORLD  XY: [%1, %2]").arg(static_cast<int>(m_mouseWorldPos.x())).arg(static_cast<int>(m_mouseWorldPos.y())));

    painter.setPen(QColor(Config::UI::COLOR_HABITABLE));
    painter.drawText(x, y + step * 3, QString("TILE   QR: %1, %2").arg(m_hoveredHex.x()).arg(m_hoveredHex.y()));

    painter.setPen(QColor(Config::UI::COLOR_TACTICAL_BLUE));
    painter.drawText(x, y + step * 4, QString("MAP SEED: %1").arg(m_mapSeed));
}

void TacticalHUD::drawScanlines(QPainter &painter, QRect rect)
{
    painter.setPen(QColor(255, 255, 255, 15));
    for (int y = rect.y(); y < rect.y() + rect.height(); y += 3)
    {
        painter.drawLine(rect.x(), y, rect.x() + rect.width(), y);
    }
}

QString TacticalHUD::getTileTypeName(World::TileType type) const
{
    switch (type)
    {
    case World::TileType::Water:
        return "Water";
    case World::TileType::Dirt:
        return "Dirt";
    case World::TileType::Grass:
        return "Grass";
    case World::TileType::Mountain:
        return "Mountain";
    default:
        return "Unknown";
    }
}
