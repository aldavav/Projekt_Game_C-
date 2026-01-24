#include "BaseTacticalDialog.h"

BaseTacticalDialog::BaseTacticalDialog(const QSize &fixedSize, const QString &objName, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(fixedSize);
    setObjectName(objName);
}

void BaseTacticalDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        handleEscape();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}

void BaseTacticalDialog::handleEscape()
{
    reject();
}
