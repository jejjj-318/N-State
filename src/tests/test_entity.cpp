#include <cassert>
#include <string>

#include "../core/entity.h"

class TestEntity : public Entity
{
public:
    TestEntity()
        : Entity()
    {
    }

    explicit TestEntity(const std::string& name)
        : Entity(name)
    {
    }

    TestEntity(const TestEntity& other)
        : Entity(other)
    {
    }

    std::string identify() const override
    {
        return "TestEntity";
    }
};

void test_entity()
{
    TestEntity entity;

    assert(entity.getName() == "unnamed");
    assert(entity.identify() == "TestEntity");

    entity.setName("Country");
    assert(entity.getName() == "Country");

    entity.setName("");
    assert(entity.getName() == "unnamed");

    TestEntity named("Test");

    assert(named.getName() == "Test");

    TestEntity copied(named);

    assert(copied.getName() == "Test");
    assert(copied.identify() == "TestEntity");
}