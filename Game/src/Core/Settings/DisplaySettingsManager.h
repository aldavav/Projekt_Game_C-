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

    DisplaySettingsManager(const DisplaySettingsManager &) = delete;

    DisplaySettingsManager &operator=(const DisplaySettingsManager &) = delete;

    DisplaySettingsManager(DisplaySettingsManager &&other) noexcept = delete;

    DisplaySettingsManager &operator=(DisplaySettingsManager &&other) noexcept = delete;

signals:
    void displayChanged();

private:
    DisplaySettingsManager() = default;
};

#endif
