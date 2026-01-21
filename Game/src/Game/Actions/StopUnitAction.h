#ifndef MOVEUNITACTIONS_H
#define MOVEUNITACTIONS_H

#include <Core/Common/ICommand.h>
#include <QPoint>

class StopUnitAction : public ICommand
{
public:
    StopUnitAction() : m_targetPos(0, 0) {}

    explicit StopUnitAction(const QPoint &targetPos)
        : m_targetPos(targetPos) {}

    bool execute(GameEngine &engine) override;

    bool undo() override;

    QString debugName() const override { return "StopAction"; }

private:
    QPoint m_targetPos;
};

#endif
