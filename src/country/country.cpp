#include "country.h"

#include <chrono>

namespace
{
    std::chrono::year_month_day getToday()
    {
        using namespace std::chrono;

        const auto now = system_clock::now();
        const auto today = floor<days>(now);

        return year_month_day{today};
    }
}

Country::Country()
    : Country("N", Difficulty::Unstable)
{
}

Country::Country(
    const std::string& name,
    Difficulty difficulty
)
    : Entity(name),
      difficulty(difficulty),
      startDate(getToday()),
      currentDate(startDate),
      turn(0),
      gdp(1.0),
      budgetExpenditure(0.0),
      inflation(0.0),
      unemployment(0.0),
      governmentSupport(0.0)
{
    applyPreset(difficulty);
}

Country::Country(const Country& other)
    : Entity(other),
      difficulty(other.difficulty),
      startDate(other.startDate),
      currentDate(other.currentDate),
      turn(other.turn),
      gdp(other.gdp),
      budgetExpenditure(other.budgetExpenditure),
      inflation(other.inflation),
      unemployment(other.unemployment),
      governmentSupport(other.governmentSupport),
      funding(other.funding)
{
}

Country::~Country() = default;

std::string Country::identify() const
{
    return "Country";
}

void Country::applyPreset(Difficulty difficulty)
{
    switch (difficulty)
    {
        case Difficulty::Unstable:
            gdp = 1.20;
            inflation = 5.0;
            unemployment = 6.0;
            governmentSupport = 58.0;
            funding.set(FundingArea::Industry, 0.06);
            funding.set(FundingArea::Agriculture, 0.04);
            funding.set(FundingArea::Education, 0.05);
            funding.set(FundingArea::Healthcare, 0.05);
            funding.set(FundingArea::Defense, 0.05);
            funding.set(FundingArea::Administration, 0.04);
            break;

        case Difficulty::Crisis:
            gdp = 1.05;
            inflation = 14.0;
            unemployment = 12.0;
            governmentSupport = 43.0;
            funding.set(FundingArea::Industry, 0.05);
            funding.set(FundingArea::Agriculture, 0.04);
            funding.set(FundingArea::Education, 0.04);
            funding.set(FundingArea::Healthcare, 0.05);
            funding.set(FundingArea::Defense, 0.06);
            funding.set(FundingArea::Administration, 0.05);
            break;

        case Difficulty::Collapsing:
            gdp = 0.75;
            inflation = 32.0;
            unemployment = 23.0;
            governmentSupport = 27.0;
            funding.set(FundingArea::Industry, 0.03);
            funding.set(FundingArea::Agriculture, 0.03);
            funding.set(FundingArea::Education, 0.03);
            funding.set(FundingArea::Healthcare, 0.04);
            funding.set(FundingArea::Defense, 0.07);
            funding.set(FundingArea::Administration, 0.04);
            break;
    }

    funding.captureInitial();
    budgetExpenditure = funding.total();
}

Difficulty Country::getDifficulty() const
{
    return difficulty;
}

std::chrono::year_month_day Country::getStartDate() const
{
    return startDate;
}

std::chrono::year_month_day Country::getCurrentDate() const
{
    return currentDate;
}

int Country::getTurn() const
{
    return turn;
}

bool Country::isFinished() const
{
    return turn >= GameLimits::MAX_TURNS;
}

double Country::getGdp() const
{
    return gdp;
}

double Country::getBudgetExpenditure() const
{
    return budgetExpenditure;
}

double Country::getInflation() const
{
    return inflation;
}

double Country::getUnemployment() const
{
    return unemployment;
}

double Country::getGovernmentSupport() const
{
    return governmentSupport;
}

double Country::getFunding(FundingArea area) const
{
    return funding.get(area);
}

double Country::getInitialFunding(FundingArea area) const
{
    return funding.getInitial(area);
}

const FundingTable& Country::getFundingTable() const
{
    return funding;
}

bool Country::changeFunding(
    FundingArea area,
    double multiplier
)
{
    if (!funding.scale(area, multiplier))
    {
        return false;
    }
    budgetExpenditure = funding.total();
    return true;
}

bool Country::changeFunding(
    FundingArea area,
    int percent
)
{
    if (percent <= -100)
    {
        return false;
    }
    const double multiplier =
        1.0 + static_cast<double>(percent) / 100.0;
    return changeFunding(area, multiplier);
}

void Country::setGdp(double value)
{
    gdp = clampValue(value, 0.01, 1000.0);
}

void Country::setBudgetExpenditure(double value)
{
    budgetExpenditure = clampValue(value, 0.0, 100.0);
}

void Country::setInflation(double value)
{
    inflation = clampValue(value, 0.0, 100.0);
}

void Country::setUnemployment(double value)
{
    unemployment = clampValue(value, 0.0, 100.0);
}

void Country::setGovernmentSupport(double value)
{
    governmentSupport = clampValue(value, 0.0, 100.0);
}

void Country::advanceMonth()
{
    using namespace std::chrono;
    currentDate += months{1};
    ++turn;
}

void Country::advanceMonth(int months)
{
    if (months <= 0)
    {
        return;
    }
    for (int i = 0; i < months; ++i)
    {
        advanceMonth();
    }
}

bool Country::operator==(const Country& other) const
{
    return getName() == other.getName() &&
           difficulty == other.difficulty &&
           turn == other.turn &&
           gdp == other.gdp &&
           funding == other.funding;
}

int Country::maxTurns()
{
    return GameLimits::MAX_TURNS;
}

double Country::clampValue(
    double value,
    double minimum,
    double maximum
)
{
    if (value < minimum)
    {
        return minimum;
    }
    if (value > maximum)
    {
        return maximum;
    }
    return value;
}
