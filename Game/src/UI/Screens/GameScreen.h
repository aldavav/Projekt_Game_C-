#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <UI/Screens/MainMenuScreen.h>
#include <Core/Logger/LoggerMacros.h>
#include <Core/Input/InputManager.h>
#include <UI/Manager/MenuManager.h>
#include <Game/Engine/GameEngine.h>
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

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void updateGameDisplay();

    void onPauseClicked();

    void resizeEvent(QResizeEvent *event) override;

private:
    bool m_isDragging = false;

    QPoint m_lastMousePos;

    void wheelEvent(QWheelEvent *event);

    void drawMap(QPainter &painter);

    QTimer *m_updateTimer;
};

#endif
