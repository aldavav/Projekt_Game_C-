#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <Core/Config/CursorTypes.h>
#include <Core/Config/Config.h>
#include <QFontDatabase>
#include <QString>
#include <QCursor>
#include <QFont>

class AssetManager
{
public:
    static void loadFonts();
    
    static QFont getFont(int size = 12, bool bold = false);
    
    static QCursor getCursor(Asset::CursorType type);
};

#endif
