#include "ConfigManager.h"

ConfigManager &ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager()
{
    m_configPath = QCoreApplication::applicationDirPath() + "/game_settings.ini";
}

void ConfigManager::saveConfiguration()
{
    QSettings settings(m_configPath, QSettings::IniFormat);
    auto &kbm = KeyBindingManager::getInstance();

    settings.beginGroup("Controls");
    settings.setValue("MOVE_UP", static_cast<int>(kbm.getKey(KeyBindingManager::Action::MOVE_UP)));
    settings.setValue("MOVE_DOWN", static_cast<int>(kbm.getKey(KeyBindingManager::Action::MOVE_DOWN)));
    settings.setValue("STOP", static_cast<int>(kbm.getKey(KeyBindingManager::Action::STOP)));
    settings.setValue("GUARD", static_cast<int>(kbm.getKey(KeyBindingManager::Action::GUARD)));
    settings.setValue("SCATTER", static_cast<int>(kbm.getKey(KeyBindingManager::Action::SCATTER)));
    settings.endGroup();

    settings.beginGroup("General");
    settings.setValue("Language", m_cachedSettings.languageIndex);
    settings.setValue("Tooltips", m_cachedSettings.showTooltips);
    settings.setValue("Resolution", m_cachedSettings.resolutionIndex);
    settings.setValue("WindowMode", m_cachedSettings.windowModeIndex);
    settings.setValue("VSync", m_cachedSettings.vsync);
    settings.endGroup();

    settings.beginGroup("Graphics");
    settings.setValue("Textures", m_cachedSettings.textureQualityIndex);
    settings.setValue("Gamma", m_cachedSettings.gamma);
    settings.endGroup();

    settings.beginGroup("Audio");
    settings.setValue("Master", m_cachedSettings.masterVol);
    settings.setValue("Music", m_cachedSettings.musicVol);
    settings.setValue("SFX", m_cachedSettings.sfxVol);
    settings.setValue("Voice", m_cachedSettings.voiceVol);
    settings.endGroup();

    settings.sync();
}

void ConfigManager::loadConfiguration()
{

    QSettings settings(m_configPath, QSettings::IniFormat);
    auto &kbm = KeyBindingManager::getInstance();

    settings.beginGroup("Controls");
    auto loadKey = [&](const QString &key, KeyBindingManager::Action action, Input::KeyCode def)
    {
        int val = settings.value(key, static_cast<int>(def)).toInt();
        kbm.setKey(action, static_cast<Input::KeyCode>(val));
    };
    loadKey("MOVE_UP", KeyBindingManager::Action::MOVE_UP, Input::KeyCode::UP_ARROW);
    loadKey("MOVE_DOWN", KeyBindingManager::Action::MOVE_DOWN, Input::KeyCode::DOWN_ARROW);
    loadKey("STOP", KeyBindingManager::Action::STOP, Input::KeyCode::STOP);
    loadKey("GUARD", KeyBindingManager::Action::GUARD, Input::KeyCode::GUARD);
    loadKey("SCATTER", KeyBindingManager::Action::SCATTER, Input::KeyCode::SCATTER);
    settings.endGroup();

    settings.beginGroup("General");
    m_cachedSettings.languageIndex = settings.value("Language", 0).toInt();
    m_cachedSettings.showTooltips = settings.value("Tooltips", true).toBool();
    m_cachedSettings.resolutionIndex = settings.value("Resolution", 0).toInt();
    m_cachedSettings.windowModeIndex = settings.value("WindowMode", 0).toInt();
    m_cachedSettings.vsync = settings.value("VSync", true).toBool();
    settings.endGroup();

    settings.beginGroup("Graphics");
    m_cachedSettings.textureQualityIndex = settings.value("Textures", 2).toInt();
    m_cachedSettings.gamma = settings.value("Gamma", 50).toInt();
    settings.endGroup();

    settings.beginGroup("Audio");
    m_cachedSettings.masterVol = settings.value("Master", 75).toInt();
    m_cachedSettings.musicVol = settings.value("Music", 75).toInt();
    m_cachedSettings.sfxVol = settings.value("SFX", 75).toInt();
    m_cachedSettings.voiceVol = settings.value("Voice", 75).toInt();
    settings.endGroup();
}
