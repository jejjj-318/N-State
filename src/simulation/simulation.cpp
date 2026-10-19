#include "simulation.h"

#include <algorithm>

namespace
{
    double deviation(
        const Country& country,
        FundingArea area
    )
    {
        const double initial = country.getInitialFunding(area);

        if (initial <= 0.0)
        {
            return 0.0;
        }

        return country.getFunding(area) / initial - 1.0;
    }

    double clamp(
        double value,
        double minimum,
        double maximum
    )
    {
        return std::max(minimum, std::min(value, maximum));
    }
}

void Simulation::advanceMonth(Country& country)
{
    const double industry = deviation(country, FundingArea::Industry);
    const double agriculture = deviation(country, FundingArea::Agriculture);
    const double education = deviation(country, FundingArea::Education);
    const double healthcare = deviation(country, FundingArea::Healthcare);
    const double defense = deviation(country, FundingArea::Defense);
    const double administration = deviation(
        country,
        FundingArea::Administration
    );

    const double initialBudget =
        country.getInitialFunding(FundingArea::Industry) +
        country.getInitialFunding(FundingArea::Agriculture) +
        country.getInitialFunding(FundingArea::Education) +
        country.getInitialFunding(FundingArea::Healthcare) +
        country.getInitialFunding(FundingArea::Defense) +
        country.getInitialFunding(FundingArea::Administration);

    const double budgetDeviation =
        initialBudget > 0.0
            ? country.getBudgetExpenditure() / initialBudget - 1.0
            : 0.0;

    const double gdpGrowth =
        0.20 +
        0.45 * industry +
        0.25 * agriculture +
        0.15 * education +
        0.15 * healthcare +
        0.05 * defense +
        0.10 * administration;

    const double inflationChange =
        0.20 +
        0.70 * budgetDeviation;

    const double unemploymentChange =
        -0.30 * industry
        -0.15 * agriculture
        -0.10 * education
        -0.10 * healthcare
        +0.05 * defense
        +0.05 * administration;

    const double supportChange =
        0.20 * education
        +0.20 * healthcare
        +0.10 * administration
        -0.10 * inflationChange;

    country.setGdp(
        std::max(
            0.01,
            country.getGdp() * (1.0 + gdpGrowth / 100.0)
        )
    );

    country.setInflation(
        clamp(
            country.getInflation() + inflationChange,
            0.0,
            100.0
        )
    );

    country.setUnemployment(
        clamp(
            country.getUnemployment() + unemploymentChange,
            0.0,
            100.0
        )
    );

    country.setGovernmentSupport(
        clamp(
            country.getGovernmentSupport() + supportChange,
            0.0,
            100.0
        )
    );

    country.advanceMonth();
}
