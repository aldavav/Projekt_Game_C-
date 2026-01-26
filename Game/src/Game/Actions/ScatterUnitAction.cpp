#include "ScatterUnitAction.h"

ScatterUnitAction::ScatterUnitAction(uint32_t unitId, const QPoint &target)
    : m_unitId(unitId), m_target(target) {}

bool ScatterUnitAction::execute(GameEngine &engine)
{
    return true;
}

QString ScatterUnitAction::name() const
{
    return "StopUnit";
}

QString ScatterUnitAction::description() const
{
    return QString("Unit %1 to %2,%3").arg(m_unitId).arg(m_target.x()).arg(m_target.y());
}
