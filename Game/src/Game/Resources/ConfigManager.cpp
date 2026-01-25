#include "ConfigManager.h"

ConfigManager &ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager()
{
    m_configPath = QCoreApplication::applicationDirPath() + "/" + Config::System::CONFIG_FILE_NAME;
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
    settings.setValue("MOVE_UP", static_cast<int>(csm.getKey(Engine::Input::Action::MoveUp)));
    settings.setValue("MOVE_DOWN", static_cast<int>(csm.getKey(Engine::Input::Action::MoveDown)));
    settings.setValue("MOVE_LEFT", static_cast<int>(csm.getKey(Engine::Input::Action::MoveLeft)));
    settings.setValue("MOVE_RIGHT", static_cast<int>(csm.getKey(Engine::Input::Action::MoveRight)));
    settings.setValue("STOP", static_cast<int>(csm.getKey(Engine::Input::Action::Stop)));
    settings.setValue("GUARD", static_cast<int>(csm.getKey(Engine::Input::Action::Guard)));
    settings.setValue("SCATTER", static_cast<int>(csm.getKey(Engine::Input::Action::Scatter)));
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
    loadKey("MOVE_UP", Engine::Input::Action::MoveUp, Engine::Input::KeyCode::UpArrow);
    loadKey("MOVE_DOWN", Engine::Input::Action::MoveDown, Engine::Input::KeyCode::DownArrow);
    loadKey("MOVE_LEFT", Engine::Input::Action::MoveLeft, Engine::Input::KeyCode::LeftArrow);
    loadKey("MOVE_RIGHT", Engine::Input::Action::MoveRight, Engine::Input::KeyCode::RightArrow);
    loadKey("STOP", Engine::Input::Action::Stop, Engine::Input::KeyCode::Stop);
    loadKey("GUARD", Engine::Input::Action::Guard, Engine::Input::KeyCode::Guard);
    loadKey("SCATTER", Engine::Input::Action::Scatter, Engine::Input::KeyCode::Scatter);
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
    csm.setKey(Engine::Input::Action::MoveUp, Engine::Input::KeyCode::UpArrow);
    csm.setKey(Engine::Input::Action::MoveDown, Engine::Input::KeyCode::DownArrow);
    csm.setKey(Engine::Input::Action::MoveLeft, Engine::Input::KeyCode::LeftArrow);
    csm.setKey(Engine::Input::Action::MoveRight, Engine::Input::KeyCode::RightArrow);
    csm.setKey(Engine::Input::Action::Stop, Engine::Input::KeyCode::Stop);
    csm.setKey(Engine::Input::Action::Guard, Engine::Input::KeyCode::Guard);
    csm.setKey(Engine::Input::Action::Scatter, Engine::Input::KeyCode::Scatter);

    QString langCode = (m_cachedSettings.languageIndex == 1) ? "cz" : "en";
    GameSettingsManager::getInstance().setLanguage(langCode);
    GameSettingsManager::getInstance().setTooltipsEnabled(m_cachedSettings.showTooltips);

    AudioSettingsManager::getInstance().volumesChanged();
    DisplaySettingsManager::getInstance().applySettings();
    GraphicsSettingsManager::getInstance().applyGraphicsSettings();

    saveConfiguration();
}
