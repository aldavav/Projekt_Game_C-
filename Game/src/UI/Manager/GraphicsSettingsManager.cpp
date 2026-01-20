#include "GraphicsSettingsManager.h"

GraphicsSettingsManager &GraphicsSettingsManager::getInstance()
{
    static GraphicsSettingsManager instance;
    return instance;
}

void GraphicsSettingsManager::applyGraphicsSettings()
{
    auto &cfg = ConfigManager::getInstance().getSettings();
    emit graphicsUpdated();
}

float GraphicsSettingsManager::getGammaValue() const
{
    auto &cfg = ConfigManager::getInstance().getSettings();
    return cfg.gamma / GameConfig::GAMMA_DEFAULT;
}
