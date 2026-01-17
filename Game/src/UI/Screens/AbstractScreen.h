#ifndef ABSTRACTSCREEN_H
#define ABSTRACTSCREEN_H

#include <QWidget>

class AbstractScreen : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractScreen(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual ~AbstractScreen() = default;

    virtual void onEnter() = 0;

    virtual void onExit() = 0;
};

#endif
