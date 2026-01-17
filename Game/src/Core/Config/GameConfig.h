#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QString>
#include <QStringList>

namespace Config
{
    const QString GAME_TITLE = "Command & Conquer";
    const QString VERSION = "1.0.0 Beta";
    const QString LEAD_DEV = "Michael Szotkowski";
    const int DEFAULT_WIDTH = 1920;
    const int DEFAULT_HEIGHT = 1080;

    namespace Strings
    {
        const QStringList PROGRAMMING_TEAM = {LEAD_DEV, "Aleš Vavřinec"};
        const QStringList CONTRIBUTIONS = {"Marek Hovorka"};
    }
}

#endif
