#include "ControlsSettingsManager.h"

ControlsSettingsManager &ControlsSettingsManager::getInstance()
{
    static ControlsSettingsManager instance;
    return instance;
}

void ControlsSettingsManager::setKey(Action action, Key key)
{
    m_bindings[action] = key;
}

ControlsSettingsManager::Key ControlsSettingsManager::getKey(Action action) const
{
    return m_bindings.value(action);
}

QString ControlsSettingsManager::getKeyName(Action action) const
{
    int qtKey = static_cast<int>(m_bindings.value(action));
    return QKeySequence(qtKey).toString();
}

ControlsSettingsManager::ControlsSettingsManager()
{
    m_bindings[Action::MOVE_UP] = Key::UP_ARROW;
    m_bindings[Action::MOVE_DOWN] = Key::DOWN_ARROW;
    m_bindings[Action::MOVE_LEFT] = Key::LEFT_ARROW;
    m_bindings[Action::MOVE_RIGHT] = Key::RIGHT_ARROW;
    m_bindings[Action::STOP] = Key::STOP;
    m_bindings[Action::GUARD] = Key::GUARD;
    m_bindings[Action::SCATTER] = Key::SCATTER;
}
