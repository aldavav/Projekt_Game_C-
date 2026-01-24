#ifndef INFORMATIONDIALOG_H
#define INFORMATIONDIALOG_H

#include <UI/Widgets/BaseTacticalDialog.h>
#include <Core/Config/Configuration.h>
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QLabel>

class InformationDialog : public BaseTacticalDialog
{
    Q_OBJECT

public:
    explicit InformationDialog(const QString &header, const QString &body, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void handleEscape() override;

private slots:
    void onDecline();

    void checkRequirements();

    void handleScroll(int value);

private:
    void setupUI(const QString &header, const QString &body);

    QCheckBox *m_acceptCheck = nullptr;

    QPushButton *m_confirmBtn = nullptr;

    QScrollArea *m_scrollArea = nullptr;

    bool m_scrolledToBottom = false;
};

#endif
