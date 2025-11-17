/********************************************************************************
** Form generated from reading UI file 'unifiedmap.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNIFIEDMAP_H
#define UI_UNIFIEDMAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_unifiedMap
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *unifiedMap)
    {
        if (unifiedMap->objectName().isEmpty())
            unifiedMap->setObjectName("unifiedMap");
        unifiedMap->resize(864, 780);
        unifiedMap->setMaximumSize(QSize(864, 780));
        verticalLayoutWidget = new QWidget(unifiedMap);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(80, 220, 661, 281));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(unifiedMap);

        QMetaObject::connectSlotsByName(unifiedMap);
    } // setupUi

    void retranslateUi(QWidget *unifiedMap)
    {
        unifiedMap->setWindowTitle(QCoreApplication::translate("unifiedMap", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class unifiedMap: public Ui_unifiedMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNIFIEDMAP_H
