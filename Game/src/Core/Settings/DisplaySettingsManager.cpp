#include "DisplaySettingsManager.h"

DisplaySettingsManager &DisplaySettingsManager::getInstance()
{
    static DisplaySettingsManager instance;
    return instance;
}

QStringList DisplaySettingsManager::getAvailableResolutions() const
{
    QStringList resList;

    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
    {
        return {QString("%1x%2")
                    .arg(Config::Gameplay::DEFAULT_WINDOW_WIDTH)
                    .arg(Config::Gameplay::DEFAULT_WINDOW_HEIGHT)};
    }

    QSize logicalSize = screen->size();
    qreal dpr = screen->devicePixelRatio();

    QSize physicalSize(
        int(logicalSize.width() * dpr),
        int(logicalSize.height() * dpr));

    QList<QSize> commonRes = {
        {1280, 720},
        {1366, 768},
        {1600, 900},
        {1920, 1080},
        {2560, 1440},
        {3840, 2160}};

    for (const QSize &res : commonRes)
    {
        if (res.width() <= physicalSize.width() &&
            res.height() <= physicalSize.height())
        {
            resList << QString("%1x%2").arg(res.width()).arg(res.height());
        }
    }
    resList.removeDuplicates();
    return resList;
}

void DisplaySettingsManager::applySettings()
{
    emit displayChanged();
}
