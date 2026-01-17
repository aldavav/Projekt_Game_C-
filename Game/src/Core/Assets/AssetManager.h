#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <QString>
#include <QFont>
#include <QCursor>
#include <QPixmap>
#include <QFontDatabase>

class AssetManager
{
public:
    static void loadFonts()
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

    static QFont getFont(int size = 12, bool bold = false)
    {
        QFont font("Orbitron");
        font.setPixelSize(size);
        font.setBold(bold);
        return font;
    }

    enum class CursorType
    {
        Standard,
        Attack,
        Select,
        Wait
    };

    static QCursor getCursor(CursorType type)
    {
        QString path;
        switch (type)
        {
        case CursorType::Standard:
            path = ":/images/assets/images/cursor.png";
            break;
        case CursorType::Select:
            path = ":/images/assets/images/cursor-pointer.png";
            break;
        default:
            path = ":/images/assets/images/cursor.png";
            break;
        }

        QPixmap pix(path);
        if (pix.isNull())
            return QCursor(Qt::ArrowCursor);

        return QCursor(pix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation), 0, 0);
    }
};

#endif
