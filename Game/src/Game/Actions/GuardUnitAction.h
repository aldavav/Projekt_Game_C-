#ifndef GUARDUNITACTIONS_H
#define GUARDUNITACTIONS_H

#include <Core/Common/ICommand.h>
#include <QPoint>

class GuardUnitAction : public ICommand
{
public:
    GuardUnitAction() : m_targetPos(0, 0) {}

    explicit GuardUnitAction(const QPoint &targetPos)
        : m_targetPos(targetPos) {}

    bool execute(GameEngine &engine) override;

    bool undo() override;

    QString debugName() const override { return "GuardAction"; }

private:
    QPoint m_targetPos;
};

#endif
