#include "Entity.h"

Entity* Entity::createEntityFromFile(std::ifstream &file) {
    int entityType;
    file.read(reinterpret_cast<char*>(&entityType), sizeof(entityType));
}
