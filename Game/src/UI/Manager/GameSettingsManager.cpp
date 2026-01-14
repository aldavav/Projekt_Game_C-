#include "GameSettingsManager.h"

GameSettingsManager &GameSettingsManager::getInstance()
{
    static GameSettingsManager instance;
    return instance;
}

void GameSettingsManager::setLanguage(const QString &langCode)
{
    qApp->removeTranslator(&m_translator);

    QString fileName = QString(":/i18n/assets/i18n/game_%1.qm").arg(langCode);

    if (m_translator.load(fileName))
    {
        qApp->installTranslator(&m_translator);
        emit languageChanged();
    }
}

void GameSettingsManager::setTooltipsEnabled(bool enabled)
{
    if (m_tooltipsEnabled != enabled)
    {
        m_tooltipsEnabled = enabled;
        emit interfaceUpdated();
    }
}
