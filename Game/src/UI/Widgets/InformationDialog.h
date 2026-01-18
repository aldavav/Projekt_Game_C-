#ifndef INFORMATIONDIALOG_H
#define INFORMATIONDIALOG_H

#include <Core/Config/Config.h>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QKeyEvent>
#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QFrame>

class InformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InformationDialog(const QString &header, const QString &body, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onDecline();

    void checkRequirements();

    void handleScroll(int value);

private:
    void setupUI(const QString &header, const QString &body);

    QCheckBox *m_acceptCheck;

    QPushButton *m_confirmBtn;

    QScrollArea *m_scrollArea;

    bool m_scrolledToBottom = false;
};

#endif
