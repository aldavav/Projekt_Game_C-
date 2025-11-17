/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionMenu;
    QAction *actionSave;
    QAction *actionQuit;
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *gameMap;
    QPushButton *pushButton;
    QWidget *menu;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonStartGame;
    QPushButton *pushButton_2;
    QPushButton *buttonQuit1;
    QMenuBar *menubar;
    QMenu *menuGame;
    QMenu *menuAbout;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(940, 635);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        actionMenu = new QAction(MainWindow);
        actionMenu->setObjectName("actionMenu");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(-1, -30, 941, 591));
        gameMap = new QWidget();
        gameMap->setObjectName("gameMap");
        pushButton = new QPushButton(gameMap);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(280, 260, 241, 32));
        stackedWidget->addWidget(gameMap);
        menu = new QWidget();
        menu->setObjectName("menu");
        menu->setStyleSheet(QString::fromUtf8(""));
        verticalLayoutWidget = new QWidget(menu);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(330, 120, 261, 291));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        buttonStartGame = new QPushButton(verticalLayoutWidget);
        buttonStartGame->setObjectName("buttonStartGame");
        buttonStartGame->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 24px;        /* Velikost p\303\255sma */\n"
"    padding: 15px 30px;     /* Vertik\303\241ln\303\255 a horizont\303\241ln\303\255 odsazen\303\255 */\n"
"    border-radius: 10px;    /* Zaoblen\303\251 rohy */\n"
" \n"
"    color: black;           /* B\303\255l\303\251 p\303\255smo */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: green; /* Trochu tmav\305\241\303\255 modr\303\241 p\305\231i najet\303\255 my\305\241\303\255 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #1c5980; /* Je\305\241t\304\233 tmav\305\241\303\255 modr\303\241 p\305\231i stisku */\n"
"}"));

        verticalLayout->addWidget(buttonStartGame);

        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 24px;        \n"
"    padding: 15px 30px;     \n"
"    border-radius: 10px;    \n"
" \n"
"    color: black;           \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: green; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #1c5980; \n"
"}"));

        verticalLayout->addWidget(pushButton_2);

        buttonQuit1 = new QPushButton(verticalLayoutWidget);
        buttonQuit1->setObjectName("buttonQuit1");
        buttonQuit1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 24px;        \n"
"    padding: 15px 30px;     \n"
"    border-radius: 10px;    \n"
" \n"
"    color: black;           \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: green; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #1c5980; \n"
"}"));

        verticalLayout->addWidget(buttonQuit1);

        stackedWidget->addWidget(menu);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 940, 39));
        menuGame = new QMenu(menubar);
        menuGame->setObjectName("menuGame");
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName("menuAbout");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuGame->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuGame->addAction(actionMenu);
        menuGame->addAction(actionSave);
        menuGame->addAction(actionQuit);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionMenu->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "TesMapButton", nullptr));
        buttonStartGame->setText(QCoreApplication::translate("MainWindow", "GamePlay", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Options", nullptr));
        buttonQuit1->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        menuGame->setTitle(QCoreApplication::translate("MainWindow", "Game", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
