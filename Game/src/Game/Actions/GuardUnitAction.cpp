#include "GuardUnitAction.h"

GuardUnitAction::GuardUnitAction(uint32_t unitId, const QPoint &target)
    : m_unitId(unitId), m_target(target) {}

bool GuardUnitAction::execute(GameEngine &engine)
{
    return true;
}

QString GuardUnitAction::name() const
{
    return "GuardUnit";
}

QString GuardUnitAction::description() const
{
    return QString("Unit %1 to %2,%3").arg(m_unitId).arg(m_target.x()).arg(m_target.y());
}
