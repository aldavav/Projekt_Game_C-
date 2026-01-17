#include "ResourceManager.h"

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager()
{
    LOG_INFO("ResourceManager initialized.");
}

ResourceManager::~ResourceManager()
{
    LOG_INFO("ResourceManager shut down. Caches cleared automatically.");
}

TexturePtr ResourceManager::getTexture(const QString &resourceId)
{
    QMutexLocker locker(&m_mutex);
    if (m_textureCache.contains(resourceId))
        return m_textureCache.value(resourceId);

    TexturePtr newTexture = loadTextureFromFile(resourceId);

    if (newTexture.isNull())
    {
        LOG_ERROR(QString("Texture not found: %1").arg(resourceId));
        const QString fallback = ":/images/assets/images/missing.png";
        if (resourceId == fallback)
            return TexturePtr();

        return getTexture(fallback);
    }

    m_textureCache.insert(resourceId, newTexture);
    return newTexture;
}

AudioPtr ResourceManager::getAudio(const QString &resourceId)
{
    if (m_audioCache.contains(resourceId))
    {
        LOG_INFO(QString("Audio cache hit for: %1").arg(resourceId));
        return m_audioCache.value(resourceId);
    }

    LOG_WARNING(QString("Audio cache miss. Loading: %1").arg(resourceId));

    AudioPtr newAudio = loadAudioFromFile(resourceId);
    if (newAudio)
    {
        m_audioCache.insert(resourceId, newAudio);
    }
    else
    {
        LOG_ERROR(QString("Failed to load audio: %1").arg(resourceId));
    }
    return newAudio;
}

TexturePtr ResourceManager::loadTextureFromFile(const QString &filePath)
{
    TexturePtr image = TexturePtr::create();

    if (image->load(filePath))
    {
        LOG_INFO(QString("Successfully loaded texture from: %1").arg(filePath));
        return image;
    }
    else
    {
        return TexturePtr();
    }
}

AudioPtr ResourceManager::loadAudioFromFile(const QString &filePath)
{
    AudioPtr sound(new QSoundEffect());
    sound->setSource(QUrl(filePath));

    return sound;
}

void ResourceManager::clearCache()
{
    m_textureCache.clear();
    m_audioCache.clear();
    LOG_INFO("ResourceManager caches cleared.");
}
