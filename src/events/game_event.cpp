#include "game_event.h"

GameEvent::GameEvent()
    : Entity("event")
{
}

GameEvent::GameEvent(const std::string& name)
    : Entity(name)
{
}

GameEvent::GameEvent(const GameEvent& other)
    : Entity(other)
{
}

GameEvent::~GameEvent() = default;

std::unique_ptr<GameEvent> GameEvent::createFor(
    const Country& country
)
{
    if (country.getInflation() >= 20.0)
    {
        return std::make_unique<EconomicEvent>();
    }
    if (country.getUnemployment() >= 15.0)
    {
        return std::make_unique<SocialEvent>();
    }
    if (country.getGovernmentSupport() <= 35.0)
    {
        return std::make_unique<PoliticalEvent>();
    }
    return nullptr;
}

EconomicEvent::EconomicEvent()
    : GameEvent("inflation_shock")
{
}

EconomicEvent::EconomicEvent(const EconomicEvent& other)
    : GameEvent(other)
{
}

EconomicEvent::~EconomicEvent() = default;

std::string EconomicEvent::identify() const
{
    return "EconomicEvent";
}

void EconomicEvent::apply(Country& country) const
{
    country.setInflation(country.getInflation() + 0.4);
    country.setGovernmentSupport(
        country.getGovernmentSupport() - 0.3
    );
}

std::string EconomicEvent::describe() const
{
    return "Экономический шок: инфляция давит на бюджет.";
}

SocialEvent::SocialEvent()
    : GameEvent("labor_tension")
{
}

SocialEvent::SocialEvent(const SocialEvent& other)
    : GameEvent(other)
{
}

SocialEvent::~SocialEvent() = default;

std::string SocialEvent::identify() const
{
    return "SocialEvent";
}

void SocialEvent::apply(Country& country) const
{
    country.setUnemployment(country.getUnemployment() + 0.2);
    country.setGovernmentSupport(
        country.getGovernmentSupport() - 0.4
    );
}

std::string SocialEvent::describe() const
{
    return "Социальное напряжение из-за безработицы.";
}

PoliticalEvent::PoliticalEvent()
    : GameEvent("support_drop")
{
}

PoliticalEvent::PoliticalEvent(const PoliticalEvent& other)
    : GameEvent(other)
{
}

PoliticalEvent::~PoliticalEvent() = default;

std::string PoliticalEvent::identify() const
{
    return "PoliticalEvent";
}

void PoliticalEvent::apply(Country& country) const
{
    country.setGovernmentSupport(
        country.getGovernmentSupport() - 0.5
    );
}

std::string PoliticalEvent::describe() const
{
    return "Политический кризис: поддержка падает.";
}
