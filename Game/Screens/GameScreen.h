#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "../InputManager/InputManager.h"
#include "../MenuManager/MenuManager.h"
#include "../GameEngine/GameEngine.h"
#include "../Logger/LoggerMacros.h"
#include "../Map/Location.h"
#include "MainMenuScreen.h"
#include "../Map/Tile.h"
#include "../Map/Map.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QMessageBox>
#include <QWidget>
#include <QPainter>
#include <QTimer>

class GameScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);
    
    ~GameScreen() override;

    void onEnter() override;
    
    void onExit() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void updateGameDisplay();
    
    void onPauseClicked();

private:
    void drawMap(QPainter &painter);

    QTimer *m_updateTimer;
};

#endif
