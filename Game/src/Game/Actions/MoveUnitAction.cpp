#include "MoveUnitAction.h"

MoveUnitAction::MoveUnitAction(uint32_t unitId, const QPoint &target)
    : m_unitId(unitId), m_target(target) {}

bool MoveUnitAction::execute(GameEngine &engine)
{
    return true;
}

QString MoveUnitAction::name() const
{
    return "MoverUnit";
}

QString MoveUnitAction::description() const
{
    return QString("Unit %1 to %2,%3").arg(m_unitId).arg(m_target.x()).arg(m_target.y());
}
