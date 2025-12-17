#ifndef UNITBUILDER_H
#define UNITBUILDER_H

#include <memory>
#include "Unit.h"
#include "Player.h"
class UnitBuilder
{
protected:
    Player* m_owner = nullptr;

public:
    virtual ~UnitBuilder() = default;

    void setOwner(Player* owner) ;

    virtual std::unique_ptr<Unit> build() = 0;
};

#endif // UNITBUILDER_H
