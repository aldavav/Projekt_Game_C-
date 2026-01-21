#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <Core/Config/Configuration.h>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPainter>
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

    void resizeEvent(QResizeEvent *event);

private:
    QProgressBar *m_progressBar;

    QLabel *m_statusLabel;
};

#endif
