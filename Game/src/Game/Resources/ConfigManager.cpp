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
    auto &csm = ControlsSettingsManager::getInstance();

    settings.beginGroup("Game");
    settings.setValue("Language", m_cachedSettings.languageIndex);
    settings.setValue("Tooltips", m_cachedSettings.showTooltips);
    settings.setValue("LegalAccepted", m_cachedSettings.legalAccepted);
    settings.endGroup();

    settings.beginGroup("Controls");
    settings.setValue("MOVE_UP", static_cast<int>(csm.getKey(Engine::Input::Action::MOVE_UP)));
    settings.setValue("MOVE_DOWN", static_cast<int>(csm.getKey(Engine::Input::Action::MOVE_DOWN)));
    settings.setValue("MOVE_LEFT", static_cast<int>(csm.getKey(Engine::Input::Action::MOVE_LEFT)));
    settings.setValue("MOVE_RIGHT", static_cast<int>(csm.getKey(Engine::Input::Action::MOVE_RIGHT)));
    settings.setValue("STOP", static_cast<int>(csm.getKey(Engine::Input::Action::STOP)));
    settings.setValue("GUARD", static_cast<int>(csm.getKey(Engine::Input::Action::GUARD)));
    settings.setValue("SCATTER", static_cast<int>(csm.getKey(Engine::Input::Action::SCATTER)));
    settings.endGroup();

    settings.beginGroup("Display");
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
    auto &csm = ControlsSettingsManager::getInstance();

    settings.beginGroup("Game");
    m_cachedSettings.languageIndex = settings.value("Language", 0).toInt();
    m_cachedSettings.showTooltips = settings.value("Tooltips", true).toBool();
    m_cachedSettings.legalAccepted = settings.value("LegalAccepted", false).toBool();
    settings.endGroup();

    settings.beginGroup("Controls");
    auto loadKey = [&](const QString &key, Engine::Input::Action action, Engine::Input::KeyCode def)
    {
        int val = settings.value(key, static_cast<int>(def)).toInt();
        csm.setKey(action, static_cast<Engine::Input::KeyCode>(val));
    };
    loadKey("MOVE_UP", Engine::Input::Action::MOVE_UP, Engine::Input::KeyCode::UP_ARROW);
    loadKey("MOVE_DOWN", Engine::Input::Action::MOVE_DOWN, Engine::Input::KeyCode::DOWN_ARROW);
    loadKey("MOVE_LEFT", Engine::Input::Action::MOVE_LEFT, Engine::Input::KeyCode::LEFT_ARROW);
    loadKey("MOVE_RIGHT", Engine::Input::Action::MOVE_RIGHT, Engine::Input::KeyCode::RIGHT_ARROW);
    loadKey("STOP", Engine::Input::Action::STOP, Engine::Input::KeyCode::STOP);
    loadKey("GUARD", Engine::Input::Action::GUARD, Engine::Input::KeyCode::GUARD);
    loadKey("SCATTER", Engine::Input::Action::SCATTER, Engine::Input::KeyCode::SCATTER);
    settings.endGroup();

    settings.beginGroup("Display");
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

void ConfigManager::resetToDefaults()
{
    auto &csm = ControlsSettingsManager::getInstance();
    csm.setKey(Engine::Input::Action::MOVE_UP, Engine::Input::KeyCode::UP_ARROW);
    csm.setKey(Engine::Input::Action::MOVE_DOWN, Engine::Input::KeyCode::DOWN_ARROW);
    csm.setKey(Engine::Input::Action::MOVE_LEFT, Engine::Input::KeyCode::LEFT_ARROW);
    csm.setKey(Engine::Input::Action::MOVE_RIGHT, Engine::Input::KeyCode::RIGHT_ARROW);
    csm.setKey(Engine::Input::Action::STOP, Engine::Input::KeyCode::STOP);
    csm.setKey(Engine::Input::Action::GUARD, Engine::Input::KeyCode::GUARD);
    csm.setKey(Engine::Input::Action::SCATTER, Engine::Input::KeyCode::SCATTER);

    QString langCode = (m_cachedSettings.languageIndex == 1) ? "cz" : "en";
    GameSettingsManager::getInstance().setLanguage(langCode);
    GameSettingsManager::getInstance().setTooltipsEnabled(m_cachedSettings.showTooltips);

    AudioSettingsManager::getInstance().volumesChanged();
    DisplaySettingsManager::getInstance().applySettings();
    GraphicsSettingsManager::getInstance().applyGraphicsSettings();

    saveConfiguration();
}
