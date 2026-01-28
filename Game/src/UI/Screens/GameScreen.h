#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <Core/Config/Configuration.h>
#include <Game/Engine/GameManager.h>
#include <UI/Manager/MenuManager.h>
#include <Core/Common/WorldData.h>
#include <Game/Camera/Camera.h> // Potřebujeme pro Camera::getInstance() v UI
#include <Game/Map/Map.h>    // Potřebujeme pro Map::getInstance()
#include <Game/Entities/Entity.h>
#include <Game/Entities/UnitBuilder.h>

#include <qpainter.h>
#include <QPolygonF>
#include <QPointF>
#include <QPixmap>
#include <QTimer>
#include <QSet>
#include <QMap>

// Forward deklarace
class QResizeEvent;
class QPaintEvent;
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

class GameScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);
    void onEnter() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateGameDisplay();

private:
    // --- LOGIKA MENU A AKCÍ ---
    void showProductionMenu(const QPoint& globalPos, const QPoint& hexPos);
    void showUnitMenu(const QPoint& globalPos, Entity* unit);

    // --- NOVÁ LOGIKA PRO NEPŘÍTELE ---
    void spawnEnemy(int x, int y);

    // --- VYKRESLOVÁNÍ ---
    void setupUI(QPainter &painter);
    void drawMap3D(QPainter &painter, QPoint currentHover);
    void drawMap2D(QPainter &painter, QPoint currentHover);
    void drawHexagon(QPainter &painter, QPoint center, float radius, QColor color, float height);
    void drawHexagon(QPainter &painter, QPoint center, float radius);
    void drawClouds(QPainter &painter, Camera &cam, float gameTime, float zoom);
    QColor getTileVisualColor(const World::Tile &tile, float gameTime, bool overrideValue = false);

    void loadTextures();

private:
    // Textury a zdroje
    QMap<QString, QPixmap> m_unitTextures;
    QPixmap m_cloudTexture;
    QTimer *m_updateTimer = nullptr;

    // Cache a stavové proměnné
    QPolygonF m_cachedHex;
    float m_lastRadius = 0.0f;
    QSet<int> m_pressedKeys;
    QPoint m_lastMousePos;

    bool m_isDragging = false;
    bool m_is3D = false;
    bool m_isMovingMode = false;
};

#endif // GAMESCREEN_H
