#include <cassert>

#include "../country/country.h"

void test_country()
{
    Country defaultCountry;

    assert(defaultCountry.getName() == "N");
    assert(defaultCountry.identify() == "Country");
    assert(defaultCountry.getDifficulty() == Difficulty::Unstable);
    assert(defaultCountry.getTurn() == 0);
    assert(!defaultCountry.isFinished());

    assert(defaultCountry.getGdp() == 1.20);
    assert(defaultCountry.getInflation() == 5.0);
    assert(defaultCountry.getUnemployment() == 6.0);
    assert(defaultCountry.getGovernmentSupport() == 58.0);

    assert(defaultCountry.getFunding(FundingArea::Industry) == 0.06);
    assert(defaultCountry.getFunding(FundingArea::Agriculture) == 0.04);
    assert(defaultCountry.getFunding(FundingArea::Education) == 0.05);
    assert(defaultCountry.getFunding(FundingArea::Healthcare) == 0.05);
    assert(defaultCountry.getFunding(FundingArea::Defense) == 0.05);
    assert(defaultCountry.getFunding(FundingArea::Administration) == 0.04);

    assert(defaultCountry.getFundingTable().total() == 0.29);
    assert(defaultCountry.getFundingTable().initialTotal() == 0.29);

    Country crisis("Crisis", Difficulty::Crisis);

    assert(crisis.getName() == "Crisis");
    assert(crisis.getDifficulty() == Difficulty::Crisis);
    assert(crisis.getGdp() == 1.05);
    assert(crisis.getInflation() == 14.0);
    assert(crisis.getUnemployment() == 12.0);
    assert(crisis.getGovernmentSupport() == 43.0);

    Country collapsing("Collapse", Difficulty::Collapsing);

    assert(collapsing.getDifficulty() == Difficulty::Collapsing);
    assert(collapsing.getGdp() == 0.75);
    assert(collapsing.getInflation() == 32.0);
    assert(collapsing.getUnemployment() == 23.0);
    assert(collapsing.getGovernmentSupport() == 27.0);

    assert(defaultCountry.getStartDate() ==
           defaultCountry.getCurrentDate());

    const auto startDate = defaultCountry.getCurrentDate();

    defaultCountry.advanceMonth();

    assert(defaultCountry.getTurn() == 1);
    assert(defaultCountry.getCurrentDate() != startDate);
    assert(!defaultCountry.isFinished());

    const int turnBefore = defaultCountry.getTurn();

    defaultCountry.advanceMonth(0);
    assert(defaultCountry.getTurn() == turnBefore);

    defaultCountry.advanceMonth(-5);
    assert(defaultCountry.getTurn() == turnBefore);

    defaultCountry.advanceMonth(2);
    assert(defaultCountry.getTurn() == 3);

    assert(defaultCountry.changeFunding(
        FundingArea::Industry,
        2.0
    ));

    assert(defaultCountry.getFunding(FundingArea::Industry) == 0.12);
    assert(defaultCountry.getBudgetExpenditure() == 0.35);

    assert(defaultCountry.changeFunding(
        FundingArea::Agriculture,
        100
    ));

    assert(defaultCountry.getFunding(FundingArea::Agriculture) == 0.08);

    assert(!defaultCountry.changeFunding(
        FundingArea::Industry,
        -100
    ));

    assert(!defaultCountry.changeFunding(
        FundingArea::Industry,
        -2.0
    ));

    defaultCountry.setGdp(-10.0);
    assert(defaultCountry.getGdp() == 0.01);

    defaultCountry.setGdp(2000.0);
    assert(defaultCountry.getGdp() == 1000.0);

    defaultCountry.setBudgetExpenditure(-1.0);
    assert(defaultCountry.getBudgetExpenditure() == 0.0);

    defaultCountry.setBudgetExpenditure(200.0);
    assert(defaultCountry.getBudgetExpenditure() == 100.0);

    defaultCountry.setInflation(-1.0);
    assert(defaultCountry.getInflation() == 0.0);

    defaultCountry.setInflation(200.0);
    assert(defaultCountry.getInflation() == 100.0);

    defaultCountry.setUnemployment(-1.0);
    assert(defaultCountry.getUnemployment() == 0.0);

    defaultCountry.setUnemployment(200.0);
    assert(defaultCountry.getUnemployment() == 100.0);

    defaultCountry.setGovernmentSupport(-1.0);
    assert(defaultCountry.getGovernmentSupport() == 0.0);

    defaultCountry.setGovernmentSupport(200.0);
    assert(defaultCountry.getGovernmentSupport() == 100.0);

    Country copied(crisis);

    assert(copied == crisis);

    copied.setGdp(999.0);
    assert(!(copied == crisis));

    assert(Country::maxTurns() == 72);

    Country finished("Finished", Difficulty::Unstable);

    finished.advanceMonth(Country::maxTurns());

    assert(finished.getTurn() == Country::maxTurns());
    assert(finished.isFinished());
}