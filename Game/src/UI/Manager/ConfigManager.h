#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <UI/Manager/GraphicsSettingsManager.h>
#include <UI/Manager/ControlsSettingsManager.h>
#include <UI/Manager/DisplaySettingsManager.h>
#include <UI/Manager/AudioSettingsManager.h>
#include <UI/Manager/GameSettingsManager.h>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

struct GameSettings
{
    int languageIndex = 1;
    bool showTooltips = true;

    int resolutionIndex = 3;
    int windowModeIndex = 0;
    bool vsync = true;

    int textureQualityIndex = 2;
    int gamma = 50;

    int masterVol = 75;
    int musicVol = 75;
    int sfxVol = 75;
    int voiceVol = 75;
};

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
