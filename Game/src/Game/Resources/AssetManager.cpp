#include "AssetManager.h"

AssetManager &AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::loadFonts()
{
    for (const QString &fontPath : Config::Paths::PATHS_FONTS)
    {
        int id = QFontDatabase::addApplicationFont(fontPath);
        if (id == -1)
        {
        }
    }
}

QFont AssetManager::getFont(int size, bool bold)
{
    QFont font(Config::UI::FONT_FAMILY);
    font.setPixelSize(size);
    font.setBold(bold);
    return font;
}

QCursor AssetManager::getCursor(Engine::Graphics::CursorType type)
{
    if (m_cursorCache.contains(type))
    {
        return m_cursorCache.value(type);
    }

    QString path;
    switch (type)
    {
    case Engine::Graphics::CursorType::Select:
        path = Config::Paths::CURSOR_SELECT;
        break;
    case Engine::Graphics::CursorType::Attack:
        path = Config::Paths::CURSOR_ATTACK;
        break;
    case Engine::Graphics::CursorType::Wait:
        path = Config::Paths::CURSOR_WAIT;
        break;
    case Engine::Graphics::CursorType::Standard:
    default:
        path = Config::Paths::CURSOR_DEFAULT;
        break;
    }

    QPixmap pix(path);
    QCursor finalCursor;

    if (pix.isNull())
    {
        finalCursor = QCursor(Qt::ArrowCursor);
    }
    else
    {
        finalCursor = QCursor(pix.scaled(Config::UI::CURSOR_SIZE, Config::UI::CURSOR_SIZE,
                                         Qt::KeepAspectRatio, Qt::SmoothTransformation),
                              0, 0);
    }

    m_cursorCache.insert(type, finalCursor);
    return finalCursor;
}
