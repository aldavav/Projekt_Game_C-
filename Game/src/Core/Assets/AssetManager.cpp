#include "AssetManager.h"

void AssetManager::loadFonts()
{
    for (const QString &fontPath : Config::PATHS_FONTS)
    {
        QFontDatabase::addApplicationFont(fontPath);
    }
}

QFont AssetManager::getFont(int size, bool bold)
{
    QFont font(Config::FONT_FAMILY);
    font.setPixelSize(size);
    font.setBold(bold);
    return font;
}

QCursor AssetManager::getCursor(Asset::CursorType type)
{
    QString path;
    switch (type)
    {
    case Asset::CursorType::Select:
        path = Config::PATH_CURSOR_SELECT;
        break;
    case Asset::CursorType::Attack:
        path = Config::PATH_CURSOR_ATTACK;
        break;
    case Asset::CursorType::Wait:
        path = Config::PATH_CURSOR_WAIT;
        break;
    case Asset::CursorType::Standard:
    default:
        path = Config::PATH_CURSOR_DEFAULT;
        break;
    }

    QPixmap pix(path);
    if (pix.isNull())
        return QCursor(Qt::ArrowCursor);

    return QCursor(pix.scaled(Config::CURSOR_SIZE, Config::CURSOR_SIZE,
                              Qt::KeepAspectRatio, Qt::SmoothTransformation),
                   0, 0);
}
