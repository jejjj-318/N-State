#ifndef N_STATE_GAME_EVENT_H
#define N_STATE_GAME_EVENT_H

#include "../core/entity.h"
#include "../country/country.h"

#include <memory>
#include <string>

class GameEvent : public Entity
{
public:
    GameEvent();
    explicit GameEvent(const std::string& name);
    GameEvent(const GameEvent& other);
    ~GameEvent() override;

    virtual void apply(Country& country) const = 0;
    virtual std::string describe() const = 0;

    static std::unique_ptr<GameEvent> createFor(
        const Country& country
    );
};

class EconomicEvent final : public GameEvent
{
public:
    EconomicEvent();
    EconomicEvent(const EconomicEvent& other);
    ~EconomicEvent() override;

    std::string identify() const override;
    void apply(Country& country) const override;
    std::string describe() const override;
};

class SocialEvent final : public GameEvent
{
public:
    SocialEvent();
    SocialEvent(const SocialEvent& other);
    ~SocialEvent() override;

    std::string identify() const override;
    void apply(Country& country) const override;
    std::string describe() const override;
};

class PoliticalEvent final : public GameEvent
{
public:
    PoliticalEvent();
    PoliticalEvent(const PoliticalEvent& other);
    ~PoliticalEvent() override;

    std::string identify() const override;
    void apply(Country& country) const override;
    std::string describe() const override;
};

#endif
