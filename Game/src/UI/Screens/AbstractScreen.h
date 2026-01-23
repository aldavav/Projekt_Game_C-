#ifndef ABSTRACTSCREEN_H
#define ABSTRACTSCREEN_H

#include <QWidget>

class AbstractScreen : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractScreen(QWidget *parent = nullptr);

    virtual void onEnter();

    virtual void onExit();
};

#endif
