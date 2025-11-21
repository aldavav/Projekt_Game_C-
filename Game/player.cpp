#include "player.h"

Player::Player(const QString &name): name(name) {}


QString Player::getName() {
    return name;
}

void Player::addUnit(Unit *unit) {
    units.push_back(unit);
}

void Player::removeUnit(Unit *unit) {
   // units.erase();
}

std::vector<Unit*>& Player::getUnits(){
    return units;
}
