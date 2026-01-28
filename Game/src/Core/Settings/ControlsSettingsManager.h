#ifndef CONTROLSSETTINGSMANAGER_H
#define CONTROLSSETTINGSMANAGER_H

#include <Core/Config/Configuration.h>
#include <QKeySequence>
#include <QObject>
#include <QString>
#include <QMap>

class ControlsSettingsManager : public QObject
{
    Q_OBJECT

public:
    static ControlsSettingsManager &getInstance();

    void setKey(Engine::Input::Action action, Engine::Input::KeyCode key);

    Engine::Input::Action getActionForKey(int qtKey) const;

    Engine::Input::KeyCode getKey(Engine::Input::Action action) const;

    QString getKeyName(Engine::Input::Action action) const;

    ControlsSettingsManager(const ControlsSettingsManager &) = delete;

    ControlsSettingsManager &operator=(const ControlsSettingsManager &) = delete;

    ControlsSettingsManager(ControlsSettingsManager &&other) noexcept = delete;

    ControlsSettingsManager &operator=(ControlsSettingsManager &&other) noexcept = delete;

private:
    ControlsSettingsManager();

    QMap<Engine::Input::Action, Engine::Input::KeyCode> m_bindings;
};

#endif
