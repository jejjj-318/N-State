#ifndef N_STATE_TYPES_H
#define N_STATE_TYPES_H

#include <cstddef>

enum class Difficulty
{
    Unstable,
    Crisis,
    Collapsing
};

enum class FundingArea
{
    Industry,
    Agriculture,
    Education,
    Healthcare,
    Defense,
    Administration
};

namespace GameLimits
{
    constexpr int MAX_TURNS = 72;
    constexpr double FUND_UP = 1.10;
    constexpr double FUND_DOWN = 0.90;
    constexpr std::size_t AREA_COUNT = 6;
}

#endif
