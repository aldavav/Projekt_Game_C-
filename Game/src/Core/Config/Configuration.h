#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>
#include <QString>
#include <QRect>
#include <QPoint>
#include <QColor>

namespace Config
{
    const QString GAME_TITLE = "Tiny Tiles Tactics";
    const QString VERSION = "1.0.0 Beta";
    constexpr int TARGET_FPS = 60;
    constexpr int TIMER_INTERVAL_MS = 16;

    struct PlayerSettings
    {
        int languageIndex = 1;
        bool showTooltips = true;
        bool legalAccepted = false;
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

    namespace Path
    {
        const QString ICON = ":/images/assets/images/icon.png";
        const QString CURSOR_DEFAULT = ":/images/assets/images/cursor.png";
        const QString BGM = "qrc:/audio/assets/audio/music.mp3";

    }

    namespace Simulation
    {
        constexpr float DELTA_TIME = 0.016f;
        constexpr float TICK_RATE = 1.0f / 30.0f;
        constexpr float SPEED_SLOW = 0.5f;
        constexpr float SPEED_NORMAL = 1.0f;
        constexpr float SPEED_FAST = 2.0f;
    }

    namespace World
    {
        constexpr int CHUNK_SIZE = 32;
        constexpr float BASE_TILE_SIZE = 32.0f;
        constexpr float NOISE_SCALE = 15.0f;
        constexpr float THRESH_WATER = 0.2f;
        constexpr float THRESH_GRASS = 0.65f;
    }
}

#endif
