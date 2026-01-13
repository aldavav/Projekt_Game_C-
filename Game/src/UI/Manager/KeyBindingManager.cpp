#include "KeyBindingManager.h"

KeyBindingManager &KeyBindingManager::getInstance()
{
    static KeyBindingManager instance;
    return instance;
}

void KeyBindingManager::setKey(Action action, Key key)
{
    m_bindings[action] = key;
}

KeyBindingManager::Key KeyBindingManager::getKey(Action action) const
{
    return m_bindings.value(action);
}

QString KeyBindingManager::getKeyName(Action action) const
{
    int qtKey = static_cast<int>(m_bindings.value(action));
    return QKeySequence(qtKey).toString();
}

KeyBindingManager::KeyBindingManager()
{
    m_bindings[Action::MOVE_UP] = Key::UP_ARROW;
    m_bindings[Action::MOVE_DOWN] = Key::DOWN_ARROW;
    m_bindings[Action::MOVE_LEFT] = Key::LEFT_ARROW;
    m_bindings[Action::MOVE_RIGHT] = Key::RIGHT_ARROW;
    m_bindings[Action::STOP] = Key::STOP;
    m_bindings[Action::GUARD] = Key::GUARD;
    m_bindings[Action::SCATTER] = Key::SCATTER;
}
