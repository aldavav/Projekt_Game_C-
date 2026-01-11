#include "GameScreen.h"

GameScreen::GameScreen(QWidget *parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{
    m_cloudTexture.load(":/images/assets/images/cloud.png");
    setMouseTracking(true);
    m_fpsTimer.start();
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

    cam.setWorldBounds(QRect(-2000, -1500, 3500, 3000));

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
    drawHUD(painter);
}

void GameScreen::drawMap(QPainter &painter)
{
    float dayCycle = (std::sin(m_gameTime * (2.0f * M_PI / 60.0f)) * 0.5f) + 0.5f;
    auto &cam = Camera::getInstance();
    auto &map = Map::getInstance();
    const float BASE_TILE = 32.0f;
    float zoom = cam.getZoom();
    const int VIEW_RADIUS = 20;

    QPointF centerCoord = cam.screenToWorld(QPoint(width() / 2, height() / 2));
    int cq = static_cast<int>(centerCoord.x());
    int cr = static_cast<int>(centerCoord.y());

    float baseVisualRadius = (BASE_TILE * zoom) * 0.96f;

    for (int q = cq - VIEW_RADIUS; q <= cq + VIEW_RADIUS; ++q)
    {
        int r_start = std::max(cr - VIEW_RADIUS, cr - q + cq - VIEW_RADIUS);
        int r_end = std::min(cr + VIEW_RADIUS, cr - q + cq + VIEW_RADIUS);

        for (int r = r_start; r <= r_end; ++r)
        {
            QPoint screenPos = cam.toScreen(q, r, BASE_TILE);

            if (screenPos.x() < -100 || screenPos.x() > width() + 100 ||
                screenPos.y() < -100 || screenPos.y() > height() + 100)
            {
                continue;
            }

            Tile &tile = map.getTileAt(q, r);
            float currentRadius = baseVisualRadius;

            QColor tileColor;
            switch (tile.type)
            {
            case TileType::WATER:
            {
                float wave = std::sin(m_gameTime * 2.0f) * 0.5f + 0.5f;

                QColor deepBlue("#1976D2");
                QColor lightBlue("#2196F3");

                int r = deepBlue.red() + wave * (lightBlue.red() - deepBlue.red());
                int g = deepBlue.green() + wave * (lightBlue.green() - deepBlue.green());
                int b = deepBlue.blue() + wave * (lightBlue.blue() - deepBlue.blue());

                tileColor = QColor(r, g, b);
                break;
            }
            case TileType::DIRT:
                tileColor = QColor("#D2B48C");
                break;
            case TileType::GRASS:
            {
                int jitter = (std::abs(q * 13 + r * 7) % 20) - 10;
                tileColor = QColor("#388E3C").lighter(100 + jitter);
                break;
            }
            case TileType::MOUNTAIN:
                tileColor = QColor("#757575");
                break;
            case TileType::ORE_DEPOSIT:
                tileColor = QColor("#FFD600");
                break;
            default:
                tileColor = QColor("#2E7D32");
                break;
            }

            bool isHovered = (q == (int)m_hoveredHex.x() && r == (int)m_hoveredHex.y());
            bool isSelected = (m_hasSelection && q == (int)m_selectedHex.x() && r == (int)m_selectedHex.y());

            if (!tile.discovered)
            {
                painter.setBrush(QColor(20, 20, 25));
                painter.setPen(Qt::NoPen);
            }
            else if (isSelected)
            {
                painter.setBrush(tileColor.lighter(160));
                painter.setPen(QPen(Qt::white, 2));

                if (dayCycle > 0.5f)
                {
                    painter.setBrush(tileColor.lighter(250));
                }
            }
            else if (isHovered)
            {
                int pulse = 170 + static_cast<int>(std::sin(m_gameTime * 3.0f) * 30);

                painter.setPen(QPen(QColor(255, 255, 0, pulse), 2));

                painter.setBrush(tileColor.lighter(115));
            }
            else
            {
                painter.setPen(Qt::NoPen);
                painter.setBrush(tileColor);
            }

            drawHexagon(painter, screenPos, currentRadius);
        }
    }

    QPointF camPos = cam.getCurrentPos();

    float centerX = width() / 2.0f;
    float centerY = height() / 2.0f;

    if (m_cloudTexture.isNull())
        return;

    for (int i = 0; i < 50; ++i)
    {
        float speedVar = std::abs(std::sin(i * 1.618f)) * 20.0f;

        float xOffset = i * 937.0f;

        float yOffset = std::fmod(i * 563.0f, 4000.0f);

        float worldX = (m_gameTime * speedVar) + xOffset;
        float worldY = yOffset - 2000.0f;

        worldY += std::sin(m_gameTime * 0.2f + i) * 100.0f;

        float range = 4000.0f;
        float relativeX = std::fmod(worldX - camPos.x() + range, range * 2.0f) - range;
        float relativeY = std::fmod(worldY - camPos.y() + range, range * 2.0f) - range;

        float screenX = centerX + (relativeX * zoom);
        float screenY = centerY + (relativeY * zoom);

        float baseAlpha = 0.4f + (std::abs(std::cos(i * 0.7f)) * 0.4f);

        float nightFactor = 1.0f - (dayCycle * 0.5f);
        painter.setOpacity(baseAlpha * nightFactor);

        float baseScale = 4.0f;
        float variety = 0.5f + (std::abs(std::sin(i * 2.5f)) * 1.5f);
        float scale = baseScale * variety * zoom;

        float w = m_cloudTexture.width() * scale;
        float h = m_cloudTexture.height() * scale;

        painter.drawPixmap(QRectF(screenX - w / 2, screenY - h / 2, w, h),
                           m_cloudTexture,
                           m_cloudTexture.rect());
    }
    painter.setOpacity(1.0f);
}

void GameScreen::wheelEvent(QWheelEvent *event)
{
    float zoomAmount = (event->angleDelta().y() > 0) ? 0.1f : -0.1f;
    Camera::getInstance().adjustZoom(zoomAmount);
    update();
}

void GameScreen::mousePressEvent(QMouseEvent *event)
{
    int margin = 20;
    int rectY = height() - HUD_BOX_H - margin;
    int btnY = rectY + 38;
    int startBtnX = margin + 15;
    int btnW = 40, btnH = 25, btnSpacing = 8;

    if (event->button() == Qt::LeftButton)
    {
        for (int i = 0; i < 4; ++i)
        {
            QRect btnRect(startBtnX + (i * (btnW + btnSpacing)), btnY, btnW, btnH);
            if (btnRect.contains(event->pos()))
            {
                if (i == 0)
                {
                    m_isPaused = !m_isPaused;
                }
                else
                {
                    m_isPaused = false;
                    if (i == 1)
                        m_currentSpeed = GameSpeed::SLOW;
                    else if (i == 2)
                        m_currentSpeed = GameSpeed::NORMAL;
                    else if (i == 3)
                        m_currentSpeed = GameSpeed::FAST;
                }
                update();
                return;
            }
        }
    }

    int mmSize = 150;
    int mmMargin = 15;
    int mmX = width() - mmSize - mmMargin;
    int mmY = height() - mmSize - mmMargin;
    QRect minimapRect(mmX, mmY, mmSize, mmSize);

    if (minimapRect.contains(event->pos()))
    {
        float localX = event->pos().x() - (mmX + mmSize / 2);
        float localY = event->pos().y() - (mmY + mmSize / 2);

        const int MM_RANGE = 40;
        float dotSize = (float)mmSize / (MM_RANGE * 2);

        float size = 1.0f;
        float q = (2.0f / 3.0f * localX) / dotSize;
        float r = (-1.0f / 3.0f * localX + std::sqrt(3.0f) / 3.0f * localY) / dotSize;

        auto &cam = Camera::getInstance();
        QPointF camCenter = cam.screenToWorld(QPoint(width() / 2, height() / 2));

        QPointF targetWorldPos(camCenter.x() + q, camCenter.y() + r);

        cam.setTargetPos(targetWorldPos);

        update();
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = true;
        m_lastMousePos = event->pos();

        QPointF clickedHex = Camera::getInstance().screenToWorld(event->pos());

        m_selectedHex = clickedHex;
        m_hasSelection = true;

        LOG_INFO("Selected Hex: q=" + std::to_string((int)m_selectedHex.x()) +
                 ", r=" + std::to_string((int)m_selectedHex.y()));
    }

    InputManager::getInstance().onMouseClick(event->button(), event->pos());
    update();
}

void GameScreen::mouseMoveEvent(QMouseEvent *event)
{
    m_hoveredHex = Camera::getInstance().screenToWorld(event->pos());

    if (m_isDragging)
    {
        QPoint delta = event->pos() - m_lastMousePos;
        float moveX = -static_cast<float>(delta.x()) / Camera::getInstance().getZoom();
        float moveY = -static_cast<float>(delta.y()) / Camera::getInstance().getZoom();

        Camera::getInstance().move(moveX, moveY);
        m_lastMousePos = event->pos();
    }

    update();
}

void GameScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_isDragging = false;
}

