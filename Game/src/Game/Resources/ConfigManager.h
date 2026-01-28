#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <Core/Settings/GraphicsSettingsManager.h>
#include <Core/Settings/ControlsSettingsManager.h>
#include <Core/Settings/DisplaySettingsManager.h>
#include <Core/Settings/AudioSettingsManager.h>
#include <Core/Settings/GameSettingsManager.h>
#include <Core/Config/Configuration.h>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

class ConfigManager
{
public:
    static ConfigManager &getInstance();

    void saveConfiguration();

    void loadConfiguration();

    void resetToDefaults();

    Config::System::PlayerSettings &getSettings() { return m_cachedSettings; }

    ConfigManager(const ConfigManager &) = delete;

    ConfigManager &operator=(const ConfigManager &) = delete;

    ConfigManager(ConfigManager &&other) noexcept = default;

    ConfigManager &operator=(ConfigManager &&other) noexcept = default;

private:
    ConfigManager();

    QString m_configPath = nullptr;

    Config::System::PlayerSettings m_cachedSettings;
};

#endif
