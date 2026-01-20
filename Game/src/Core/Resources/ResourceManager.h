#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <Core/Config/GameConfig.h>
#include <Core/Config/Config.h>
#include <QSharedPointer>
#include <QSoundEffect>
#include <qmutex.h>
#include <QObject>
#include <QString>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QFile>
#include <QUrl>
#include <QMap>

using TexturePtr = QSharedPointer<QImage>;
using AudioPtr = QSharedPointer<QSoundEffect>;

class ResourceManager
{
private:
    ResourceManager() = default;

    QMap<QString, TexturePtr> m_textureCache;

    QMap<QString, AudioPtr> m_audioCache;

    TexturePtr loadTextureFromFile(const QString &filePath);

    AudioPtr loadAudioFromFile(const QString &filePath);

    mutable QMutex m_mutex;

public:
    static ResourceManager &getInstance();

    ResourceManager(const ResourceManager &) = delete;

    void operator=(const ResourceManager &) = delete;

    ResourceManager(ResourceManager &&) = delete;

    ResourceManager &operator=(ResourceManager &&) = delete;

    TexturePtr getTexture(const QString &resourceId);

    AudioPtr getAudio(const QString &resourceId);

    void clearCache();
};

#endif
