#ifndef TACTICALDIALOG_H
#define TACTICALDIALOG_H

#include <Core/Config/Config.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QDialog>
#include <QFrame>
#include <QLabel>

class TacticalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TacticalDialog(const QString &title, const QString &message, QWidget *parent = nullptr);

private:
    void setupUI(const QString &title, const QString &message);

    void keyPressEvent(QKeyEvent *event);
};

#endif
