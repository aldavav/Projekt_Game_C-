#ifndef PLAYER_H
#define PLAYER_H

#include <Logger/LoggerMacros.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

class Entity;

class Player
{
private:
    std::string m_factionName;

    int m_credits;

    int m_powerSupply;

    int m_powerUsage;

    std::vector<Entity *> m_ownedUnits;

public:
    Player(const std::string &name, int startingCredits);

    ~Player();

    const std::string &getFactionName() const;

    int getCredits() const;

    void addCredits(int amount);

    bool spendCredits(int amount);

    int getAvailablePower() const;

    void updatePower(int supplyChange, int usageChange);

    void addOwnedUnit(Entity *unit);

    void removeOwnedUnit(Entity *unit);

    void savePlayer(std::ofstream &file) const;

    static Player *loadPlayer(std::ifstream &file);
};

#endif
