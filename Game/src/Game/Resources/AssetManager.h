#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <Core/Config/Configuration.h>
#include <QFontDatabase>
#include <QString>
#include <QCursor>
#include <QFont>

class AssetManager
{
public:
    static void loadFonts();

    static QFont getFont(int size = 12, bool bold = false);

    static QCursor getCursor(Engine::Graphics::CursorType type = Engine::Graphics::CursorType::Standard);

    AssetManager(const AssetManager &) = delete;

    AssetManager &operator=(const AssetManager &) = delete;

    AssetManager(AssetManager &&other) noexcept = default;

    AssetManager &operator=(AssetManager &&other) noexcept = default;
};

#endif
