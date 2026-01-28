#include "GameScreen.h"
#include <QMenu>
#include <cmath>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

GameScreen::GameScreen(QWidget *parent)
    : AbstractScreen(parent), m_updateTimer(new QTimer(this))
{
    setObjectName("gameScreen");
    m_cloudTexture.load(Config::Paths::CLOUD_TEXTURE);

    loadTextures();

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    connect(m_updateTimer, &QTimer::timeout, this, &GameScreen::updateGameDisplay);
}

void GameScreen::loadTextures()
{
    m_unitTextures["T"] = QPixmap(":/images/assets/images/tank.png");
    m_unitTextures["S"] = QPixmap(":/images/assets/images/soldier.png");
    m_unitTextures["P"] = QPixmap(":/images/assets/images/plane.png");
    m_unitTextures["C"] = QPixmap(":/images/assets/images/city.png");

    for(auto it = m_unitTextures.begin(); it != m_unitTextures.end(); ++it) {
        if (it.value().isNull()) {
            qDebug() << "Chyba: Nepodařilo se načíst texturu pro symbol:" << it.key();
        }
    }
}

void GameScreen::spawnEnemy(int x, int y)
{
    auto &gm = GameManager::getInstance();
    UnitBuilder builder;

    auto enemy = builder.setPosition(QPointF(x, y))
                     .makeTank()
                     .build();

    if (enemy) {
        enemy->setTeam(Team::Enemy);
        enemy->setSymbol("T");
        enemy->setSpeed(2.0f);
        enemy->setTarget(QPointF(x, y)); // Stojí na místě při spawnu

        gm.addEntity(std::move(enemy));
        qDebug() << "ENTER: Nepritel spawnut na [" << x << "," << y << "]";
    }
}

void GameScreen::onEnter()
{
    this->setFocus();
    auto &gm = GameManager::getInstance();
    if (gm.getEntities().empty()) {
        UnitBuilder builder;
        auto city = builder.setPosition(QPointF(0,0)).makeCity().build();
        gm.addEntity(std::move(city));
    }
    m_updateTimer->start(Config::System::TIMER_INTERVAL_MS);
}

void GameScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    setupUI(painter);

    if (auto *hud = GameManager::getInstance().getHUD()) {
        hud->draw(painter, width(), height());
    }
}

void GameScreen::mousePressEvent(QMouseEvent *event)
{
    auto &gm = GameManager::getInstance();
    auto &cam = Camera::getInstance();

    // Získání RAW souřadnic
    QPointF rawHex = cam.screenToHex(event->pos(), m_is3D);

    QPointF snappedTarget(std::round(rawHex.x()), std::round(rawHex.y()));

    if (gm.isPlacing()) {
        if (event->button() == Qt::LeftButton) {
            UnitBuilder builder;
            builder.setPosition(snappedTarget);
            std::unique_ptr<Entity> newUnit = nullptr;
            int cost = 0;

            if (gm.getPlacementMode() == GameManager::PlacementMode::Tank) {
                newUnit = builder.makeTank().build(); cost = 50;
            } else if (gm.getPlacementMode() == GameManager::PlacementMode::Soldier) {
                newUnit = builder.makeSoldier().build(); cost = 10;
            } else if (gm.getPlacementMode() == GameManager::PlacementMode::Plane) {
                newUnit = builder.makePlane().build(); cost = 100;
            }

            if (newUnit && gm.getGold() >= cost) {
                gm.spendGold(cost);
                gm.addEntity(std::move(newUnit));
            }
            gm.cancelPlacement();
        } else {
            gm.cancelPlacement();
        }
        update();
        return;
    }

    if (m_isMovingMode && event->button() == Qt::LeftButton) {
        if (auto* selected = gm.getSelectedUnit()) {
            // tank střed hexu
            selected->setTarget(snappedTarget);
            qDebug() << "POHYB: Tank míří přesně na" << snappedTarget;
        }
        m_isMovingMode = false;
        update();
        return;
    }

    if (event->button() == Qt::LeftButton) {
        Entity* clickedEntity = nullptr;
        for (const auto& e : gm.getEntities()) {
            // Kontrola kolize s jednotkou
            QPoint eHex(std::round(e->getPosition().x()), std::round(e->getPosition().y()));
            if (eHex == QPoint(snappedTarget.x(), snappedTarget.y())) {
                clickedEntity = e.get();
                break;
            }
        }

        if (clickedEntity) {
            gm.setSelectedUnit(clickedEntity);
            if (clickedEntity->getType() == Engine::Entity::Type::Building) {
                showProductionMenu(event->globalPosition().toPoint(), snappedTarget.toPoint());
            } else {
                showUnitMenu(event->globalPosition().toPoint(), clickedEntity);
            }
        } else {
            gm.setSelectedUnit(nullptr);
            m_isMovingMode = false;
        }
    }
    update();
}

