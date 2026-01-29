#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <Core/Config/Configuration.h>
#include <QFontDatabase>
#include <QString>
#include <QCursor>
#include <QPixmap>
#include <QFont>
#include <QMap>

class AssetManager
{
public:
    static AssetManager &getInstance();

    void loadFonts();

    QFont getFont(int size = 12, bool bold = false);

    QCursor getCursor(Engine::Graphics::CursorType type = Engine::Graphics::CursorType::Standard);

    AssetManager(const AssetManager &) = delete;

    AssetManager &operator=(const AssetManager &) = delete;

    AssetManager(AssetManager &&other) noexcept = default;

    AssetManager &operator=(AssetManager &&other) noexcept = default;

private:
    AssetManager() = default;

    QMap<Engine::Graphics::CursorType, QCursor> m_cursorCache;
};

#endif
