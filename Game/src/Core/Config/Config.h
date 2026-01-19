#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include <QStringList>
#include <QString>
#include <QRect>

namespace Config
{
    const QString GAME_TITLE = "Tiny Tiles Tactics";
    const QString VERSION = "1.0.0 Beta";
    const QString LEAD_DEV = "Michael Szotkowski";

    namespace Strings
    {
        const QStringList PROGRAMMING_TEAM = {LEAD_DEV, "Aleš Vavřinec"};
        const QStringList CONTRIBUTIONS = {"Marek Hovorka"};
    }

    constexpr int DEFAULT_WIDTH = 1920;
    constexpr int DEFAULT_HEIGHT = 1080;
    constexpr int CURSOR_SIZE = 32;

    const QString PATH_ICON = ":/images/assets/images/icon.png";
    const QString PATH_STYLES = ":/styles/assets/styles/style.qss";
    const QString PATH_BGM = "qrc:/audio/assets/audio/music.mp3";
    const QString PATH_EULA = ":/legal/assets/legal/EULA/EULA.html";
    const QString PATH_TOS = ":/legal/assets/legal/ToS/ToS.html";

    constexpr int LOADING_STEP_DELAY = 200;
    constexpr int LOADING_FINAL_PAUSE = 300;

    constexpr int SETTINGS_SIDE_STRETCH = 1;
    constexpr int SETTINGS_CONTENT_STRETCH = 8;

    const QList<QPair<QString, QString>> LANGUAGES = {
        {"English", "en"},
        {"Čeština", "cz"}};

    const QString DEFAULT_MISSION_NAME = "Sector_7";

    constexpr int UI_SIDE_STRETCH = 1;
    constexpr int UI_CONTENT_STRETCH = 8;

    constexpr int GENERATION_DELAY = 500;
    constexpr int ARCHIVE_DELAY = 1000;

    const QString PATH_MENU_BG = ":/animations/assets/animations/menu.gif";
    constexpr int BG_ANIM_SPEED = 50;

    constexpr int MENU_LEFT_MARGIN = 200;

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

    constexpr int LOADING_BAR_WIDTH = 560;
    constexpr int LOADING_BAR_HEIGHT = 30;

    const QString DEFAULT_UPLINK_TEXT = "ESTABLISHING UPLINK...";
    const QString DEFAULT_STATUS_TEXT = "INITIALIZING ENGINE...";

    const QString SAVE_DIR_NAME = "/saves";

    constexpr int LIST_SCREEN_MARGIN_H = 100;
    constexpr int LIST_SCREEN_MARGIN_V = 50;

    const QString TEXT_VICTORY = "VICTORY ACHIEVED";
    const QString TEXT_DEFEAT = "DEFEAT: SIGNAL LOST";

    constexpr int END_SCREEN_BTN_WIDTH = 300;
    constexpr int END_SCREEN_BTN_HEIGHT = 50;
    constexpr int END_SCREEN_SPACING = 30;

    const QString PATH_CLOUD_TEXTURE = ":/images/assets/images/cloud.png";

    const QRect WORLD_BOUNDS = QRect(-2000, -1500, 3500, 3000);

    constexpr int INFO_DIALOG_WIDTH = 650;
    constexpr int INFO_DIALOG_HEIGHT = 550;

    constexpr float LEGAL_SCROLL_THRESHOLD = 0.95f;
    constexpr int KEYBOARD_SCROLL_STEP = 20;

    constexpr int KEY_DIALOG_WIDTH = 300;
    constexpr int KEY_DIALOG_HEIGHT = 150;
    constexpr int KEY_DIALOG_MARGINS = 20;

    constexpr int DIALOG_WIDTH = 400;
    constexpr int DIALOG_HEIGHT = 200;

    const QString STR_ABORT = "ABORT";
    const QString STR_PROCEED = "PROCEED";

    constexpr int BTN_WIDTH_NORMAL = 250;
    constexpr int BTN_HEIGHT_NORMAL = 70;
    constexpr int BTN_WIDTH_HOVER = 265;
    constexpr int BTN_HEIGHT_HOVER = 75;

    constexpr int BTN_X_NORMAL = 100;
    constexpr int BTN_X_HOVER = 92;

    constexpr int HUD_MARGIN = 15;
    constexpr int HUD_MINIMAP_SIZE = 150;
    constexpr int HUD_BOX_WIDTH = 220;
    constexpr int HUD_STATS_WIDTH = 180;

    const QString COLOR_TACTICAL_BLUE = "#4FC3F7";
    const QString COLOR_GOLD_ORE = "#FFD600";
    const QString COLOR_HABITABLE = "#81C784";
    const QString COLOR_WATER_INFO = "#64B5F6";
}

#endif
