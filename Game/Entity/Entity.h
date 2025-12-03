#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <fstream>

class Entity {
public:
    virtual void saveEntity(std::ofstream &file) = 0;

    static Entity* createEntityFromFile(std::ifstream &file);
};

#endif
