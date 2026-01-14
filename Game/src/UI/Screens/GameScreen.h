#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <Core/Input/InputManager.h>
#include <UI/Manager/MenuManager.h>
#include <Game/Engine/GameEngine.h>
#include <UI/Screens/MenuScreen.h>
#include <Game/Map/Tile.h>
#include <Game/Map/Map.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>
#include <QTimer>

enum class GameSpeed
{
    SLOW,
    NORMAL,
    FAST
};

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

private slots:
    void updateGameDisplay();

    void onPauseClicked();

private:
    void drawMap(QPainter &painter);

    void drawHexagon(QPainter &painter, QPoint center, float radius);

    void drawSelectionBox(QPainter &painter);

    void drawHUD(QPainter &painter);

    void drawResourceStats(QPainter &painter);

    void drawMinimap(QPainter &painter);

    void drawDayNightCycle(QPainter &painter);

    QString getTileTypeName(TileType type);

    QTimer *m_updateTimer;

    QElapsedTimer m_fpsTimer;

    int m_frameCount = 0;

    int m_fps = 0;

    void updateFPS();

    float m_gameTime = 0.0f;

    float m_timeScale = 1.0f;

    GameSpeed m_currentSpeed = GameSpeed::NORMAL;

    bool m_isDragging = false;

    QPoint m_lastMousePos;

    QPointF m_hoveredHex;

    QPointF m_selectedHex = QPointF(9999, 9999);

    bool m_hasSelection = false;

    QPolygonF m_cachedHex;

    float m_lastRadius = 0.0f;

    QPixmap m_cloudTexture;

    bool m_isPaused = false;

    const int HUD_BOX_H = 75;
};

#endif
