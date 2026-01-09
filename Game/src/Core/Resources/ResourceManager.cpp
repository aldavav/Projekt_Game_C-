#include "ResourceManager.h"

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager()
{
    qDebug() << "ResourceManager initialized.";
}

ResourceManager::~ResourceManager()
{
    qDebug() << "ResourceManager shut down. Caches cleared automatically.";
}

TexturePtr ResourceManager::getTexture(const QString &resourceId)
{
    if (m_textureCache.contains(resourceId))
        return m_textureCache.value(resourceId);

    TexturePtr newTexture = loadTextureFromFile(resourceId);

    if (!newTexture)
    {
        return getTexture(":/images/assets/images/missing.png");
    }

    m_textureCache.insert(resourceId, newTexture);
    return newTexture;
}

AudioPtr ResourceManager::getAudio(const QString &resourceId)
{
    if (m_audioCache.contains(resourceId))
    {
        qDebug() << "Audio cache hit for:" << resourceId;
        return m_audioCache.value(resourceId);
    }
    qDebug() << "Audio cache miss. Loading:" << resourceId;
    AudioPtr newAudio = loadAudioFromFile(resourceId);
    if (newAudio)
    {
        m_audioCache.insert(resourceId, newAudio);
    }
    else
    {
        qDebug() << "ERROR: Failed to load audio:" << resourceId;
    }
    return newAudio;
}

TexturePtr ResourceManager::loadTextureFromFile(const QString &filePath)
{
    TexturePtr image = TexturePtr::create();

    if (image->load(filePath))
    {
        qDebug() << "Successfully loaded texture from:" << filePath;
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
    sound->setSource(QUrl::fromLocalFile(filePath));

    return sound;
}

void ResourceManager::clearCache()
{
    m_textureCache.clear();
    m_audioCache.clear();
    qDebug() << "ResourceManager caches cleared.";
}
