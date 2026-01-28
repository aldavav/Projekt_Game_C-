#ifndef AUDIOSETTINGSMANAGER_H
#define AUDIOSETTINGSMANAGER_H

#include <Game/Resources/ConfigManager.h>
#include <QObject>

class AudioSettingsManager : public QObject
{
    Q_OBJECT

public:
    static AudioSettingsManager &getInstance();

    float getFinalVolume(float channelVolume) const;

    float getMusicVolume() const;

    float getSFXVolume() const;

    float getVoiceVolume() const;

    AudioSettingsManager(const AudioSettingsManager &) = delete;

    AudioSettingsManager &operator=(const AudioSettingsManager &) = delete;

    AudioSettingsManager(AudioSettingsManager &&other) noexcept = delete;

    AudioSettingsManager &operator=(AudioSettingsManager &&other) noexcept = delete;

signals:
    void volumesChanged();

private:
    AudioSettingsManager() = default;
};

#endif
