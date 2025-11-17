/********************************************************************************
** Form generated from reading UI file 'menu.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_H
#define UI_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Menu
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonStart;
    QPushButton *buttonQuit;
    QPushButton *buttonOption;

    void setupUi(QWidget *Menu)
    {
        if (Menu->objectName().isEmpty())
            Menu->setObjectName("Menu");
        Menu->resize(708, 527);
        layoutWidget = new QWidget(Menu);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(240, 130, 251, 201));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        buttonStart = new QPushButton(layoutWidget);
        buttonStart->setObjectName("buttonStart");
        buttonStart->setMinimumSize(QSize(80, 0));
        buttonStart->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid black; /* \304\214ern\303\251 ohrani\304\215en\303\255 */\n"
"    border-radius: 5px;     /* Zakulacen\303\251 rohy (voliteln\303\251) */\n"
"    padding: 5px;           /* Vnit\305\231n\303\255 odsazen\303\255 */\n"
"	color:white;\n"
"	font-size:20px;\n"
"	font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: 2px solid green; /* Ohrani\304\215en\303\255 p\305\231i najet\303\255 my\305\241\303\255 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    border: 2px solid red;  /* Ohrani\304\215en\303\255 p\305\231i kliknut\303\255 */\n"
"    background-color: lightgray; /* Zm\304\233na pozad\303\255 p\305\231i stisku */\n"
"}"));

        verticalLayout->addWidget(buttonStart);

        buttonQuit = new QPushButton(layoutWidget);
        buttonQuit->setObjectName("buttonQuit");
        buttonQuit->setMinimumSize(QSize(80, 0));
        buttonQuit->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid black; /* \304\214ern\303\251 ohrani\304\215en\303\255 */\n"
"    border-radius: 5px;     /* Zakulacen\303\251 rohy (voliteln\303\251) */\n"
"    padding: 5px;           /* Vnit\305\231n\303\255 odsazen\303\255 */\n"
"color:white;\n"
"	font-size:20px;\n"
"	font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: 2px solid red; /* Ohrani\304\215en\303\255 p\305\231i najet\303\255 my\305\241\303\255 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    border: 2px solid red;  /* Ohrani\304\215en\303\255 p\305\231i kliknut\303\255 */\n"
"    background-color: lightgray; /* Zm\304\233na pozad\303\255 p\305\231i stisku */\n"
"}"));

        verticalLayout->addWidget(buttonQuit);

        buttonOption = new QPushButton(layoutWidget);
        buttonOption->setObjectName("buttonOption");
        buttonOption->setMinimumSize(QSize(199, 0));
        buttonOption->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: 2px solid black; /* \304\214ern\303\251 ohrani\304\215en\303\255 */\n"
"    border-radius: 5px;     /* Zakulacen\303\251 rohy (voliteln\303\251) */\n"
"    padding: 5px;           /* Vnit\305\231n\303\255 odsazen\303\255 */\n"
"color:white;\n"
"	font-size:20px;\n"
"	font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: 2px solid green; /* Ohrani\304\215en\303\255 p\305\231i najet\303\255 my\305\241\303\255 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    border: 2px solid red;  /* Ohrani\304\215en\303\255 p\305\231i kliknut\303\255 */\n"
"    background-color: lightgray; /* Zm\304\233na pozad\303\255 p\305\231i stisku */\n"
"}"));

        verticalLayout->addWidget(buttonOption);


        retranslateUi(Menu);

        QMetaObject::connectSlotsByName(Menu);
    } // setupUi

    void retranslateUi(QWidget *Menu)
    {
        Menu->setWindowTitle(QCoreApplication::translate("Menu", "Form", nullptr));
        buttonStart->setText(QCoreApplication::translate("Menu", "Start", nullptr));
        buttonQuit->setText(QCoreApplication::translate("Menu", "Quit", nullptr));
        buttonOption->setText(QCoreApplication::translate("Menu", "Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Menu: public Ui_Menu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_H