void GameScreen::updateGameDisplay()
{
    switch (m_currentSpeed)
    {
    case GameSpeed::SLOW:
        m_timeScale = 0.5f;
        break;
    case GameSpeed::NORMAL:
        m_timeScale = 1.0f;
        break;
    case GameSpeed::FAST:
        m_timeScale = 2.0f;
        break;
    }

    if (!m_isPaused)
    {
        float deltaTime = 0.016f * m_timeScale;
        m_gameTime += deltaTime;
    }

    Camera::getInstance().update(0.016f);

    QPointF worldCenter = Camera::getInstance().screenToWorld(QPoint(width() / 2, height() / 2));
    Map::getInstance().revealRadius(worldCenter.x(), worldCenter.y(), 10);

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

void GameScreen::drawHUD(QPainter &painter)
{
    updateFPS();
    drawSelectionBox(painter);
    drawResourceStats(painter);
    drawMinimap(painter);
    drawDayNightCycle(painter);
}

void GameScreen::updateFPS()
{
    m_frameCount++;
    if (m_fpsTimer.elapsed() >= 1000)
    {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer.restart();
    }
}

void GameScreen::drawSelectionBox(QPainter &painter)
{
    int totalSeconds = static_cast<int>(m_gameTime);
    QString timeStr = QString("Game time: %1:%2")
                          .arg(totalSeconds / 60, 2, 10, QChar('0'))
                          .arg(totalSeconds % 60, 2, 10, QChar('0'));

    int boxW = 220;
    int boxH = m_hasSelection ? 130 : 100;

    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(10, 10, boxW, boxH, 5, 5);

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

void GameScreen::drawResourceStats(QPainter &painter)
{
    auto &stats = Map::getInstance().getStats();
    int rightBoxW = 180, rightBoxH = 100, padding = 10;
    int rx = width() - rightBoxW - padding;
    int ry = padding;

    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rx, ry, rightBoxW, rightBoxH, 5, 5);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 9, QFont::Bold));

    int textX = rx + 10, textY = ry + 25, spacing = 18;
    painter.drawText(textX, textY, "RESOURCES DISCOVERED");

    painter.setPen(QColor("#FFD600"));
    painter.drawText(textX, textY + spacing, "Gold Ore:  " + QString::number(stats.oreCount));
    painter.setPen(QColor("#81C784"));
    painter.drawText(textX, textY + spacing * 2, "Habitable: " + QString::number(stats.grassCount));
    painter.setPen(QColor("#64B5F6"));
    painter.drawText(textX, textY + spacing * 3, "Water:     " + QString::number(stats.waterCount));
}

