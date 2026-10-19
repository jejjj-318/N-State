#ifndef N_STATE_GAME_H
#define N_STATE_GAME_H

#include "../country/country.h"
#include "../history/history_table.h"
#include "../history/month_record.h"
#include "../simulation/simulator.h"

#include <boost/optional.hpp>
#include <memory>
#include <string>

class Game
{
public:
    Game();
    Game(const std::string& name, Difficulty difficulty);
    Game(const Game& other);
    ~Game();

    bool isStarted() const;
    bool isFinished() const;

    void start(
        const std::string& name,
        Difficulty difficulty
    );

    Country& country();
    const Country& country() const;

    void nextMonth();
    void nextMonth(int months);

    const HistoryTable<MonthRecord>& history() const;
    boost::optional<std::string> lastEventText() const;

    bool operator==(const Game& other) const;

private:
    void recordMonth(const std::string& eventId);

    std::unique_ptr<Country> state;
    std::unique_ptr<ISimulator> simulator;
    HistoryTable<MonthRecord> records;
    boost::optional<std::string> lastEvent;
};

#endif
