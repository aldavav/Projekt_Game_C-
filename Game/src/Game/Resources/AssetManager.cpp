#include "AssetManager.h"

void AssetManager::loadFonts()
{
    for (const QString &fontPath : Config::UI::PATHS_FONTS)
    {
        QFontDatabase::addApplicationFont(fontPath);
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
    if (pix.isNull())
        return QCursor(Qt::ArrowCursor);

    return QCursor(pix.scaled(Config::UI::CURSOR_SIZE, Config::UI::CURSOR_SIZE,
                              Qt::KeepAspectRatio, Qt::SmoothTransformation),
                   0, 0);
}
