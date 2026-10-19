#include "game.h"

#include "../events/game_event.h"

Game::Game()
    : simulator(std::make_unique<MonthSimulator>())
{
}

Game::Game(
    const std::string& name,
    Difficulty difficulty
)
    : state(std::make_unique<Country>(name, difficulty)),
      simulator(std::make_unique<MonthSimulator>())
{
}

Game::Game(const Game& other)
    : simulator(std::make_unique<MonthSimulator>()),
      records(other.records),
      lastEvent(other.lastEvent)
{
    if (other.state != nullptr)
    {
        state = std::make_unique<Country>(*other.state);
    }
}

Game::~Game() = default;

bool Game::isStarted() const
{
    return state != nullptr;
}

bool Game::isFinished() const
{
    return state != nullptr && state->isFinished();
}

void Game::start(
    const std::string& name,
    Difficulty difficulty
)
{
    state = std::make_unique<Country>(name, difficulty);
    records.clear();
    lastEvent = boost::none;
}

Country& Game::country()
{
    return *state;
}

const Country& Game::country() const
{
    return *state;
}

void Game::recordMonth(const std::string& eventId)
{
    const Country& current = *state;
    const MonthRecord record(
        current.getTurn(),
        current.getGdp(),
        current.getInflation(),
        current.getUnemployment(),
        current.getGovernmentSupport(),
        eventId
    );
    records << record;
}

void Game::nextMonth()
{
    if (state == nullptr || state->isFinished())
    {
        return;
    }

    simulator->advanceMonth(*state);

    std::string eventId = "none";
    auto event = GameEvent::createFor(*state);
    if (event != nullptr)
    {
        event->apply(*state);
        lastEvent = event->describe();
        eventId = event->identify();
    }
    else
    {
        lastEvent = boost::none;
    }

    recordMonth(eventId);
}

void Game::nextMonth(int months)
{
    if (months <= 0)
    {
        return;
    }
    for (int i = 0; i < months; ++i)
    {
        nextMonth();
    }
}

const HistoryTable<MonthRecord>& Game::history() const
{
    return records;
}

boost::optional<std::string> Game::lastEventText() const
{
    return lastEvent;
}

bool Game::operator==(const Game& other) const
{
    if (isStarted() != other.isStarted())
    {
        return false;
    }
    if (!isStarted())
    {
        return records == other.records;
    }
    return *state == *other.state &&
           records == other.records;
}
