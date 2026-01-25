#ifndef SCATTERUNITACTIONS_H
#define SCATTERUNITACTIONS_H

#include <Core/Common/ICommand.h>
#include <QPoint>

class ScatterUnitAction : public ICommand
{
public:
    ScatterUnitAction(uint32_t unitId, const QPoint &target);

    bool execute(GameEngine &engine) override;

    QString name() const override;

    QString description() const override;

private:
    uint32_t m_unitId;

    QPoint m_target;
};

#endif
