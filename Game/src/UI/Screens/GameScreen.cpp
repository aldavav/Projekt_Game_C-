#include "GameScreen.h"

GameScreen::GameScreen(QWidget *parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{
    setMouseTracking(true);
    m_gameTimer.start();
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
    m_gameTimer.restart();

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

    painter.setPen(Qt::NoPen);
    QPointF camPos = Camera::getInstance().getCurrentPos();

    for (int i = 0; i < 8; ++i)
    {

        float driftSpeed = 5.0f + (std::abs(std::sin(i * 1.5f)) * 10.0f);
        float xOffset = i * 600.0f;

        float worldX = (m_gameTime * driftSpeed) + xOffset - (camPos.x() * 0.4f);

        float cloudX = std::fmod(worldX, width() + 1200.0f) - 600.0f;

        float baseHeight = 50.0f + (std::fmod(i * 199.9f, height() * 0.8f));
        float cloudY = baseHeight - (camPos.y() * 0.4f) + (std::sin(m_gameTime * 0.4f + i) * 20.0f);

        int brightness = 255 - (dayCycle * 160);
        int alpha = 15 + (i % 4) * 5;
        painter.setBrush(QColor(brightness, brightness, brightness, alpha));

        float sizeMod = 1.0f + (std::sin(i) * 0.2f);
        painter.drawEllipse(QPointF(cloudX, cloudY), 140 * sizeMod, 50 * sizeMod);
        painter.drawEllipse(QPointF(cloudX + (40 * sizeMod), cloudY + (15 * sizeMod)), 90 * sizeMod, 40 * sizeMod);
        painter.drawEllipse(QPointF(cloudX - (30 * sizeMod), cloudY - (10 * sizeMod)), 110 * sizeMod, 45 * sizeMod);
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
    m_gameTime += 0.016f;
    auto &cam = Camera::getInstance();
    auto &map = Map::getInstance();

    cam.update(0.016f);

    QPointF worldCenter = cam.screenToWorld(QPoint(width() / 2, height() / 2));
    map.revealRadius(worldCenter.x(), worldCenter.y(), 10);

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
    m_frameCount++;
    if (m_fpsTimer.elapsed() >= 1000)
    {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer.restart();
    }

    int totalSeconds = m_gameTimer.elapsed() / 1000;
    int mins = totalSeconds / 60;
    int secs = totalSeconds % 60;
    QString timeStr = QString("Time: %1:%2")
                          .arg(mins, 2, 10, QChar('0'))
                          .arg(secs, 2, 10, QChar('0'));

    int boxW = 220;
    int boxH = m_hasSelection ? 130 : 100;

    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(10, 10, boxW, boxH, 5, 5);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 10, QFont::Bold));
    int startX = 20;
    int startY = 30;
    int lineSpacing = 20;

    painter.drawText(startX, startY, "FPS: " + QString::number(m_fps));
    painter.drawText(startX, startY + lineSpacing, timeStr);

    if (m_hasSelection)
    {
        int sq = static_cast<int>(m_selectedHex.x());
        int sr = static_cast<int>(m_selectedHex.y());
        Tile &selectedTile = Map::getInstance().getTileAt(sq, sr);

        QString typeStr = "Unknown";
        if (selectedTile.type == TileType::WATER)
            typeStr = "Water";
        else if (selectedTile.type == TileType::DIRT)
            typeStr = "Dirt";
        else if (selectedTile.type == TileType::GRASS)
            typeStr = "Grass";
        else if (selectedTile.type == TileType::MOUNTAIN)
            typeStr = "Mountain";
        else if (selectedTile.type == TileType::ORE_DEPOSIT)
            typeStr = "Ore Deposit";

        painter.setPen(QColor("#4FC3F7"));
        painter.drawText(startX, startY + (lineSpacing * 2.5), "--- SELECTION ---");

        painter.setPen(Qt::white);
        painter.drawText(startX, startY + (lineSpacing * 3.5), QString("Coords: %1, %2").arg(sq).arg(sr));
        painter.drawText(startX, startY + (lineSpacing * 4.5), "Type: " + typeStr);
    }
    else
    {
        painter.setPen(Qt::gray);
        painter.drawText(startX, startY + (lineSpacing * 3), "No Selection");
    }

    auto &stats = Map::getInstance().getStats();
    int rightBoxW = 180;
    int rightBoxH = 100;
    int padding = 10;

    int rx = width() - rightBoxW - padding;
    int ry = padding;

    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rx, ry, rightBoxW, rightBoxH, 5, 5);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 9, QFont::Bold));

    int textX = rx + 10;
    int textY = ry + 25;
    int spacing = 18;

    painter.drawText(textX, textY, "RESOURCES DISCOVERED");
    painter.setPen(QColor("#FFD600"));
    painter.drawText(textX, textY + spacing, "Gold Ore:  " + QString::number(stats.oreCount));
    painter.setPen(QColor("#81C784"));
    painter.drawText(textX, textY + spacing * 2, "Habitable: " + QString::number(stats.grassCount));
    painter.setPen(QColor("#64B5F6"));
    painter.drawText(textX, textY + spacing * 3, "Water:     " + QString::number(stats.waterCount));

    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    int mmSize = 150;
    int mmMargin = 15;
    int mmPadding = 5;

    int mmX = width() - mmSize - mmMargin;
    int mmY = height() - mmSize - mmMargin;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(QColor("#444444"), 2));
    painter.drawRect(mmX, mmY, mmSize, mmSize);

    const int MM_RANGE = 40;
    float dotSize = (float)mmSize / (MM_RANGE * 2);

    QPointF camCenter = cam.screenToWorld(QPoint(width() / 2, height() / 2));
    int cq = static_cast<int>(camCenter.x());
    int cr = static_cast<int>(camCenter.y());

    painter.setPen(Qt::NoPen);

    float hexToPixelScale = dotSize * 0.75f;

    for (int q = cq - MM_RANGE; q <= cq + MM_RANGE; ++q)
    {
        for (int r = cr - MM_RANGE; r <= cr + MM_RANGE; ++r)
        {
            Tile &tile = map.getTileAt(q, r);

            if (tile.discovered)
            {
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

                float worldX = (3.0f / 2.0f * q);
                float worldY = (std::sqrt(3.0f) / 2.0f * q + std::sqrt(3.0f) * r);

                float centerX = (3.0f / 2.0f * cq);
                float centerY = (std::sqrt(3.0f) / 2.0f * cq + std::sqrt(3.0f) * cr);

                float screenX = (worldX - centerX) * dotSize;
                float screenY = (worldY - centerY) * dotSize;

                painter.setBrush(dotColor);

                painter.drawRect(QRectF(mmX + (mmSize / 2) + screenX,
                                        mmY + (mmSize / 2) + screenY,
                                        dotSize + 1.0f, dotSize + 1.0f));
            }
        }
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 1));
    int viewIndicatorSize = 15;
    painter.drawRect(mmX + (mmSize / 2) - (viewIndicatorSize / 2),
                     mmY + (mmSize / 2) - (viewIndicatorSize / 2),
                     viewIndicatorSize, viewIndicatorSize);

    float dayCycle = (std::sin(m_gameTime * (2.0f * M_PI / 60.0f)) * 0.5f) + 0.5f;

    int nightAlpha = static_cast<int>(dayCycle * 130);

    if (nightAlpha > 0)
    {
        painter.setBrush(QColor(15, 15, 50, nightAlpha));
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, width(), height());
    }
}
