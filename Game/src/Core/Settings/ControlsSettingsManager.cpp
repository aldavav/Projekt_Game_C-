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
    m_bindings[Engine::Input::Action::MoveUp] = Engine::Input::KeyCode::UpArrow;
    m_bindings[Engine::Input::Action::MoveDown] = Engine::Input::KeyCode::DownArrow;
    m_bindings[Engine::Input::Action::MoveLeft] = Engine::Input::KeyCode::LeftArrow;
    m_bindings[Engine::Input::Action::MoveRight] = Engine::Input::KeyCode::RightArrow;
    m_bindings[Engine::Input::Action::Stop] = Engine::Input::KeyCode::Stop;
    m_bindings[Engine::Input::Action::Guard] = Engine::Input::KeyCode::Guard;
    m_bindings[Engine::Input::Action::Scatter] = Engine::Input::KeyCode::Scatter;
}

Engine::Input::Action ControlsSettingsManager::getActionForKey(int qtKey) const
{
    auto it = m_bindings.begin();
    while (it != m_bindings.end())
    {
        if (static_cast<int>(it.value()) == qtKey)
        {
            return it.key();
        }
        ++it;
    }

    return static_cast<Engine::Input::Action>(-1);
}