void GameScreen::drawMinimap(QPainter &painter)
{
    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    int mmSize = 150, mmMargin = 15;
    int mmX = width() - mmSize - mmMargin;
    int mmY = height() - mmSize - mmMargin;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(QColor("#444444"), 2));
    painter.drawRect(mmX, mmY, mmSize, mmSize);

    painter.save();
    painter.setClipRect(mmX, mmY, mmSize, mmSize);

    const int MM_RANGE = 40;
    float dotSize = (float)mmSize / (MM_RANGE * 2);

    QPointF camCenter = cam.screenToWorld(QPoint(width() / 2, height() / 2));
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
            case TileType::ORE_DEPOSIT:
                dotColor = QColor("#FFD600");
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

void GameScreen::drawDayNightCycle(QPainter &painter)
{
    float timeOffset = 45.0f;
    float dayCycle = (std::sin((m_gameTime + timeOffset) * (2.0f * M_PI / 60.0f)) * 0.5f) + 0.5f;

    int nightAlpha = static_cast<int>(dayCycle * 130);
    if (nightAlpha > 0)
    {
        painter.setBrush(QColor(15, 15, 50, nightAlpha));
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, width(), height());
    }

    QString phaseText;
    QColor phaseColor;
    if (dayCycle < 0.2f)
    {
        phaseText = "NOON";
        phaseColor = QColor("#FFD600");
    }
    else if (dayCycle < 0.4f)
    {
        phaseText = "AFTERNOON";
        phaseColor = QColor("#FFA000");
    }
    else if (dayCycle < 0.6f)
    {
        phaseText = "EVENING";
        phaseColor = QColor("#FF5722");
    }
    else if (dayCycle < 0.8f)
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
    int boxW = 230;
    int boxH = 75;
    int rectX = margin;
    int rectY = height() - boxH - margin;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(phaseColor, 2));
    painter.drawRect(rectX, rectY, boxW, boxH);

    painter.save();
    painter.translate(rectX + 20, rectY + 20);
    painter.rotate(dayCycle * 360.0f);
    painter.setBrush(phaseColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(-6, -6, 12, 12);
    painter.restore();

    painter.setPen(Qt::white);
    painter.drawText(rectX + 40, rectY + 25, phaseText);

    int btnW = 42, btnH = 25, btnSpacing = 6;
    int startBtnX = rectX + 15;
    int btnY = rectY + 38;

    QString labels[] = {"||", "x0.5", "x1.0", "x2.0"};

    for (int i = 0; i < 4; ++i)
    {
        QRect btnRect(startBtnX + (i * (btnW + btnSpacing)), btnY, btnW, btnH);

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

QString GameScreen::getTileTypeName(TileType type)
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
    case TileType::ORE_DEPOSIT:
        return "Ore Deposit";
    default:
        return "Unknown";
    }
}
