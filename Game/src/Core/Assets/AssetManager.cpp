#include "AssetManager.h"

void AssetManager::loadFonts()
{
    const QStringList fonts = {
        ":/fonts/assets/fonts/Orbitron-Black.ttf",
        ":/fonts/assets/fonts/Orbitron-Bold.ttf",
        ":/fonts/assets/fonts/Orbitron-ExtraBold.ttf",
        ":/fonts/assets/fonts/Orbitron-Medium.ttf",
        ":/fonts/assets/fonts/Orbitron-Regular.ttf",
        ":/fonts/assets/fonts/Orbitron-SemiBold.ttf"};

    for (const QString &fontPath : fonts)
    {
        QFontDatabase::addApplicationFont(fontPath);
    }
}

QFont AssetManager::getFont(int size, bool bold)
{
    QFont font("Orbitron");
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
        path = ":/images/assets/images/cursor-pointer.png";
        break;
    case Asset::CursorType::Attack:
        path = ":/images/assets/images/cursor-attack.png";
        break;
    case Asset::CursorType::Wait:
        path = ":/images/assets/images/cursor-wait.png";
        break;
    case Asset::CursorType::Standard:
    default:
        path = ":/images/assets/images/cursor.png";
        break;
    }

    QPixmap pix(path);
    if (pix.isNull())
        return QCursor(Qt::ArrowCursor);

    return QCursor(pix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation), 0, 0);
}
