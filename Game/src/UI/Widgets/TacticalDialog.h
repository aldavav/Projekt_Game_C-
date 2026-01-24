#ifndef TACTICALDIALOG_H
#define TACTICALDIALOG_H

#include <UI/Widgets/BaseTacticalDialog.h>
#include <Core/Config/Configuration.h>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class TacticalDialog : public BaseTacticalDialog
{
    Q_OBJECT

public:
    explicit TacticalDialog(const QString &title, const QString &message, QWidget *parent = nullptr);

private:
    void setupUI(const QString &title, const QString &message);
};

#endif
