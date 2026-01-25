#include "StopUnitAction.h"

StopUnitAction::StopUnitAction(uint32_t unitId, const QPoint &target)
    : m_unitId(unitId), m_target(target) {}

bool StopUnitAction::execute(GameEngine &engine)
{
    return true;
}

QString StopUnitAction::name() const
{
    return "StopUnit";
}

QString StopUnitAction::description() const
{
    return QString("Unit %1 to %2,%3").arg(m_unitId).arg(m_target.x()).arg(m_target.y());
}