void GameScreen::showUnitMenu(const QPoint& globalPos, Entity* unit)
{
    QMenu menu(this);
    menu.setStyleSheet("QMenu { background-color: #1a1a1a; color: white; border: 1px solid #00ffff; padding: 5px; }");
    QAction* moveAct = menu.addAction("MOVE");
    QAction* selected = menu.exec(globalPos);
    if (selected == moveAct) m_isMovingMode = true;
    update();
}

void GameScreen::showProductionMenu(const QPoint& globalPos, const QPoint& cityHex)
{
    auto &gm = GameManager::getInstance();
    QMenu menu(this);
    menu.setStyleSheet("QMenu { background-color: #1a1a1a; color: white; border: 1px solid #f0ad4e; padding: 5px; }");
    QAction* tankAct = menu.addAction("TANK (50G)");
    QAction* soldierAct = menu.addAction("SOLDIER (10G)");
    QAction* planeAct = menu.addAction("PLANE (100G)");

    QAction* selected = menu.exec(globalPos);
    if (selected == tankAct) gm.startPlacement(GameManager::PlacementMode::Tank);
    else if (selected == soldierAct) gm.startPlacement(GameManager::PlacementMode::Soldier);
    else if (selected == planeAct) gm.startPlacement(GameManager::PlacementMode::Plane);
}

void GameScreen::setupUI(QPainter &painter)
{
    auto &cam = Camera::getInstance();
    auto &gm = GameManager::getInstance();
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    QPointF rawHover = cam.screenToHex(mousePos, m_is3D);
    QPoint currentHover(std::round(rawHover.x()), std::round(rawHover.y()));

    m_is3D ? drawMap3D(painter, currentHover) : drawMap2D(painter, currentHover);

    // --- Vykreslení čáry pohybu (Důležité!) ---
    if (m_isMovingMode && gm.getSelectedUnit()) {
        QPoint startPos = cam.toScreen(gm.getSelectedUnit()->getPosition().x(),
                                       gm.getSelectedUnit()->getPosition().y(),
                                       Config::World::BASE_TILE_SIZE, m_is3D);
        QPoint endPos = mousePos; // Čára vede k myši

        painter.setPen(QPen(Qt::cyan, 2, Qt::DashLine));
        painter.drawLine(startPos, endPos);

        // Vykresli náhled cílového hexu
        QPoint targetHexPos = cam.toScreen(currentHover.x(), currentHover.y(),
                                           Config::World::BASE_TILE_SIZE, m_is3D);
        painter.setBrush(QColor(0, 255, 255, 30));
        painter.drawEllipse(targetHexPos, 15, 15);
    }

    for (const auto& entity : gm.getEntities()) {
        if (!entity) continue;

        QPoint screenPos = cam.toScreen(entity->getPosition().x(), entity->getPosition().y(), Config::World::BASE_TILE_SIZE, m_is3D);
        bool isSelected = (gm.getSelectedUnit() == entity.get());
        bool isEnemy = (entity->getTeam() == Team::Enemy);


        painter.setPen(isSelected ? QPen(Qt::cyan, 3) : (isEnemy ? QPen(Qt::red, 2) : Qt::NoPen));
        painter.setBrush(isSelected ? QColor(0, 255, 255, 40) : (isEnemy ? QColor(255, 0, 0, 60) : Qt::NoPen));
        if (isSelected || isEnemy) painter.drawEllipse(screenPos, 22, 22);

        // Symbol / Textura
        QString sym = QString::fromStdString(entity->getSymbol());
        if (m_unitTextures.contains(sym) && !m_unitTextures[sym].isNull()) {
            painter.drawPixmap(screenPos.x() - 22, screenPos.y() - 22, 45, 45, m_unitTextures[sym]);
        } else {
            painter.setBrush(isEnemy ? Qt::red : Qt::blue);
            painter.drawRect(screenPos.x()-10, screenPos.y()-10, 20, 20);
        }

        // Health bar
        painter.setBrush(Qt::black);
        painter.drawRect(screenPos.x() - 20, screenPos.y() - 30, 40, 4);
        painter.setBrush(isEnemy ? Qt::red : Qt::green);
        float hpRate = (float)entity->getHealth() / entity->getMaxHealth();
        painter.drawRect(screenPos.x() - 20, screenPos.y() - 30, (int)(40 * hpRate), 4);
    }
}

