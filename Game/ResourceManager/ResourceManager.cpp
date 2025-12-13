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
    {
        qDebug() << "Texture cache hit for:" << resourceId;
        return m_textureCache.value(resourceId);
    }
    qDebug() << "Texture cache miss. Loading:" << resourceId;
    TexturePtr newTexture = loadTextureFromFile(resourceId);
    if (newTexture)
    {
        m_textureCache.insert(resourceId, newTexture);
    }
    else
    {
        qDebug() << "ERROR: Failed to load texture:" << resourceId;
    }
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
    QImage *image = new QImage();
    if (image->load(filePath))
    {
        qDebug() << "Successfully loaded texture from:" << filePath;
        return TexturePtr(image);
    }
    else
    {
        delete image;
        return TexturePtr(nullptr);
    }
}

AudioPtr ResourceManager::loadAudioFromFile(const QString &filePath)
{
    QSoundEffect *sound = new QSoundEffect();
    if (QFile::exists(filePath))
    {
        sound->setSource(QUrl::fromLocalFile(filePath));
        if (sound->isLoaded())
        {
            qDebug() << "Successfully loaded audio from:" << filePath;
            return AudioPtr(sound);
        }
    }
    qDebug() << "Failed to load audio or file does not exist:" << filePath;
    delete sound;
    return AudioPtr(nullptr);
}

void ResourceManager::clearCache()
{
    m_textureCache.clear();
    m_audioCache.clear();
    qDebug() << "ResourceManager caches cleared.";
}
