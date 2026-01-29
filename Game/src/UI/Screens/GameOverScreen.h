#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <Core/Config/Configuration.h>
#include <UI/Screens/AbstractScreen.h>
#include <Game/Engine/GameEngine.h>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QLabel>

class QPaintEvent;
class QKeyEvent;
class QLabel;

class GameOverScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit GameOverScreen(bool victory, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI(bool victory);

    QLabel *m_titleLabel = nullptr;

    QLabel *m_statsLabel = nullptr;
};

#endif
