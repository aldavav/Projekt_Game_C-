/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
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
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *gameMap;
    QPushButton *pushButton;
    QWidget *menu;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_gameTitle;
    QSpacerItem *verticalSpacer_top;
    QVBoxLayout *verticalLayout_menuButtons;
    QPushButton *buttonNewGame;
    QPushButton *buttonLoadGame;
    QPushButton *buttonSettings;
    QPushButton *buttonHelp;
    QPushButton *buttonCredits;
    QPushButton *buttonQuit;
    QSpacerItem *verticalSpacer_bottom;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(940, 835);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        actionMenu = new QAction(MainWindow);
        actionMenu->setObjectName("actionMenu");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        gameMap = new QWidget();
        gameMap->setObjectName("gameMap");
        pushButton = new QPushButton(gameMap);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(280, 260, 241, 32));
        stackedWidget->addWidget(gameMap);
        menu = new QWidget();
        menu->setObjectName("menu");
        menu->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(menu);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_gameTitle = new QLabel(menu);
        label_gameTitle->setObjectName("label_gameTitle");
        label_gameTitle->setMinimumSize(QSize(0, 120));
        QFont font;
        font.setPointSize(48);
        font.setBold(true);
        label_gameTitle->setFont(font);
        label_gameTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label_gameTitle);

        verticalSpacer_top = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_top);

        verticalLayout_menuButtons = new QVBoxLayout();
        verticalLayout_menuButtons->setSpacing(15);
        verticalLayout_menuButtons->setObjectName("verticalLayout_menuButtons");
        verticalLayout_menuButtons->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        buttonNewGame = new QPushButton(menu);
        buttonNewGame->setObjectName("buttonNewGame");
        buttonNewGame->setMinimumSize(QSize(250, 50));
        buttonNewGame->setMaximumSize(QSize(350, 50));
        buttonNewGame->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"        font-size: 24px;\n"
"        padding: 10px 30px;\n"
"        border-radius: 10px;\n"
"        color: white; /* Changed font color for better contrast */\n"
"        background-color: #33b79fff; /* A nice primary color */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"        background-color: #289073ff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"        background-color: #1c805aff;\n"
"}"));

        verticalLayout_menuButtons->addWidget(buttonNewGame);

        buttonLoadGame = new QPushButton(menu);
        buttonLoadGame->setObjectName("buttonLoadGame");
        buttonLoadGame->setMinimumSize(QSize(250, 50));
        buttonLoadGame->setMaximumSize(QSize(350, 50));
        buttonLoadGame->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"        font-size: 24px;\n"
"        padding: 10px 30px;\n"
"        border-radius: 10px;\n"
"        color: white;\n"
"        background-color: #337AB7;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"        background-color: #286090;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"        background-color: #1c5980;\n"
"}"));

        verticalLayout_menuButtons->addWidget(buttonLoadGame);

        buttonSettings = new QPushButton(menu);
        buttonSettings->setObjectName("buttonSettings");
        buttonSettings->setMinimumSize(QSize(250, 50));
        buttonSettings->setMaximumSize(QSize(350, 50));
        buttonSettings->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"        font-size: 24px;\n"
"        padding: 10px 30px;\n"
"        border-radius: 10px;\n"
"        color: white;\n"
"        background-color: #337AB7;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"        background-color: #286090;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"        background-color: #1c5980;\n"
"}"));

        verticalLayout_menuButtons->addWidget(buttonSettings);

        buttonHelp = new QPushButton(menu);
        buttonHelp->setObjectName("buttonHelp");
        buttonHelp->setMinimumSize(QSize(250, 50));
        buttonHelp->setMaximumSize(QSize(350, 50));
        buttonHelp->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"        font-size: 24px;\n"
"        padding: 10px 30px;\n"
"        border-radius: 10px;\n"
"        color: white;\n"
"        background-color: #337AB7;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"        background-color: #286090;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"        background-color: #1c5980;\n"
"}"));

        verticalLayout_menuButtons->addWidget(buttonHelp);

        buttonCredits = new QPushButton(menu);
        buttonCredits->setObjectName("buttonCredits");
        buttonCredits->setMinimumSize(QSize(250, 50));
        buttonCredits->setMaximumSize(QSize(350, 50));
        buttonCredits->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"        font-size: 24px;\n"
"        padding: 10px 30px;\n"
"        border-radius: 10px;\n"
"        color: white;\n"
"        background-color: #337AB7;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"        background-color: #286090;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"        background-color: #1c5980;\n"
"}"));

        verticalLayout_menuButtons->addWidget(buttonCredits);

        buttonQuit = new QPushButton(menu);
        buttonQuit->setObjectName("buttonQuit");
        buttonQuit->setMinimumSize(QSize(250, 50));
        buttonQuit->setMaximumSize(QSize(350, 50));
        buttonQuit->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"        font-size: 24px;\n"
"        padding: 10px 30px;\n"
"        border-radius: 10px;\n"
"        color: white;\n"
"        background-color: #C9302C; /* Distinct color for Quit button */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"        background-color: #A92C28;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"        background-color: #902622;\n"
"}"));

        verticalLayout_menuButtons->addWidget(buttonQuit);


        verticalLayout_2->addLayout(verticalLayout_menuButtons);

        verticalSpacer_bottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_bottom);

        stackedWidget->addWidget(menu);

        verticalLayout_3->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Game Title", nullptr));
        actionMenu->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "TesMapButton", nullptr));
        label_gameTitle->setText(QCoreApplication::translate("MainWindow", "Command&Concure", nullptr));
        buttonNewGame->setText(QCoreApplication::translate("MainWindow", "NEW GAME", nullptr));
        buttonLoadGame->setText(QCoreApplication::translate("MainWindow", "LOAD GAME", nullptr));
        buttonSettings->setText(QCoreApplication::translate("MainWindow", "SETTINGS", nullptr));
        buttonHelp->setText(QCoreApplication::translate("MainWindow", "HELP", nullptr));
        buttonCredits->setText(QCoreApplication::translate("MainWindow", "CREDITS", nullptr));
        buttonQuit->setText(QCoreApplication::translate("MainWindow", "QUIT", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
