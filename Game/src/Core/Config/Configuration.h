#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Core/Common/GameTypes.h>
#include <QStringList>
#include <QString>
#include <QPoint>
#include <QColor>
#include <string>
#include <QRect>
#include <QPair>

namespace Config
{
    namespace System
    {
        const QString GAME_TITLE = "Tiny Tiles Tactics";
        const QString VERSION = "1.0.0 Beta";
        const QString CONFIG_FILE_NAME = "settings.ini";
        constexpr int TIMER_INTERVAL_MS = 16;
        constexpr float MAX_FRAME_TIME = 0.1f;
        const QList<QPair<QString, QString>> LANGUAGES = {
            {"English", "en"},
            {"Čeština", "cz"}};
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
    }

    namespace Input
    {
        constexpr int KEY_ZOOM_IN = Qt::Key_Plus;
        constexpr int KEY_ZOOM_IN_ALT = Qt::Key_Equal;
        constexpr int KEY_ZOOM_OUT = Qt::Key_Minus;
        constexpr Qt::MouseButton BTN_MOVE = Qt::LeftButton;
        constexpr int KEYBOARD_SCROLL_STEP = 20;
    }

    namespace Audio
    {
        constexpr int VOLUME_DEFAULT = 75;
        const QString PATH_BGM = "qrc:/audio/assets/audio/bgm/main_menu.wav";
        const QString PATH_BGM_ALT = "qrc:/audio/assets/audio/music.mp3";
        const QString PATH_BUTTON_SFX = "qrc:/audio/assets/audio/sfx/button_click.wav";
    }

    namespace Entities
    {
        constexpr float DEFAULT_SPEED = 150.0f;
        constexpr float ROTATION_SPEED = 5.0f;
        constexpr float MIN_MOVE_DIST_SQ = 1.0f;
    }

    namespace Gameplay
    {
        constexpr float ZOOM_STEP = 0.1f;
        constexpr float INITIAL_ZOOM = 5.0f;
        constexpr Engine::Difficulty DEFAULT_DIFFICULTY = Engine::Difficulty::Medium;
        const QString TEXT_VICTORY = "VICTORY ACHIEVED";
        const QString TEXT_DEFEAT = "DEFEAT: SIGNAL LOST";
        const QString DEFAULT_MISSION_NAME = "Sector_7";
        constexpr int DEFAULT_WINDOW_WIDTH = 1920;
        constexpr int DEFAULT_WINDOW_HEIGHT = 1080;
        constexpr int GAMMA_DEFAULT = 50;
        constexpr int GAMMA_MIN = 0;
        constexpr int GAMMA_MAX = 100;
        constexpr int VOLUME_MIN = 0;
        constexpr int VOLUME_MAX = 100;
    }

    namespace Log
    {
        const size_t MAX_ARCHIVE_FILES = 10;
        const std::string DIRECTORY = "logs";
        const std::string LATEST_FILE = "latest.log";
        const std::string ARCHIVE_FILE_PREFIX = "log_";
        const std::string RESET = "\033[0m";
        const std::string RED_BOLD = "\033[1;31m";
        const std::string RED_NORMAL = "\033[0;31m";
        const std::string YELLOW_BOLD = "\033[1;33m";
        const std::string GREEN_NORMAL = "\033[0;32m";
    }

    namespace Paths
    {
        const QString SAVE_DIR_NAME = "/saves";
        const QString INITIAL_SAVE_FILENAME = "autosave_01.dat";
        const QString LANGUAGE_FILE = ":/i18n/assets/i18n/game_%1.qm";
        const QString STYLE_SHEET = ":/styles/assets/styles/style.qss";
        const QString CURSOR_DEFAULT = ":/ui/assets/ui/cursor_std.png";
        const QString CURSOR_SELECT = ":/ui/assets/ui/cursor_sel.png";
        const QString CURSOR_ATTACK = ":/ui/assets/ui/cursor_atk.png";
        const QString CURSOR_WAIT = ":/ui/assets/ui/cursor_wait.png";
        const QString ICON = ":/images/assets/images/icon.png";
        const QString MISSING_TEXTURE = ":/images/assets/images/missing.png";
        const QString CLOUD_TEXTURE = ":/images/assets/images/cloud.png";
        const QString EULA = ":/legal/assets/legal/EULA/EULA.html";
        const QString TOS = ":/legal/assets/legal/ToS/ToS.html";
        const QString BUTTON_BG = ":/images/assets/images/button.png";
        const QString ARROW_ICON = ":/images/assets/images/arrow.png";
        const QString PATH_MENU_BG = ":/animations/assets/animations/menu.gif";
        const QStringList PATHS_FONTS = {
            ":/fonts/assets/fonts/Orbitron-Black.ttf",
            ":/fonts/assets/fonts/Orbitron-Bold.ttf",
            ":/fonts/assets/fonts/Orbitron-ExtraBold.ttf",
            ":/fonts/assets/fonts/Orbitron-Medium.ttf",
            ":/fonts/assets/fonts/Orbitron-Regular.ttf",
            ":/fonts/assets/fonts/Orbitron-SemiBold.ttf"};
    }

