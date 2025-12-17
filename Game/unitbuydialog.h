#ifndef UNITBUYDIALOG_H
#define UNITBUYDIALOG_H

#pragma once
#include <QDialog>

class UnitBuyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UnitBuyDialog(const QString& unitName, QWidget* parent = nullptr);
};

#endif // UNITBUYDIALOG_H
