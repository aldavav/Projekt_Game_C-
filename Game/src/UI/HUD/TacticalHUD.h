#ifndef TACTICALHUD_H
#define TACTICALHUD_H

#include <Core/Config/GameConfig.h>
#include <Game/Engine/GameSpeed.h>
#include <Game/Camera/Camera.h>
#include <Core/Config/Config.h>
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

class TacticalHUD : public QObject
{
    Q_OBJECT
public:
    explicit TacticalHUD(QObject *parent = nullptr);

    void update(float gameTime, bool isPaused, GameSpeed speed);

    void draw(QPainter &painter, int width, int height);

    bool handleMousePress(QMouseEvent *event, int width, int height);

    void setSelection(QPointF hexCoords, bool hasSelection);

    void setDiagnosticsData(const QPoint &hoveredHex, const QPointF &mouseWorldPos, const QPoint &mouseScreenPos)
    {
        m_hoveredHex = hoveredHex;
        m_mouseWorldPos = mouseWorldPos;
        m_mouseScreenPos = mouseScreenPos;
    }

    void toggleDiagnostics() { m_showDiagnostics = !m_showDiagnostics; }

signals:
    void hudButtonClicked(int buttonIndex);

    void minimapClicked(QPointF worldPos);

private:
    void drawSelectionBox(QPainter &painter, int width, int height);

    void drawResourceStats(QPainter &painter, int width, int height);

    void drawMinimap(QPainter &painter, int width, int height);

    void drawDayNightCycle(QPainter &painter, int width, int height);

    void drawDiagnostics(QPainter &painter, int width, int height);

    void drawMinimapCached(QPainter &painter, int width, int height);

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

    QElapsedTimer m_minimapThrottleTimer;

    QPoint m_hoveredHex;
    
    QPointF m_mouseWorldPos;
    
    QPoint m_mouseScreenPos;

    bool m_showDiagnostics = false;
};

#endif
