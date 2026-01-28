#ifndef UNITBUILDER_H
#define UNITBUILDER_H
#include "Entity.h"
#include <memory>
class UnitBuilder
{
public:
    UnitBuilder();

    UnitBuilder& reset();
    UnitBuilder& setPosition(const QPointF& pos);
    UnitBuilder& setOwner(Player* owner);

    // Šablony pro tvé 3 jednotky + město
    UnitBuilder& makeTank();
    UnitBuilder& makeSoldier();
    UnitBuilder& makePlane();
    UnitBuilder& makeCity();

    std::unique_ptr<Entity> build();

private:
    std::unique_ptr<Entity> m_product;
};
\

#endif // UNITBUILDER_H
