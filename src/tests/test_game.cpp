#include <cassert>

#include "../game/game.h"

void test_game()
{
    Game empty;

    assert(!empty.isStarted());
    assert(!empty.isFinished());
    assert(empty.history().size() == 0);
    assert(!empty.lastEventText());

    empty.nextMonth();

    assert(!empty.isStarted());
    assert(empty.history().size() == 0);

    empty.nextMonth(5);

    assert(!empty.isStarted());
    assert(empty.history().size() == 0);

    Game game("N", Difficulty::Unstable);

    assert(game.isStarted());
    assert(!game.isFinished());

    assert(game.country().getName() == "N");
    assert(game.country().getDifficulty() == Difficulty::Unstable);
    assert(game.history().size() == 0);
    assert(!game.lastEventText());

    game.nextMonth();

    assert(game.country().getTurn() == 1);
    assert(game.history().size() == 1);
    assert(game.history().contains(1));
    assert(game.history()[1].getTurn() == 1);
    assert(game.history()[1].getEventId() == "none");
    assert(!game.lastEventText());

    const double gdpAfterOneMonth =
        game.country().getGdp();

    game.nextMonth(2);

    assert(game.country().getTurn() == 3);
    assert(game.history().size() == 3);
    assert(game.country().getGdp() > gdpAfterOneMonth);
    assert(game.history().contains(2));
    assert(game.history().contains(3));

    game.nextMonth(0);

    assert(game.country().getTurn() == 3);

    game.nextMonth(-10);

    assert(game.country().getTurn() == 3);

    Game copied(game);

    assert(copied == game);
    assert(copied.isStarted());
    assert(copied.history().size() == game.history().size());

    copied.country().setGdp(999.0);

    assert(!(copied == game));

    Game assigned("Other", Difficulty::Crisis);

    assigned.start("N", Difficulty::Unstable);

    assigned.nextMonth();

    assert(assigned.history().size() == 1);

    assigned.start("Restarted", Difficulty::Crisis);

    assert(assigned.isStarted());
    assert(assigned.country().getName() == "Restarted");
    assert(assigned.country().getDifficulty() == Difficulty::Crisis);
    assert(assigned.history().size() == 0);
    assert(!assigned.lastEventText());

    Game eventGame("N", Difficulty::Collapsing);

    assert(eventGame.country().getInflation() == 32.0);

    eventGame.nextMonth();

    assert(eventGame.country().getTurn() == 1);
    assert(eventGame.history().size() == 1);
    assert(eventGame.history()[1].getEventId() == "EconomicEvent");
    assert(eventGame.lastEventText());

    Game finished("N", Difficulty::Unstable);

    finished.nextMonth(72);

    assert(finished.country().getTurn() == 72);
    assert(finished.isFinished());
    assert(finished.history().size() == 72);

    const double gdpAtFinish =
        finished.country().getGdp();

    const std::size_t historyAtFinish =
        finished.history().size();

    finished.nextMonth();

    assert(finished.country().getTurn() == 72);
    assert(finished.country().getGdp() == gdpAtFinish);
    assert(finished.history().size() == historyAtFinish);

    Game sameFinished("N", Difficulty::Unstable);

    sameFinished.nextMonth(72);

    assert(sameFinished == finished);
}