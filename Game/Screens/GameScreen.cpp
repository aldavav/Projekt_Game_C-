#include "GameScreen.h"

GameScreen::GameScreen(QWidget *parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    this->setMinimumSize(800, 600);
    this->setStyleSheet("background-color: #000000;");

    QWidget *commandPanel = new QWidget(this);
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

    mainLayout->addWidget(commandPanel);
    mainLayout->setStretch(0, 4);
    mainLayout->setStretch(1, 1);

    connect(m_updateTimer, &QTimer::timeout, this, &GameScreen::updateGameDisplay);
}

GameScreen::~GameScreen()
{
}

void GameScreen::onEnter()
{
    LOG_INFO("Entering GameScreen. Starting game loop.");

    m_updateTimer->start(33);

    update();
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
    Location *location = Map::getInstance().getCurrentLocation();
    if (!location)
    {
        painter.drawText(rect(), Qt::AlignCenter, tr("Map Not Initialized."));
        return;
    }

    const auto &tiles = location->getTiles();
    if (tiles.empty())
    {
        painter.drawText(rect(), Qt::AlignCenter, tr("Location is empty."));
        return;
    }

    int mapWidth = width() - 250;
    int mapHeight = height();
    int tileCols = tiles[0].size();
    int tileRows = tiles.size();

    int tileSize = qMin(mapWidth / tileCols, mapHeight / tileRows);

    for (int y = 0; y < tileRows; ++y)
    {
        for (int x = 0; x < tileCols; ++x)
        {
            Tile *tile = tiles[y][x];
            if (!tile)
                continue;

            QRect tileRect(x * tileSize, y * tileSize, tileSize, tileSize);

            QColor bgColor = tile->isTraversable() ? QColor("#004D40") : QColor("#3E2723");
            painter.fillRect(tileRect, bgColor);

            if (tile->getEntity())
            {
                painter.setPen(QPen(Qt::white));
                QFont font = painter.font();
                font.setPointSize(tileSize / 2);
                painter.setFont(font);

                painter.drawText(tileRect, Qt::AlignCenter, QString::fromStdString(tile->getEntity()->getSymbol()));
            }

            painter.setPen(QPen(QColor("#1B5E20"), 1));
            painter.drawRect(tileRect);
        }
    }
}

void GameScreen::updateGameDisplay()
{

    update();
}

void GameScreen::mousePressEvent(QMouseEvent *event)
{

    InputManager::getInstance().onMouseClick(event->button(), event->pos());

    AbstractScreen::mousePressEvent(event);
}

void GameScreen::onPauseClicked()
{
    LOG_INFO("Pause button clicked. Returning to main menu.");
    
    MenuManager::getInstance().setScreen(new MainMenuScreen());
}