void GameScreen::drawMap2D(QPainter &painter, QPoint currentHover) {
    auto &cam = Camera::getInstance();
    auto &map = Map::getInstance();
    auto &gm = GameManager::getInstance();
    float zoom = cam.getZoom();
    const float size = Config::World::BASE_TILE_SIZE;
    float vRad = (size * zoom) * Config::World::HEX_VISUAL_SCALE;

    QPointF tl = cam.screenToWorld(QPoint(0,0), false);
    QPointF br = cam.screenToWorld(QPoint(width(), height()), false);

    for (int q = (int)(tl.x()/size)-2; q <= (int)(br.x()/size)+2; ++q) {
        for (int r = (int)(tl.y()/size)-2; r <= (int)(br.y()/size)+2; ++r) {
            World::Tile &tile = map.getTileAt(q, r);
            QPoint pos = cam.toScreen(q, r, size, false);
            QColor baseColor = getTileVisualColor(tile, gm.getGameTime(), false);

            painter.setBrush(baseColor);
            if (q == currentHover.x() && r == currentHover.y()) {
                painter.setBrush(baseColor.lighter(140));
            } else {
                painter.setPen(QPen(QColor(0, 0, 0, 40), 1));
            }

            QPolygonF hex;
            for (int i = 0; i < 6; ++i) {
                float ang = (M_PI / 3.0f) * i;
                hex << QPointF(pos.x() + vRad * std::cos(ang), pos.y() + vRad * std::sin(ang));
            }
            painter.drawPolygon(hex);
        }
    }
}

void GameScreen::drawMap3D(QPainter &p, QPoint c) { drawMap2D(p, c); }

QColor GameScreen::getTileVisualColor(const World::Tile &t, float time, bool ovr) {
    if (t.type == World::TileType::Water) return QColor(30, 80, 160);
    if (t.type == World::TileType::Grass) return QColor(50, 120, 50);
    if (t.type == World::TileType::Mountain) return QColor(90, 90, 95);
    return QColor(40, 40, 40);
}

void GameScreen::updateGameDisplay() {
    auto &cam = Camera::getInstance();
    float speed = 5.0f / cam.getZoom();
    if (m_pressedKeys.contains(Qt::Key_W)) cam.move(0, -speed);
    if (m_pressedKeys.contains(Qt::Key_S)) cam.move(0, speed);
    if (m_pressedKeys.contains(Qt::Key_A)) cam.move(-speed, 0);
    if (m_pressedKeys.contains(Qt::Key_D)) cam.move(speed, 0);
    GameManager::getInstance().update();
    update();
}

void GameScreen::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        Camera::getInstance().move(-delta.x() / Camera::getInstance().getZoom(), -delta.y() / Camera::getInstance().getZoom());
        m_lastMousePos = event->pos();
    }
    update();
}

void GameScreen::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) m_isDragging = false;
}

void GameScreen::wheelEvent(QWheelEvent *event) {
    Camera::getInstance().adjustZoom((event->angleDelta().y() > 0) ? 0.05f : -0.05f);
    update();
}

void GameScreen::keyPressEvent(QKeyEvent *event) {
    auto &gm = GameManager::getInstance();
    if (event->key() == Qt::Key_Escape) {
        gm.cancelPlacement();
        m_isMovingMode = false;
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // Spawn nepřítele na Enter
        spawnEnemy(4, 4);
    }
    else if (event->key() == Qt::Key_M) {
        m_is3D = !m_is3D;
    }
    else {
        m_pressedKeys.insert(event->key());
    }
    update();
}

void GameScreen::keyReleaseEvent(QKeyEvent *event) {
    m_pressedKeys.remove(event->key());
    update();
}

void GameScreen::resizeEvent(QResizeEvent *event) {
    Camera::getInstance().setViewportSize(width(), height());
    AbstractScreen::resizeEvent(event);
}

void GameScreen::drawHexagon(QPainter&, QPoint, float, QColor, float) {}
void GameScreen::drawHexagon(QPainter&, QPoint, float) {}
void GameScreen::drawClouds(QPainter&, Camera&, float, float) {}
