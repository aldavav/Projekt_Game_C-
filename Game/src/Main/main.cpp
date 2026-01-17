#include <Core/Logger/LoggerMacros.h>
#include <Main/MainWindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    LOG_INFO("Application starting...");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LOG_INFO("Game exited successfully.");
    return a.exec();
}
