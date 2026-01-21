#ifndef GAMESETTINGSMANAGER_H
#define GAMESETTINGSMANAGER_H

#include <Core/Config/Configuration.h>
#include <QApplication>
#include <QTranslator>
#include <QObject>

class GameSettingsManager : public QObject
{
    Q_OBJECT
public:
    static GameSettingsManager &getInstance();

    void setLanguage(const QString &langCode);
    
    void setTooltipsEnabled(bool enabled);
    
    bool areTooltipsEnabled() const { return m_tooltipsEnabled; }

signals:
    void languageChanged();
    
    void interfaceUpdated();

private:
    GameSettingsManager() = default;
    
    QTranslator m_translator;
    
    bool m_tooltipsEnabled = true;
};

#endif
