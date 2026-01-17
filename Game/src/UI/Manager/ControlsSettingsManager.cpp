#include "ControlsSettingsManager.h"

ControlsSettingsManager &ControlsSettingsManager::getInstance()
{
    static ControlsSettingsManager instance;
    return instance;
}

void ControlsSettingsManager::setKey(Controls::Action action, Controls::Key key)
{
    m_bindings[action] = key;
}

Controls::Key ControlsSettingsManager::getKey(Controls::Action action) const
{
    return m_bindings.value(action);
}

QString ControlsSettingsManager::getKeyName(Controls::Action action) const
{
    int qtKey = static_cast<int>(m_bindings.value(action));
    return QKeySequence(qtKey).toString();
}

ControlsSettingsManager::ControlsSettingsManager()
{
    m_bindings[Controls::Action::MOVE_UP] = Controls::Key::UP_ARROW;
    m_bindings[Controls::Action::MOVE_DOWN] = Controls::Key::DOWN_ARROW;
    m_bindings[Controls::Action::MOVE_LEFT] = Controls::Key::LEFT_ARROW;
    m_bindings[Controls::Action::MOVE_RIGHT] = Controls::Key::RIGHT_ARROW;
    m_bindings[Controls::Action::STOP] = Controls::Key::STOP;
    m_bindings[Controls::Action::GUARD] = Controls::Key::GUARD;
    m_bindings[Controls::Action::SCATTER] = Controls::Key::SCATTER;
}
