#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <UI/Manager/GameManager.h>
#include <Core/Config/GameConfig.h>
#include <Core/Config/Config.h>
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

    Q_PROPERTY(QColor grassColor MEMBER m_grassColor)

    Q_PROPERTY(QColor waterColor MEMBER m_waterColor)

    Q_PROPERTY(QColor mountainColor MEMBER m_mountainColor)

    Q_PROPERTY(QColor dirtColor MEMBER m_dirtColor)

    Q_PROPERTY(QColor selectionColor MEMBER m_selectionColor)

    Q_PROPERTY(QColor fogColor MEMBER m_fogColor)

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

    void drawMap3D(QPainter &painter, QPoint currentHover);

    void drawMap2D(QPainter &painter, QPoint currentHover);

    void drawHexagon(QPainter &painter, QPoint center, float radius, QColor color, float height);

    void drawHexagon(QPainter &painter, QPoint center, float radius);

    void drawClouds(QPainter &painter, Camera &cam, float gameTime, float zoom);

    QColor getTileVisualColor(const Tile &tile, float gameTime);

    QTimer *m_updateTimer;

    QPixmap m_cloudTexture;

    QPolygonF m_cachedHex;

    float m_lastRadius = 0.0f;

    bool m_isDragging = false;

    QPoint m_lastMousePos;

    QPointF m_hoveredHex;

    QSet<int> m_pressedKeys;

    QColor m_grassColor;

    QColor m_waterColor;

    QColor m_mountainColor;

    QColor m_dirtColor;

    QColor m_selectionColor;

    QColor m_fogColor;

    bool m_is3D = false;

    // Hover/diagnostics cache (perf)
    QPoint  m_cachedMousePos;
    QPoint  m_cachedHoverHex;
    QPointF m_cachedWorldPos;
    bool    m_hoverDirty = true;

};

#endif
