#include "AbstractScreen.h"

AbstractScreen::AbstractScreen(QWidget *parent)
    : QWidget(parent)
{
}

void AbstractScreen::onEnter()
{
    this->show();
    this->setFocus();
}

void AbstractScreen::onExit()
{
    this->hide();
}
