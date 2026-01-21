#ifndef UNITACTIONS_H
#define UNITACTIONS_H

#include <Core/Common/ICommand.h>
#include <QPoint>

class MoveUnitAction : public ICommand
{
public:
    MoveUnitAction() : m_targetPos(0, 0) {}

    explicit MoveUnitAction(const QPoint &targetPos)
        : m_targetPos(targetPos) {}

    bool execute(GameEngine &engine) override;

    bool undo() override;

    QString debugName() const override { return "MoveAction"; }

private:
    QPoint m_targetPos;
};

#endif
