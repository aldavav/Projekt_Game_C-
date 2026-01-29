#include "ResourceManager.h"

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

TexturePtr ResourceManager::getTexture(const QString &resourceId)
{
    QMutexLocker locker(&m_mutex);
    if (m_textureCache.contains(resourceId))
        return m_textureCache.value(resourceId);

    TexturePtr newTexture = loadTextureFromFile(resourceId);

    if (newTexture.isNull())
    {
        const QString fallback = Config::Paths::MISSING_TEXTURE;
        if (resourceId == fallback)
            return TexturePtr();

        return getTexture(fallback);
    }

    m_textureCache.insert(resourceId, newTexture);
    return newTexture;
}

TexturePtr ResourceManager::loadTextureFromFile(const QString &filePath)
{
    QImage image;
    if (!image.load(filePath))
    {
        return TexturePtr();
    }

    return TexturePtr(new QImage(image));
}

AudioPtr ResourceManager::getAudio(const QString &resourceId)
{
    if (m_audioCache.contains(resourceId))
    {
        return m_audioCache.value(resourceId);
    }

    AudioPtr newAudio = loadAudioFromFile(resourceId);
    if (newAudio)
    {
        m_audioCache.insert(resourceId, newAudio);
    }
    return newAudio;
}

AudioPtr ResourceManager::loadAudioFromFile(const QString &filePath)
{
    AudioPtr sound(new QSoundEffect());
    sound->setSource(QUrl(filePath));

    sound->setVolume(static_cast<float>(Config::Audio::VOLUME_DEFAULT) / 100.0f);

    return sound;
}

void ResourceManager::clearCache()
{
    m_textureCache.clear();
    m_audioCache.clear();
}
