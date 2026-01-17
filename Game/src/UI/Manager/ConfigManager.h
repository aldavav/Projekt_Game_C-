#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <UI/Manager/GraphicsSettingsManager.h>
#include <UI/Manager/ControlsSettingsManager.h>
#include <UI/Manager/DisplaySettingsManager.h>
#include <UI/Manager/AudioSettingsManager.h>
#include <UI/Manager/GameSettingsManager.h>
#include <Core/Config/ConfigTypes.h>
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

    GameSettings &getSettings() { return m_cachedSettings; }

private:
    ConfigManager();

    QString m_configPath;

    GameSettings m_cachedSettings;
};

#endif
