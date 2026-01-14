#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <UI/Manager/MenuManager.h>
#include <UI/Manager/GameManager.h>
#include <qpainter.h>
#include <QPolygonF>
#include <QPointF>
#include <QPixmap>
#include <QTimer>

class QPainter;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class Tile;

class GameScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);

    void onEnter() override;

    void onExit() override;

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGameDisplay();

private:
    void drawMap(QPainter &painter);

    void drawHexagon(QPainter &painter, QPoint center, float radius);

    void drawClouds(QPainter &painter, Camera &cam, float gameTime, float zoom, float dayCycle);

    QColor getTileVisualColor(const Tile &tile, float gameTime);

    QTimer *m_updateTimer;

    QPixmap m_cloudTexture;

    QPolygonF m_cachedHex;

    float m_lastRadius = 0.0f;

    bool m_isDragging = false;

    QPoint m_lastMousePos;

    QPointF m_hoveredHex;

    QSet<int> m_pressedKeys;
};

#endif
