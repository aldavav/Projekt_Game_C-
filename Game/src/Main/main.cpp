#include <Core/Logger/LoggerMacros.h>
#include <UI/Manager/ConfigManager.h>
#include <Main/MainWindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    LOG_INFO("Application starting...");
    QApplication a(argc, argv);
    ConfigManager::getInstance().loadConfiguration();
    MainWindow w;
    w.show();
    LOG_INFO("Game exited successfully.");
    return a.exec();
}
