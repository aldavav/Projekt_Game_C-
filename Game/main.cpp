#include "logger/LoggerMacros.h"
#include "mainwindow/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    LOG_INFO("Starting game...");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LOG_INFO("Game exited successfully.");
    return a.exec();
}
