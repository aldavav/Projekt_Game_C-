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
#include "Unit.h"

enum class PlacementState
{
    NONE,
    PLACING_UNIT
};

class GameScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr);
    
    ~GameScreen() override;

    void onEnter() override;
    
    void onExit() override;

    PlacementState m_placementState = PlacementState::NONE;
    UnitType m_pendingUnit = UnitType::NONE;

    bool isMoveMode() const { return m_moveMode; }
    Unit* getSelectedUnit() const { return m_selectedUnit; }
protected:
    void paintEvent(QPaintEvent *event) override;
    

private slots:
    void updateGameDisplay();
    
    void onPauseClicked();

    void onTileClicked(int x, int y);

    void tryMoveSelectedUnit(int x, int y);

private:
    void drawMap(QPainter &painter);

    QTimer *m_updateTimer;

    bool m_moveMode = false;
    Unit* m_selectedUnit = nullptr;
};

#endif
