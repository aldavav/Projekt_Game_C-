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

    settings.setValue("MAP_MODE", static_cast<int>(csm.getKey(Engine::Input::Action::ToggleMapMode)));
    settings.setValue("DEBUG_VIEW", static_cast<int>(csm.getKey(Engine::Input::Action::ToggleDebug)));
    settings.setValue("QUICK_SAVE", static_cast<int>(csm.getKey(Engine::Input::Action::QuickSave)));

    settings.setValue("ZOOM_IN", static_cast<int>(csm.getKey(Engine::Input::Action::ZoomIn)));
    settings.setValue("ZOOM_OUT", static_cast<int>(csm.getKey(Engine::Input::Action::ZoomOut)));
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

    loadKey("MOVE_UP", Engine::Input::Action::MoveUp, Engine::Input::KeyCode::W);
    loadKey("MOVE_DOWN", Engine::Input::Action::MoveDown, Engine::Input::KeyCode::S);
    loadKey("MOVE_LEFT", Engine::Input::Action::MoveLeft, Engine::Input::KeyCode::A);
    loadKey("MOVE_RIGHT", Engine::Input::Action::MoveRight, Engine::Input::KeyCode::D);

    loadKey("MAP_MODE", Engine::Input::Action::ToggleMapMode, Engine::Input::KeyCode::MapMode);
    loadKey("DEBUG_VIEW", Engine::Input::Action::ToggleDebug, Engine::Input::KeyCode::DebugF3);
    loadKey("QUICK_SAVE", Engine::Input::Action::QuickSave, Engine::Input::KeyCode::SaveF5);

    loadKey("ZOOM_IN", Engine::Input::Action::ZoomIn, Engine::Input::KeyCode::ZoomIn);
    loadKey("ZOOM_OUT", Engine::Input::Action::ZoomOut, Engine::Input::KeyCode::ZoomOut);
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

    csm.setKey(Engine::Input::Action::MoveUp, Engine::Input::KeyCode::W);
    csm.setKey(Engine::Input::Action::MoveDown, Engine::Input::KeyCode::S);
    csm.setKey(Engine::Input::Action::MoveLeft, Engine::Input::KeyCode::A);
    csm.setKey(Engine::Input::Action::MoveRight, Engine::Input::KeyCode::D);

    csm.setKey(Engine::Input::Action::ToggleMapMode, Engine::Input::KeyCode::MapMode);
    csm.setKey(Engine::Input::Action::ToggleDebug, Engine::Input::KeyCode::DebugF3);
    csm.setKey(Engine::Input::Action::QuickSave, Engine::Input::KeyCode::SaveF5);

    csm.setKey(Engine::Input::Action::ZoomIn, Engine::Input::KeyCode::ZoomIn);
    csm.setKey(Engine::Input::Action::ZoomOut, Engine::Input::KeyCode::ZoomOut);

    QString langCode = (m_cachedSettings.languageIndex == 1) ? "cz" : "en";
    GameSettingsManager::getInstance().setLanguage(langCode);
    GameSettingsManager::getInstance().setTooltipsEnabled(true);

    AudioSettingsManager::getInstance().volumesChanged();
    DisplaySettingsManager::getInstance().applySettings();
    GraphicsSettingsManager::getInstance().applyGraphicsSettings();

    saveConfiguration();
}
