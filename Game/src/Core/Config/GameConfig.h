#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QStringList>
#include <QString>

namespace GameConfig
{
    constexpr int GAMMA_MIN = 10;
    constexpr int GAMMA_MAX = 90;
    constexpr int GAMMA_DEFAULT = 50;

    constexpr int VOLUME_MIN = 0;
    constexpr int VOLUME_MAX = 100;

    const QStringList DIFFICULTIES = {"EASY", "NORMAL", "HARD", "BRUTAL"};

    const QStringList OPPONENTS = {
        "AI: NEURAL NETWORK",
        "HUMAN: UPLINK (NETWORK)"};

    constexpr int LOAD_STEP_RETRIEVE = 100;
    constexpr int LOAD_STEP_RECONSTRUCT = 200;

    const QString STATS_HEADER = "OPERATIONAL DATA:";
    const QString STATS_UNITS = "UNITS PRODUCED:";
    const QString STATS_LOSSES = "STRUCTURES LOST:";
    const QString STATS_TIME = "TOTAL TIME:";

    constexpr int FRAME_MS = 16;
    constexpr float DELTA_TIME = 0.016f;

    constexpr float CLOUD_MAX_RANGE = 4000.0f;
    constexpr float CLOUD_SPEED_BASE = 5.0f;
    constexpr float CLOUD_SPEED_MULT = 15.0f;

    constexpr int CULLING_MARGIN = 50;
    constexpr float HEX_VISUAL_SCALE = 0.98f;

    constexpr float CAMERA_BASE_SPEED = 15.0f;
    constexpr float ZOOM_STEP = 0.1f;
    constexpr float MIN_ZOOM = 0.5f;
    constexpr float MAX_ZOOM = 3.0f;

    constexpr int WORLD_BOUNDS = 50;
    constexpr float BASE_TILE_SIZE = 32.0f;
    constexpr int HEIGHT_OFFSET = 15;
    constexpr int VIEW_RADIUS = 25;
    constexpr int CLOUD_COUNT = 50;

    constexpr int MOUNTAIN_HEIGHT = 15;
    constexpr int GRASS_HEIGHT = 10;
    constexpr int DIRT_HEIGHT = 5;
    constexpr int WATER_HEIGHT = 0;

    constexpr float DAY_CYCLE_DURATION = 60.0f;

    constexpr float WATER_WAVE_SPEED = 2.0f;
    constexpr int WATER_BRIGHTNESS_BASE = 100;
    constexpr int WATER_BRIGHTNESS_SWING = 20;

    constexpr int MINIMAP_UPDATE_MS = 100;
    constexpr int MINIMAP_RANGE = 40;

    const QString COLOR_PHASE_NOON = "#FFD600";
    const QString COLOR_PHASE_AFTERNOON = "#FFA000";
    const QString COLOR_PHASE_EVENING = "#FF5722";
    const QString COLOR_PHASE_NIGHT = "#3F51B5";
    const QString COLOR_PHASE_MIDNIGHT = "#1A237E";

    constexpr int SCANLINE_SPACING = 3;
}

#endif
