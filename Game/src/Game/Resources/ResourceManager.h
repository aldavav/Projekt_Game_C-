#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <Core/Config/Configuration.h>
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
public:
    static ResourceManager &getInstance();

    TexturePtr getTexture(const QString &resourceId);

    AudioPtr getAudio(const QString &resourceId);

    void clearCache();

    ResourceManager(const ResourceManager &) = delete;

    ResourceManager &operator=(const ResourceManager &) = delete;

    ResourceManager(ResourceManager &&other) noexcept = default;

    ResourceManager &operator=(ResourceManager &&other) noexcept = default;

private:
    ResourceManager() = default;

    QMap<QString, TexturePtr> m_textureCache;

    QMap<QString, AudioPtr> m_audioCache;

    TexturePtr loadTextureFromFile(const QString &filePath);

    AudioPtr loadAudioFromFile(const QString &filePath);

    mutable QMutex m_mutex;
};

#endif
