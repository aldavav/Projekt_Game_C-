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

    cam.setWorldBounds(QRect(-1000, -1000, 2000, 2000));

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
    auto &cam = Camera::getInstance();
    auto &map = Map::getInstance();
    const float BASE_TILE = 32.0f;
    float zoom = cam.getZoom();

    const int VIEW_RADIUS = 20;

    QPointF centerCoord = cam.screenToWorld(QPoint(width() / 2, height() / 2));
    int cq = static_cast<int>(centerCoord.x());
    int cr = static_cast<int>(centerCoord.y());

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

            QColor tileColor;
            switch (tile.type)
            {
            case TileType::WATER:
                tileColor = QColor("#1976D2");
                break;
            case TileType::DIRT:
                tileColor = QColor("#D2B48C");
                break;
            case TileType::GRASS:
                tileColor = QColor("#388E3C");
                break;
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

            if (isSelected)
            {
                painter.setPen(QPen(Qt::white, 2));
                painter.setBrush(tileColor.lighter(160));
                drawHexagon(painter, screenPos, BASE_TILE * zoom);
            }
            else if (isHovered)
            {
                painter.setPen(QPen(Qt::yellow, 1));
                painter.setBrush(tileColor.lighter(120));
                drawHexagon(painter, screenPos, BASE_TILE * zoom);
            }
            else
            {
                painter.setPen(Qt::NoPen);
                painter.setBrush(tileColor);
                drawHexagon(painter, screenPos, BASE_TILE * zoom);
            }

            float visualRadius = (BASE_TILE * zoom) * 0.96f;
            drawHexagon(painter, screenPos, visualRadius);
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
    auto &cam = Camera::getInstance();
    QPoint localMouse = mapFromGlobal(QCursor::pos());

    cam.handleEdgePanning(localMouse, width(), height(), 0.016f);
    cam.update(0.016f);
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
}
