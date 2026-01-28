#include "UnitBuilder.h"

UnitBuilder::UnitBuilder() {
    reset();
}

UnitBuilder& UnitBuilder::reset() {
    // Teď už to funguje přímo, protože Entity není abstraktní!
    m_product = std::make_unique<Entity>();
    return *this;
}

UnitBuilder& UnitBuilder::setPosition(const QPointF& pos) {
    if (m_product) m_product->setPosition(pos);
    return *this;
}

UnitBuilder& UnitBuilder::setOwner(Player* owner) {
    if (m_product) m_product->setOwner(owner);
    return *this;
}

UnitBuilder& UnitBuilder::makeTank() {
    m_product->setType(Engine::Entity::Type::Unit);
    m_product->setName("Tank");
    m_product->setSymbol("T");
    m_product->setSpeed(Config::Entities::DEFAULT_SPEED * 0.7f);
    return *this;
}

UnitBuilder& UnitBuilder::makeSoldier() {
    m_product->setType(Engine::Entity::Type::Unit);
    m_product->setName("Infantry");
    m_product->setSymbol("I");
    m_product->setSpeed(Config::Entities::DEFAULT_SPEED);
    return *this;
}

UnitBuilder& UnitBuilder::makePlane() {
    m_product->setType(Engine::Entity::Type::Unit);
    m_product->setName("BattlePlane");
    m_product->setSymbol("P");
    m_product->setSpeed(Config::Entities::DEFAULT_SPEED * 2.5f);
    return *this;
}

UnitBuilder& UnitBuilder::makeCity() {
    m_product->setType(Engine::Entity::Type::Building);
    m_product->setName("Base");
    m_product->setSymbol("City");
    m_product->setSpeed(0.0f);
    return *this;
}

std::unique_ptr<Entity> UnitBuilder::build() {
    return std::move(m_product);
}
