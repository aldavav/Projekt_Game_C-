/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
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
    QAction *actionSave;
    QAction *actioLoad;
    QAction *actionQuit;
    QWidget *centralwidget;
    QPushButton *rightConfirmButton;
    QPushButton *leftConfirmButton;
    QStackedWidget *stackedWidget;
    QWidget *placingMap;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *leftPlaceholder;
    QHBoxLayout *rightPlacehoder;
    QWidget *unifiedMap;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *battlePlaceholder;
    QStackedWidget *stackedWidget_2;
    QWidget *page;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_15;
    QHBoxLayout *horizontalLayout_27;
    QLabel *playerLabel2;
    QLabel *activeInventory2;
    QHBoxLayout *horizontalLayout_28;
    QLabel *playerGoldLabel2;
    QLabel *stackedInventory2;
    QGroupBox *groupBox_8;
    QVBoxLayout *verticalLayout_16;
    QHBoxLayout *horizontalLayout_29;
    QLabel *playerLabel;
    QLabel *activeInventory;
    QHBoxLayout *horizontalLayout_30;
    QLabel *playerGoldLabel;
    QLabel *stackedInventory;
    QWidget *page_2;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuSave;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(864, 780);
        MainWindow->setMinimumSize(QSize(864, 780));
        MainWindow->setMaximumSize(QSize(864, 780));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actioLoad = new QAction(MainWindow);
        actioLoad->setObjectName("actioLoad");
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        rightConfirmButton = new QPushButton(centralwidget);
        rightConfirmButton->setObjectName("rightConfirmButton");
        rightConfirmButton->setGeometry(QRect(560, 690, 151, 24));
        leftConfirmButton = new QPushButton(centralwidget);
        leftConfirmButton->setObjectName("leftConfirmButton");
        leftConfirmButton->setGeometry(QRect(150, 690, 166, 24));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(10, 160, 841, 501));
        placingMap = new QWidget();
        placingMap->setObjectName("placingMap");
        layoutWidget_2 = new QWidget(placingMap);
        layoutWidget_2->setObjectName("layoutWidget_2");
        layoutWidget_2->setGeometry(QRect(0, 0, 841, 501));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        leftPlaceholder = new QHBoxLayout();
        leftPlaceholder->setObjectName("leftPlaceholder");

        horizontalLayout->addLayout(leftPlaceholder);

        rightPlacehoder = new QHBoxLayout();
        rightPlacehoder->setObjectName("rightPlacehoder");

        horizontalLayout->addLayout(rightPlacehoder);

        stackedWidget->addWidget(placingMap);
        unifiedMap = new QWidget();
        unifiedMap->setObjectName("unifiedMap");
        horizontalLayoutWidget = new QWidget(unifiedMap);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 841, 511));
        battlePlaceholder = new QHBoxLayout(horizontalLayoutWidget);
        battlePlaceholder->setObjectName("battlePlaceholder");
        battlePlaceholder->setContentsMargins(0, 0, 0, 0);
        stackedWidget->addWidget(unifiedMap);
        stackedWidget_2 = new QStackedWidget(centralwidget);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(20, 40, 821, 101));
        page = new QWidget();
        page->setObjectName("page");
        groupBox_7 = new QGroupBox(page);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(420, 0, 401, 101));
        verticalLayout_15 = new QVBoxLayout(groupBox_7);
        verticalLayout_15->setObjectName("verticalLayout_15");
        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setObjectName("horizontalLayout_27");
        playerLabel2 = new QLabel(groupBox_7);
        playerLabel2->setObjectName("playerLabel2");

        horizontalLayout_27->addWidget(playerLabel2);

        activeInventory2 = new QLabel(groupBox_7);
        activeInventory2->setObjectName("activeInventory2");

        horizontalLayout_27->addWidget(activeInventory2);


        verticalLayout_15->addLayout(horizontalLayout_27);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setObjectName("horizontalLayout_28");
        playerGoldLabel2 = new QLabel(groupBox_7);
        playerGoldLabel2->setObjectName("playerGoldLabel2");

        horizontalLayout_28->addWidget(playerGoldLabel2);

        stackedInventory2 = new QLabel(groupBox_7);
        stackedInventory2->setObjectName("stackedInventory2");

        horizontalLayout_28->addWidget(stackedInventory2);


        verticalLayout_15->addLayout(horizontalLayout_28);

        groupBox_8 = new QGroupBox(page);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setGeometry(QRect(0, 0, 401, 101));
        verticalLayout_16 = new QVBoxLayout(groupBox_8);
        verticalLayout_16->setObjectName("verticalLayout_16");
        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setObjectName("horizontalLayout_29");
        playerLabel = new QLabel(groupBox_8);
        playerLabel->setObjectName("playerLabel");

        horizontalLayout_29->addWidget(playerLabel);

        activeInventory = new QLabel(groupBox_8);
        activeInventory->setObjectName("activeInventory");

        horizontalLayout_29->addWidget(activeInventory);


        verticalLayout_16->addLayout(horizontalLayout_29);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setObjectName("horizontalLayout_30");
        playerGoldLabel = new QLabel(groupBox_8);
        playerGoldLabel->setObjectName("playerGoldLabel");

        horizontalLayout_30->addWidget(playerGoldLabel);

        stackedInventory = new QLabel(groupBox_8);
        stackedInventory->setObjectName("stackedInventory");

        horizontalLayout_30->addWidget(stackedInventory);


        verticalLayout_16->addLayout(horizontalLayout_30);

        stackedWidget_2->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget_2->addWidget(page_2);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(330, 670, 221, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 864, 37));
        menuSave = new QMenu(menubar);
        menuSave->setObjectName("menuSave");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSave->menuAction());
        menuSave->addAction(actionSave);
        menuSave->addAction(actioLoad);
        menuSave->addAction(actionQuit);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);
        stackedWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actioLoad->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
#if QT_CONFIG(shortcut)
        actioLoad->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        rightConfirmButton->setText(QCoreApplication::translate("MainWindow", "Confirm", nullptr));
        leftConfirmButton->setText(QCoreApplication::translate("MainWindow", "Confirm", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "Team2", nullptr));
        playerLabel2->setText(QCoreApplication::translate("MainWindow", "Player", nullptr));
        activeInventory2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"right\">Active:</p></body></html>", nullptr));
        playerGoldLabel2->setText(QCoreApplication::translate("MainWindow", "Gold", nullptr));
        stackedInventory2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"right\">Stacked:</p></body></html>", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "Team1", nullptr));
        playerLabel->setText(QCoreApplication::translate("MainWindow", "Player", nullptr));
        activeInventory->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"right\">Active:</p></body></html>", nullptr));
        playerGoldLabel->setText(QCoreApplication::translate("MainWindow", "Gold", nullptr));
        stackedInventory->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"right\">Stacked:</p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Action", nullptr));
        menuSave->setTitle(QCoreApplication::translate("MainWindow", "Game", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
