#include "ControlsSettingsManager.h"

ControlsSettingsManager &ControlsSettingsManager::getInstance()
{
    static ControlsSettingsManager instance;
    return instance;
}

void ControlsSettingsManager::setKey(Engine::Input::Action action, Engine::Input::KeyCode key)
{
    m_bindings[action] = key;
}

Engine::Input::KeyCode ControlsSettingsManager::getKey(Engine::Input::Action action) const
{
    return m_bindings.value(action);
}

QString ControlsSettingsManager::getKeyName(Engine::Input::Action action) const
{
    int qtKey = static_cast<int>(m_bindings.value(action));
    return QKeySequence(qtKey).toString();
}

ControlsSettingsManager::ControlsSettingsManager()
{
    m_bindings[Engine::Input::Action::MOVE_UP] = Engine::Input::KeyCode::UP_ARROW;
    m_bindings[Engine::Input::Action::MOVE_DOWN] = Engine::Input::KeyCode::DOWN_ARROW;
    m_bindings[Engine::Input::Action::MOVE_LEFT] = Engine::Input::KeyCode::LEFT_ARROW;
    m_bindings[Engine::Input::Action::MOVE_RIGHT] = Engine::Input::KeyCode::RIGHT_ARROW;
    m_bindings[Engine::Input::Action::STOP] = Engine::Input::KeyCode::STOP;
    m_bindings[Engine::Input::Action::GUARD] = Engine::Input::KeyCode::GUARD;
    m_bindings[Engine::Input::Action::SCATTER] = Engine::Input::KeyCode::SCATTER;
}
