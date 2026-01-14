#ifndef GRAPHICSSETTINGSMANAGER_H
#define GRAPHICSSETTINGSMANAGER_H

#include <UI/Manager/ConfigManager.h>
#include <QObject>

class GraphicsSettingsManager : public QObject
{
    Q_OBJECT
public:
    static GraphicsSettingsManager &getInstance();

    enum class QualityLevel
    {
        Low = 0,
        Medium,
        High,
        Ultra
    };

    void applyGraphicsSettings();

    float getGammaValue() const;

signals:
    void graphicsUpdated();

private:
    GraphicsSettingsManager() = default;
};

#endif
