#ifndef TACTICALHUD_H
#define TACTICALHUD_H

#include <Game/Engine/GameManager.h>
#include <UI/HUD/MinimapProvider.h>
#include <Core/Common/WorldData.h>
#include <Core/Common/GameTypes.h>
#include <Game/Camera/Camera.h>
#include <Game/Map/Map.h>
#include <QElapsedTimer>
#include <qpainter.h>
#include <QPolygonF>
#include <qevent.h>
#include <QObject>
#include <QPointF>
#include <QString>

class QMouseEvent;
class QPainter;

class TacticalHUD : public QObject
{
    Q_OBJECT

public:
    explicit TacticalHUD(QObject *parent = nullptr);

    void update(float gameTime, bool isPaused, Engine::GameSpeed speed);

    void draw(QPainter &painter, int width, int height);

    bool handleMousePress(QMouseEvent *event, int width, int height);

    void setSelection(QPointF hexCoords, bool hasSelection);

    void toggleDiagnostics() { m_showDiagnostics = !m_showDiagnostics; }

    void setDiagnosticsData(const QPoint &hoveredHex, const QPointF &mouseWorldPos, const QPoint &mouseScreenPos, uint32_t mapSeed);

signals:
    void hudButtonClicked(int buttonIndex);

    void minimapClicked(QPointF worldPos);

private:
    void drawResourceStats(QPainter &painter, int width, int height);

    void drawMinimap(QPainter &painter, int width, int height);

    void drawDayNightCycle(QPainter &painter, int width, int height);

    void drawDiagnostics(QPainter &painter, int width, int height);

    void drawScanlines(QPainter &painter, QRect rect);

    QString getTileTypeName(World::TileType type) const;

    float m_gameTime = 0.0f;

    bool m_isPaused = false;

    Engine::GameSpeed m_currentSpeed = Engine::GameSpeed::NORMAL;

    QPointF m_selectedHex;

    bool m_hasSelection = false;

    QPixmap m_minimapCache;

    QRect m_minimapBox;

    QElapsedTimer m_minimapThrottleTimer;

    bool m_minimapNeedsUpdate = true;

    QElapsedTimer m_fpsTimer;

    QPoint m_hoveredHex;

    QPointF m_mouseWorldPos;

    QPoint m_mouseScreenPos;

    int m_frameCount = 0;

    int m_fps = 0;

    uint32_t m_mapSeed = 0;

    bool m_showDiagnostics = false;

    static constexpr int HUD_BOX_H = 75;

    MinimapProvider m_minimapProvider;
};

#endif
