#ifndef UNITACTIONS_H
#define UNITACTIONS_H

#include <Core/Config/GameActions.h>
#include <QPoint>

class MoveUnitAction : public GameAction
{
public:
    MoveUnitAction(const QPoint& targetTile) : m_target(targetTile) {}

    bool execute(GameEngine& engine) override;

    bool undo() override;

private:
    QPoint m_target;
};

#endif
