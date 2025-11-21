#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <vector>
#include"unit.h"

class Player
{
public:
    Player(const QString &name = "Player");

    QString getName() ;

    void addUnit(Unit *unit);
    void removeUnit(Unit *unit);

    std::vector<Unit*>& getUnits();

private:
    QString name;
    std::vector<Unit*> units;

};

#endif // PLAYER_H
