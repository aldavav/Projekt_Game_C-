#ifndef DISPLAYSETTINGSMANAGER_H
#define DISPLAYSETTINGSMANAGER_H

#include <UI/Manager/ConfigManager.h>
#include <Core/Config/Config.h>
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

    QSize getResolutionFromIndex(int index) const;

signals:
    void displayChanged();

private:
    DisplaySettingsManager() = default;
};

#endif
