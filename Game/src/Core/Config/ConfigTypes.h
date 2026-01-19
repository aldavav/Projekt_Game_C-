#ifndef CONFIGTYPES_H
#define CONFIGTYPES_H

#include <Core/Config/GameConfig.h>

struct GameSettings
{
    int languageIndex = 1;
    bool showTooltips = true;
    bool legalAccepted = false;

    int resolutionIndex = 3;
    int windowModeIndex = 0;
    bool vsync = true;

    int textureQualityIndex = 2;
    int gamma = GameConfig::GAMMA_DEFAULT;

    int masterVol = 75;
    int musicVol = 75;
    int sfxVol = 75;
    int voiceVol = 75;
};

#endif
