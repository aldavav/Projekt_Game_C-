/********************************************************************************
** Form generated from reading UI file 'option.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTION_H
#define UI_OPTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Option
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonSaveSetting;
    QPushButton *buttonBackMenu;

    void setupUi(QWidget *Option)
    {
        if (Option->objectName().isEmpty())
            Option->setObjectName("Option");
        Option->resize(400, 300);
        widget = new QWidget(Option);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(110, 210, 172, 26));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        buttonSaveSetting = new QPushButton(widget);
        buttonSaveSetting->setObjectName("buttonSaveSetting");

        horizontalLayout->addWidget(buttonSaveSetting);

        buttonBackMenu = new QPushButton(widget);
        buttonBackMenu->setObjectName("buttonBackMenu");

        horizontalLayout->addWidget(buttonBackMenu);


        retranslateUi(Option);

        QMetaObject::connectSlotsByName(Option);
    } // setupUi

    void retranslateUi(QWidget *Option)
    {
        Option->setWindowTitle(QCoreApplication::translate("Option", "Form", nullptr));
        buttonSaveSetting->setText(QCoreApplication::translate("Option", "Save Settings", nullptr));
        buttonBackMenu->setText(QCoreApplication::translate("Option", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Option: public Ui_Option {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTION_H
