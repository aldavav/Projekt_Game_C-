#ifndef GRAPHICSSETTINGSMANAGER_H
#define GRAPHICSSETTINGSMANAGER_H

#include <Game/Resources/ConfigManager.h>
#include <Core/Common/GameTypes.h>
#include <QObject>

class GraphicsSettingsManager : public QObject
{
    Q_OBJECT

public:
    static GraphicsSettingsManager &getInstance();

    void applyGraphicsSettings();

    float getGammaValue() const;

signals:
    void graphicsUpdated();

private:
    GraphicsSettingsManager() = default;
};

#endif
