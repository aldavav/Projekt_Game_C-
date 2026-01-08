#include "Player.h"

Player::Player(const std::string &name, int startingCredits)
    : m_factionName(name),
      m_credits(startingCredits),
      m_powerSupply(100),
      m_powerUsage(0)
{
    LOG_INFO("New Player Faction created: " + m_factionName + " with " + std::to_string(m_credits) + " credits.");
}

Player::~Player()
{
    m_ownedUnits.clear();
}

const std::string &Player::getFactionName() const
{
    return m_factionName;
}

int Player::getCredits() const
{
    return m_credits;
}

void Player::addCredits(int amount)
{
    m_credits += amount;
    LOG_INFO(m_factionName + " gained " + std::to_string(amount) + " credits. Total: " + std::to_string(m_credits));
}

bool Player::spendCredits(int amount)
{
    if (m_credits >= amount)
    {
        m_credits -= amount;
        LOG_INFO(m_factionName + " spent " + std::to_string(amount) + " credits. Remaining: " + std::to_string(m_credits));
        return true;
    }
    LOG_WARNING(m_factionName + " failed to spend " + std::to_string(amount) + " credits (Insufficient funds).");
    return false;
}

int Player::getAvailablePower() const
{
    return m_powerSupply - m_powerUsage;
}

void Player::updatePower(int supplyChange, int usageChange)
{
    m_powerSupply += supplyChange;
    m_powerUsage += usageChange;
    LOG_INFO(m_factionName + " power updated. Supply: " + std::to_string(m_powerSupply) + ", Usage: " + std::to_string(m_powerUsage));
}

void Player::addOwnedUnit(Entity *unit)
{
    m_ownedUnits.push_back(unit);
}

void Player::removeOwnedUnit(Entity *unit)
{
    m_ownedUnits.erase(std::remove(m_ownedUnits.begin(), m_ownedUnits.end(), unit), m_ownedUnits.end());
}

void Player::savePlayer(std::ofstream &file) const
{
    size_t nameSize = m_factionName.size();
    file.write(reinterpret_cast<const char *>(&nameSize), sizeof(nameSize));
    file.write(m_factionName.c_str(), nameSize);

    file.write(reinterpret_cast<const char *>(&m_credits), sizeof(m_credits));
    file.write(reinterpret_cast<const char *>(&m_powerSupply), sizeof(m_powerSupply));
    file.write(reinterpret_cast<const char *>(&m_powerUsage), sizeof(m_powerUsage));

    LOG_INFO("Player Faction saved: " + m_factionName);
}

Player *Player::loadPlayer(std::ifstream &file)
{
    size_t name_length;
    file.read(reinterpret_cast<char *>(&name_length), sizeof(name_length));
    std::string name(name_length, '\0');
    file.read(&name[0], name_length);

    int credits, powerSupply, powerUsage;
    file.read(reinterpret_cast<char *>(&credits), sizeof(credits));
    file.read(reinterpret_cast<char *>(&powerSupply), sizeof(powerSupply));
    file.read(reinterpret_cast<char *>(&powerUsage), sizeof(powerUsage));

    Player *player = new Player(name, credits);
    player->m_powerSupply = powerSupply;
    player->m_powerUsage = powerUsage;

    LOG_INFO("Player Faction loaded: " + name);
    return player;
}
