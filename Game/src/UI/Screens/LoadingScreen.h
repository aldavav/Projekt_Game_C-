#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <UI/Manager/MenuManager.h>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>

class LoadingScreen : public AbstractScreen
{
    Q_OBJECT
public:
    explicit LoadingScreen(QWidget *parent = nullptr);

    void onEnter() override;

    void onExit() override;

    void setProgress(int value);

    void setStatus(const QString &status);

private:
    QProgressBar *m_progressBar;

    QLabel *m_statusLabel;
};

#endif
