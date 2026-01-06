#include "GameScreen.h"
#include "unit_builder/TankBuilder.h"
#include "unit_builder/InfantryBuilder.h"
#include "unit_builder/PlaneBuilder.h"
#include "MapWidget.h"
#include <QPushButton>
#include <QMouseEvent>
#include "Screens/dialogs/UnitBuyDialog.h"

GameScreen::GameScreen(QWidget* parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{
    setStyleSheet("background-color: black;");

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // MAPA
    MapWidget* mapWidget = new MapWidget(this);
    mainLayout->addWidget(mapWidget, 1);

    // PANEL
    QWidget* panel = new QWidget(this);
    panel->setFixedWidth(250);
    panel->setStyleSheet("background:#222;color:#ccc;");

    QVBoxLayout* panelLayout = new QVBoxLayout(panel);
    panelLayout->setAlignment(Qt::AlignTop);

    QLabel* info = new QLabel("--- Player Info ---\nResources: 0\nUnits: 0\n\n--- Commands ---");
    info->setStyleSheet("color:#00ff00;");
    panelLayout->addWidget(info);

    QPushButton* btnInf = new QPushButton("Infantry");
    QPushButton* btnTank = new QPushButton("Tank");
    QPushButton* btnPlane = new QPushButton("Plane");

    panelLayout->addWidget(btnInf);
    panelLayout->addWidget(btnTank);
    panelLayout->addWidget(btnPlane);

    QPushButton* pause = new QPushButton("Pause / Menu");
    panelLayout->addWidget(pause);

    panelLayout->addStretch();
    mainLayout->addWidget(panel);

    //connect for mapwidget
     connect(mapWidget, &MapWidget::tileClicked,this, &GameScreen::onTileClicked);

    //buywindow

    connect(btnTank, &QPushButton::clicked, this, [this]() {

        UnitBuyDialog dialog("Tank", this);
        if (dialog.exec() == QDialog::Accepted)
        {
            m_pendingUnit = UnitType::TANK;
            m_placementState = PlacementState::PLACING_UNIT;

            LOG_INFO("Tank bought. Waiting for placement.");
        }
    });
    connect(btnPlane, &QPushButton::clicked, this, [this]() {

        UnitBuyDialog dialog("Plane", this);
        if (dialog.exec() == QDialog::Accepted)
        {
            m_pendingUnit = UnitType::PLANE;
            m_placementState = PlacementState::PLACING_UNIT;

            LOG_INFO("Plane bought. Waiting for placement.");
        }
    });
    connect(btnInf, &QPushButton::clicked, this, [this]() {

        UnitBuyDialog dialog("Infantry", this);
        if (dialog.exec() == QDialog::Accepted)
        {
            m_pendingUnit = UnitType::INFANTRY;
            m_placementState = PlacementState::PLACING_UNIT;

            LOG_INFO("Infantry bought. Waiting for placement.");
        }
    });


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

    int mapWidth = width() - 250;  // Odečita šířku panelu
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

            // -- vykreslení overlay pro pohyb --
            if (m_moveMode && m_selectedUnit)
            {
                int dist = abs(x - m_selectedUnit->getX()) + abs(y - m_selectedUnit->getY());

                QColor overlay;

                if (tile->getEntity())
                    overlay = QColor(255, 0, 0, 120); // obsazeno
                else if (!tile->isTraversable())
                    overlay = QColor(255, 0, 0, 120); // blok
                else if (dist <= m_selectedUnit->getMoveRange())
                    overlay = QColor(255, 255, 0, 120); // OK
                else
                    overlay = QColor(255, 0, 0, 120); // mimo range

                painter.fillRect(tileRect, overlay);
            }
        }
    }
}

void GameScreen::updateGameDisplay()
{
    update();
}



void GameScreen::onPauseClicked()
{
    LOG_INFO("Pause button clicked. Returning to main menu.");
    MenuManager::getInstance().setScreen(new MainMenuScreen());
}
void GameScreen::onTileClicked(int x, int y)
{
    Location* location = Map::getInstance().getCurrentLocation();
    if (!location) return;

    Tile* tile = location->getTiles()[y][x];

    // KLIK NA JEDNOTKU MAPA
    if (!m_moveMode && tile->getEntity())
    {
        Unit* unit = dynamic_cast<Unit*>(tile->getEntity());
        if (!unit) return;

        m_selectedUnit = unit;
        m_moveMode = true;

        LOG_INFO("Unit selected for move");
        update();
        return;
    }

    // KLIK NA ZVOLENOU POZICI
    if (m_moveMode && m_selectedUnit)
    {
        tryMoveSelectedUnit(x, y);
        return;
    }

    if (m_placementState != PlacementState::PLACING_UNIT)
        return;

    if (!tile->isTraversable() || tile->getEntity())
        return;

    std::unique_ptr<Unit> unit;

    switch (m_pendingUnit)
    {
    case UnitType::TANK:
    {
        TankBuilder b;
        b.setOwner(location->getPlayer());
        unit = b.build();
        break;
    }
    case UnitType::INFANTRY:
    {
        InfantryBuilder b;
        b.setOwner(location->getPlayer());
        unit = b.build();
        break;
    }
    case UnitType::PLANE:
    {
        PlaneBuilder b;
        b.setOwner(location->getPlayer());
        unit = b.build();
        break;
    }
    default:
        return;
    }

    unit->setPosition(x, y);
    tile->setEntity(std::move(unit));

    // RESET
    m_pendingUnit = UnitType::NONE;
    m_placementState = PlacementState::NONE;

    update();
}

void GameScreen::tryMoveSelectedUnit(int x, int y)
{
    Location* location = Map::getInstance().getCurrentLocation();
    if (!location) return;

    Tile* target = location->getTiles()[y][x];
    Tile* oldTile = location->getTiles()[m_selectedUnit->getY()][m_selectedUnit->getX()];

    // nelze vstoupit
    if (!target->isTraversable())
        return;

    if (target->getEntity())
        return;

    if (!m_selectedUnit->canMoveTo(x, y))
        return;

    // Přesun entity: uvolnit starý tile a přesunout unique_ptr do nového tile
    std::unique_ptr<Entity> unitEntity = oldTile->takeEntity();
    unitEntity->setPosition(x, y);
    target->setEntity(std::move(unitEntity));

    m_selectedUnit = nullptr;
    m_moveMode = false;

    update();
}
