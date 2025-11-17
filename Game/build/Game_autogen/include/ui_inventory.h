/********************************************************************************
** Form generated from reading UI file 'inventory.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INVENTORY_H
#define UI_INVENTORY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Inventory
{
public:

    void setupUi(QWidget *Inventory)
    {
        if (Inventory->objectName().isEmpty())
            Inventory->setObjectName("Inventory");
        Inventory->resize(200, 450);
        Inventory->setMaximumSize(QSize(200, 450));

        retranslateUi(Inventory);

        QMetaObject::connectSlotsByName(Inventory);
    } // setupUi

    void retranslateUi(QWidget *Inventory)
    {
        Inventory->setWindowTitle(QCoreApplication::translate("Inventory", "Inventory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Inventory: public Ui_Inventory {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INVENTORY_H
