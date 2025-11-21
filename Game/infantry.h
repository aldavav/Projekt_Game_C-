#ifndef INFANTRY_H
#define INFANTRY_H
#include "unit.h"
class Infantry: public Unit
{
public:
    Infantry();

    QString getType() const override { return "Infantry"; }
};

#endif // INFANTRY_H
