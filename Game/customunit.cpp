#include "customunit.h"

CustomUnit::CustomUnit(const QString& type, int health, int damage, int movement) :
    Unit(type, health, damage, movement) {}
