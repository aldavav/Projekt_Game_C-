#ifndef CONTROLSSETTINGSMANAGER_H
#define CONTROLSSETTINGSMANAGER_H

#include <Core/Config/ControlTypes.h>
#include <QKeySequence>
#include <QString>
#include <QMap>

class ControlsSettingsManager
{
public:
    static ControlsSettingsManager &getInstance();

    void setKey(Controls::Action action, Controls::Key key);

    Controls::Key getKey(Controls::Action action) const;

    QString getKeyName(Controls::Action action) const;

private:
    ControlsSettingsManager();
    
    ~ControlsSettingsManager() = default;

    ControlsSettingsManager(const ControlsSettingsManager &) = delete;
    
    ControlsSettingsManager &operator=(const ControlsSettingsManager &) = delete;

    QMap<Controls::Action, Controls::Key> m_bindings;
};

#endif
