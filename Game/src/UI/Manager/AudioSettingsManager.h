#ifndef AUDIOSETTINGSMANAGER_H
#define AUDIOSETTINGSMANAGER_H

#include <UI/Manager/ConfigManager.h>
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

signals:
    void volumesChanged();

private:
    AudioSettingsManager() = default;
};

#endif
