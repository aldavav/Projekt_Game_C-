#include "AudioSettingsManager.h"

AudioSettingsManager &AudioSettingsManager::getInstance()
{
    static AudioSettingsManager instance;
    return instance;
}

float AudioSettingsManager::getFinalVolume(float channelVolume) const
{
    float master = ConfigManager::getInstance().getSettings().masterVol / 100.0f;
    return master * (channelVolume / 100.0f);
}

float AudioSettingsManager::getMusicVolume() const
{
    return getFinalVolume(ConfigManager::getInstance().getSettings().musicVol);
}

float AudioSettingsManager::getSFXVolume() const
{
    return getFinalVolume(ConfigManager::getInstance().getSettings().sfxVol);
}

float AudioSettingsManager::getVoiceVolume() const
{
    return getFinalVolume(ConfigManager::getInstance().getSettings().voiceVol);
}