    namespace Simulation
    {
        constexpr float DELTA_TIME = 0.016f;
        constexpr float TICK_RATE = 1.0f / 30.0f;
        constexpr float SPEED_SLOW = 0.5f;
        constexpr float SPEED_NORMAL = 1.0f;
        constexpr float SPEED_FAST = 2.0f;
    }

    namespace Strings
    {
        const QString LEAD_DEV = "Michael Szotkowski";
        const QStringList PROGRAMMING_TEAM = {LEAD_DEV, "Aleš Vavřinec"};
        const QStringList CONTRIBUTIONS = {"Marek Hovorka"};
        const QString DEFAULT_UPLINK_TEXT = "ESTABLISHING UPLINK...";
        const QString DEFAULT_STATUS_TEXT = "INITIALIZING ENGINE...";
    }

    namespace UI
    {
        constexpr int SIDE_STRETCH = 1;
        constexpr int CONTENT_STRETCH = 8;
        constexpr int SETTINGS_SIDE_STRETCH = 1;
        constexpr int SETTINGS_CONTENT_STRETCH = 8;
        constexpr int LIST_SCREEN_MARGIN_H = 100;
        constexpr int LIST_SCREEN_MARGIN_V = 50;
        constexpr int BTN_WIDTH_NORMAL = 250;
        constexpr int BTN_HEIGHT_NORMAL = 70;
        constexpr int BTN_WIDTH_HOVER = 265;
        constexpr int BTN_HEIGHT_HOVER = 75;
        constexpr int BTN_X_NORMAL = 100;
        constexpr int BTN_X_HOVER = 92;
        constexpr int BG_ANIM_SPEED = 50;
        constexpr int MENU_LEFT_MARGIN = 200;
        constexpr int GENERATION_DELAY = 500;
        constexpr int ARCHIVE_DELAY = 1000;
        constexpr int LOADING_BAR_WIDTH = 560;
        constexpr int LOADING_BAR_HEIGHT = 30;
        constexpr int LOADING_STEP_DELAY = 200;
        constexpr int LOADING_FINAL_PAUSE = 300;
        const QString FONT_FAMILY = "Orbitron";
        const QString COLOR_TACTICAL_BLUE = "#4FC3F7";
        const QString COLOR_HABITABLE = "#81C784";
        const QString COLOR_WATER_INFO = "#64B5F6";
        const QString COLOR_WATER = "#1976D2";
        const QString COLOR_GRASS = "#388E3C";
        const QString COLOR_MOUNTAIN = "#757575";
        const QString COLOR_DIRT = "#795548";
        const QString COLOR_UNKNOWN = "#555555";
        const QString COLOR_PHASE_NOON = "#FFD600";
        const QString COLOR_PHASE_AFTERNOON = "#FFA000";
        const QString COLOR_PHASE_EVENING = "#FF5722";
        const QString COLOR_PHASE_NIGHT = "#3F51B5";
        const QString COLOR_PHASE_MIDNIGHT = "#1A237E";
        constexpr int CURSOR_SIZE = 32;
        constexpr int HUD_MARGIN = 15;
        constexpr int HUD_MINIMAP_SIZE = 150;
        constexpr int HUD_BOX_WIDTH = 220;
        constexpr int HUD_STATS_WIDTH = 180;
        constexpr int DIALOG_WIDTH = 400;
        constexpr int DIALOG_HEIGHT = 200;
        constexpr int INFO_DIALOG_WIDTH = 650;
        constexpr int INFO_DIALOG_HEIGHT = 550;
        constexpr int KEY_DIALOG_WIDTH = 300;
        constexpr int KEY_DIALOG_HEIGHT = 150;
        constexpr int KEY_DIALOG_MARGINS = 20;
        constexpr int END_SCREEN_BTN_WIDTH = 300;
        constexpr int END_SCREEN_BTN_HEIGHT = 50;
        constexpr int END_SCREEN_SPACING = 30;
        constexpr float LEGAL_SCROLL_THRESHOLD = 0.95f;
        const QString STR_ABORT = "ABORT";
        const QString STR_PROCEED = "PROCEED";
        const QString MANUAL_TITLE = "SYSTEM MANUAL";
        const QString MANUAL_TEXT = R"(CONTROLS:
 - Left Click: Select unit / structure / hex
 - Left Click Drag: Box select multiple units
 - Right Click: Issue command / move / interact
 - Right Click Drag: Pan camera
 - Scroll Wheel: Zoom in / out

COMMANDS:
 - Attack: Right click on enemy target
 - Move: Right click on terrain
 - Stop: Cancel current order

STRATEGIC OBJECTIVE:
 - Explore fogged areas to reveal the battlefield
 - Secure resources to expand your forces)";
        const QString STATS_HEADER = "OPERATIONAL DATA:";
        const QString STATS_UNITS = "UNITS PRODUCED:";
        const QString STATS_LOSSES = "STRUCTURES LOST:";
        const QString STATS_TIME = "TOTAL TIME:";
        constexpr int LOAD_STEP_RETRIEVE = 100;
        constexpr int LOAD_STEP_RECONSTRUCT = 200;
        const QStringList DIFFICULTIES = {"EASY", "NORMAL", "HARD", "BRUTAL"};
        const QStringList OPPONENTS = {
            "AI: NEURAL NETWORK",
            "HUMAN: UPLINK (NETWORK)"};
        const QStringList MAP_TYPES = {
            "SINGLE ISLAND",
            "ARCHIPELAGO",
            "CONTINENTS",
            "PANGEA"};
    }

    namespace World
    {
        const QRect WORLD_BOUNDS = QRect(-2000, -1500, 3500, 3000);
        constexpr uint32_t DEFAULT_SEED = 123456789;
        constexpr bool DEFAULT_3D_VIEW = true;
        constexpr float BASE_TILE_SIZE = 32.0f;
        constexpr int CHUNK_SIZE = 32;
        constexpr float NOISE_SCALE = 15.0f;
        constexpr float THRESH_WATER = 0.2f;
        constexpr float THRESH_DIRT = 0.3f;
        constexpr float THRESH_GRASS = 0.65f;
        constexpr float HEIGHT_BIAS = 0.1f;
        constexpr int HEIGHT_OFFSET = 15;
        constexpr int REVEAL_RADIUS = 10;
        constexpr float CAMERA_BASE_SPEED = 25.0f;
        constexpr float CAMERA_SMOOTHING = 0.1f;
        constexpr float CAMERA_KEYBOARD_SPEED = 800.0f;
        constexpr float EDGE_PAN_SPEED = 500.0f;
        constexpr int EDGE_MARGIN = 30;
        constexpr float MAX_OUT_ZOOM = 2.0f;
        constexpr float MAX_IN_ZOOM = 5.0f;
        constexpr float HEX_Y_SQUASH = 0.5f;
        constexpr float SHAKE_DECAY = 0.9f;
        constexpr float SHAKE_THRESHOLD = 0.1f;
        constexpr float WATER_WAVE_SPEED = 2.0f;
        constexpr int WATER_BRIGHTNESS_BASE = 100;
        constexpr int WATER_BRIGHTNESS_SWING = 20;
        constexpr float CLOUD_MAX_RANGE = 4000.0f;
        constexpr float CLOUD_SPEED_BASE = 5.0f;
        constexpr float CLOUD_SPEED_MULT = 15.0f;
        constexpr int CULLING_MARGIN = 50;
        constexpr float HEX_VISUAL_SCALE = 0.98f;
        constexpr int MINIMAP_UPDATE_MS = 100;
        constexpr int MINIMAP_RANGE = 40;
        constexpr int SCANLINE_SPACING = 3;
        constexpr int WORLD_BOUNDS_INT = 50;
        constexpr int MOUNTAIN_HEIGHT = 20;
        constexpr int GRASS_HEIGHT = 5;
        constexpr int DIRT_HEIGHT = 0;
        constexpr int WATER_HEIGHT = 0;
    }
}

#endif
