#ifndef KEYBINDINGMANAGER_H
#define KEYBINDINGMANAGER_H

#include <Core/Common/KeyCodes.h>
#include <QKeySequence>
#include <QString>
#include <QMap>

class KeyBindingManager
{
public:
    static KeyBindingManager &getInstance();

    using Key = Input::KeyCode;

    enum class Action
    {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        STOP,
        GUARD,
        SCATTER
    };

    void setKey(Action action, Key key);
    
    Key getKey(Action action) const;
    
    QString getKeyName(Action action) const;

private:
    KeyBindingManager();
    
    ~KeyBindingManager() = default;

    KeyBindingManager(const KeyBindingManager &) = delete;
    
    KeyBindingManager &operator=(const KeyBindingManager &) = delete;

    QMap<Action, Key> m_bindings;
};

#endif
