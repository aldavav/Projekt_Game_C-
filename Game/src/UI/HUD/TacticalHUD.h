#ifndef TACTICALHUD_H
#define TACTICALHUD_H

#include <Game/Camera/Camera.h>
#include <Game/Map/Map.h>
#include <QElapsedTimer>
#include <qpainter.h>
#include <QPolygonF>
#include <qevent.h>
#include <QObject>
#include <QPointF>
#include <QString>

class QPainter;
class QMouseEvent;

enum class GameSpeed
{
    SLOW,
    NORMAL,
    FAST
};

class TacticalHUD : public QObject
{
    Q_OBJECT
public:
    explicit TacticalHUD(QObject *parent = nullptr);

    void update(float gameTime, bool isPaused, GameSpeed speed);

    void draw(QPainter &painter, int width, int height);

    bool handleMousePress(QMouseEvent *event, int width, int height);

    void setSelection(QPointF hexCoords, bool hasSelection);

signals:
    void hudButtonClicked(int buttonIndex);

    void minimapClicked(QPointF worldPos);

private:
    void drawSelectionBox(QPainter &painter, int height);

    void drawResourceStats(QPainter &painter, int width);

    void drawMinimap(QPainter &painter, int width, int height);

    void drawDayNightCycle(QPainter &painter, int width, int height);

    QString getTileTypeName(TileType type);

    void drawScanlines(QPainter &painter, QRect rect);

    void updateMinimapCache(int size, int width, int height);

    float m_gameTime = 0.0f;

    bool m_isPaused = false;

    GameSpeed m_currentSpeed = GameSpeed::NORMAL;

    QPointF m_selectedHex;

    bool m_hasSelection = false;

    QElapsedTimer m_fpsTimer;

    int m_frameCount = 0;

    int m_fps = 0;

    const int HUD_BOX_H = 75;

    QPixmap m_minimapCache;

    bool m_minimapNeedsUpdate = true;

    QRect m_minimapBox;
};

#endif
