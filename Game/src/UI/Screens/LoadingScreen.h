#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <Core/Config/Configuration.h>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>

class QProgressBar;
class QResizeEvent;
class QLabel;

class LoadingScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit LoadingScreen(QWidget *parent = nullptr);

    void setProgress(int value);

    void setStatus(const QString &status);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();

    QProgressBar *m_progressBar = nullptr;

    QLabel *m_statusLabel = nullptr;
};

#endif
