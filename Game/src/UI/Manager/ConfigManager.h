#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <UI/Manager/KeyBindingManager.h>
#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QDir>

struct GameSettings
{
    int languageIndex = 0;
    bool showTooltips = true;
    int resolutionIndex = 0;
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

    GameSettings &getSettings() { return m_cachedSettings; }

private:
    ConfigManager();

    QString m_configPath;

    GameSettings m_cachedSettings;
};

#endif
