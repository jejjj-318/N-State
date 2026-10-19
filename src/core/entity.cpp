#include "entity.h"

Entity::Entity()
    : name("unnamed")
{
}

Entity::Entity(const std::string& name)
    : name(name.empty() ? "unnamed" : name)
{
}

Entity::Entity(const Entity& other)
    : name(other.name)
{
}

Entity::~Entity() = default;

const std::string& Entity::getName() const
{
    return name;
}

void Entity::setName(const std::string& name)
{
    this->name = name.empty() ? "unnamed" : name;
}
