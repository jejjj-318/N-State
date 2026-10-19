#ifndef N_STATE_ENTITY_H
#define N_STATE_ENTITY_H

#include <string>

class Entity
{
public:
    Entity();
    explicit Entity(const std::string& name);
    Entity(const Entity& other);
    virtual ~Entity();

    virtual std::string identify() const = 0;

    const std::string& getName() const;
    void setName(const std::string& name);

private:
    std::string name;
};

#endif
