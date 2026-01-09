#ifndef UNITACTIONS_H
#define UNITACTIONS_H

#include <Game/Actions/GameActions.h>
#include <Game/View/Camera.h>
#include <QPoint>

class MoveUnitAction : public GameAction
{
public:
    MoveUnitAction(const QPoint& targetTile) : m_target(targetTile) {}

    bool execute(GameEngine& engine) override;

    bool undo() override;
    
    QString debugName() const override { return "MoveUnitAction"; }

private:
    QPoint m_target;
};

class StopAction : public GameAction
{
public:
    bool execute(GameEngine& engine) override;

    bool undo() override;

    QString debugName() const override { return "StopAction"; }
};

#endif
