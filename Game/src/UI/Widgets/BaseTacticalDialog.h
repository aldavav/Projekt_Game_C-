#ifndef BASETACTICALDIALOG_H
#define BASETACTICALDIALOG_H

#include <QKeyEvent>
#include <QDialog>

class BaseTacticalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseTacticalDialog(const QSize &fixedSize, const QString &objName, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    virtual void handleEscape();
};

#endif
