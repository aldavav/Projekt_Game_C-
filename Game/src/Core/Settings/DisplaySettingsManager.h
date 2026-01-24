#ifndef DISPLAYSETTINGSMANAGER_H
#define DISPLAYSETTINGSMANAGER_H

#include <Game/Resources/ConfigManager.h>
#include <Core/Config/Configuration.h>
#include <QGuiApplication>
#include <QObject>
#include <QScreen>
#include <QSize>

class DisplaySettingsManager : public QObject
{
    Q_OBJECT

public:
    static DisplaySettingsManager &getInstance();

    QStringList getAvailableResolutions() const;

    void applySettings();

signals:
    void displayChanged();

private:
    DisplaySettingsManager() = default;
};

#endif
