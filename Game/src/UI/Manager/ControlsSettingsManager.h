#ifndef CONTROLSSETTINGSMANAGER_H
#define CONTROLSSETTINGSMANAGER_H

#include <Core/Common/KeyCodes.h>
#include <QKeySequence>
#include <QString>
#include <QMap>

class ControlsSettingsManager
{
public:
    static ControlsSettingsManager &getInstance();

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
    ControlsSettingsManager();
    
    ~ControlsSettingsManager() = default;

    ControlsSettingsManager(const ControlsSettingsManager &) = delete;
    
    ControlsSettingsManager &operator=(const ControlsSettingsManager &) = delete;

    QMap<Action, Key> m_bindings;
};

#endif
