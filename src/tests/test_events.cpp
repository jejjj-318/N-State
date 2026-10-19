#include <cassert>
#include <memory>
#include <string>

#include "../events/game_event.h"

namespace
{
    class TestEvent final : public GameEvent
    {
    public:
        TestEvent()
            : GameEvent("test_event")
        {
        }

        TestEvent(const TestEvent& other)
            : GameEvent(other)
        {
        }

        ~TestEvent() override = default;

        std::string identify() const override
        {
            return "TestEvent";
        }

        void apply(Country& country) const override
        {
            country.setGovernmentSupport(
                country.getGovernmentSupport() + 1.0
            );
        }

        std::string describe() const override
        {
            return "Test event";
        }
    };
}

void test_events()
{
    TestEvent event;

    assert(event.getName() == "test_event");
    assert(event.identify() == "TestEvent");
    assert(event.describe() == "Test event");

    Country testCountry("N", Difficulty::Unstable);

    const double supportBefore =
        testCountry.getGovernmentSupport();

    event.apply(testCountry);

    assert(
        testCountry.getGovernmentSupport() ==
        supportBefore + 1.0
    );

    TestEvent copied(event);

    assert(copied.getName() == "test_event");
    assert(copied.identify() == "TestEvent");

    EconomicEvent economic;

    assert(economic.getName() == "inflation_shock");
    assert(economic.identify() == "EconomicEvent");
    assert(economic.describe() ==
           "Экономический шок: инфляция давит на бюджет.");

    Country economicCountry("N", Difficulty::Unstable);
    economicCountry.setInflation(10.0);
    economicCountry.setGovernmentSupport(50.0);

    economic.apply(economicCountry);

    assert(economicCountry.getInflation() == 10.4);
    assert(economicCountry.getGovernmentSupport() == 49.7);

    EconomicEvent economicCopy(economic);

    assert(economicCopy.identify() == "EconomicEvent");

    SocialEvent social;

    assert(social.getName() == "labor_tension");
    assert(social.identify() == "SocialEvent");
    assert(social.describe() ==
           "Социальное напряжение из-за безработицы.");

    Country socialCountry("N", Difficulty::Unstable);
    socialCountry.setUnemployment(10.0);
    socialCountry.setGovernmentSupport(50.0);

    social.apply(socialCountry);

    assert(socialCountry.getUnemployment() == 10.2);
    assert(socialCountry.getGovernmentSupport() == 49.6);

    SocialEvent socialCopy(social);

    assert(socialCopy.identify() == "SocialEvent");

    PoliticalEvent political;

    assert(political.getName() == "support_drop");
    assert(political.identify() == "PoliticalEvent");
    assert(political.describe() ==
           "Политический кризис: поддержка падает.");

    Country politicalCountry("N", Difficulty::Unstable);
    politicalCountry.setGovernmentSupport(40.0);

    political.apply(politicalCountry);

    assert(politicalCountry.getGovernmentSupport() == 39.5);

    PoliticalEvent politicalCopy(political);

    assert(politicalCopy.identify() == "PoliticalEvent");

    Country economicTrigger("N", Difficulty::Unstable);
    economicTrigger.setInflation(20.0);

    std::unique_ptr<GameEvent> created =
        GameEvent::createFor(economicTrigger);

    assert(created != nullptr);
    assert(created->identify() == "EconomicEvent");

    Country socialTrigger("N", Difficulty::Unstable);
    socialTrigger.setInflation(10.0);
    socialTrigger.setUnemployment(15.0);

    created = GameEvent::createFor(socialTrigger);

    assert(created != nullptr);
    assert(created->identify() == "SocialEvent");

    Country politicalTrigger("N", Difficulty::Unstable);
    politicalTrigger.setInflation(10.0);
    politicalTrigger.setUnemployment(10.0);
    politicalTrigger.setGovernmentSupport(35.0);

    created = GameEvent::createFor(politicalTrigger);

    assert(created != nullptr);
    assert(created->identify() == "PoliticalEvent");

    Country noEvent("N", Difficulty::Unstable);
    noEvent.setInflation(10.0);
    noEvent.setUnemployment(10.0);
    noEvent.setGovernmentSupport(50.0);

    created = GameEvent::createFor(noEvent);

    assert(created == nullptr);

    Country priority("N", Difficulty::Unstable);
    priority.setInflation(20.0);
    priority.setUnemployment(20.0);
    priority.setGovernmentSupport(20.0);

    created = GameEvent::createFor(priority);

    assert(created != nullptr);
    assert(created->identify() == "EconomicEvent");
}