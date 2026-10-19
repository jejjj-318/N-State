#include "funding.h"

#include <algorithm>

FundingTable::FundingTable()
{
    for (std::size_t i = 0; i < GameLimits::AREA_COUNT; ++i)
    {
        current[i] = 0.0;
        initial[i] = 0.0;
    }
}

FundingTable::FundingTable(const FundingTable& other)
{
    *this = other;
}

FundingTable::~FundingTable() = default;

int FundingTable::indexOf(FundingArea area)
{
    switch (area)
    {
        case FundingArea::Industry:
            return 0;
        case FundingArea::Agriculture:
            return 1;
        case FundingArea::Education:
            return 2;
        case FundingArea::Healthcare:
            return 3;
        case FundingArea::Defense:
            return 4;
        case FundingArea::Administration:
            return 5;
    }
    return 0;
}

double FundingTable::get(FundingArea area) const
{
    return current[indexOf(area)];
}

double FundingTable::getInitial(FundingArea area) const
{
    return initial[indexOf(area)];
}

bool FundingTable::set(FundingArea area, double value)
{
    if (value < 0.0)
    {
        return false;
    }
    current[indexOf(area)] = value;
    return true;
}

bool FundingTable::scale(FundingArea area, double multiplier)
{
    if (multiplier <= 0.0)
    {
        return false;
    }
    const int index = indexOf(area);
    current[index] *= multiplier;
    return true;
}

double FundingTable::total() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < GameLimits::AREA_COUNT; ++i)
    {
        sum += current[i];
    }
    return sum;
}

double FundingTable::initialTotal() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < GameLimits::AREA_COUNT; ++i)
    {
        sum += initial[i];
    }
    return sum;
}

double FundingTable::operator[](FundingArea area) const
{
    return get(area);
}

bool FundingTable::operator==(const FundingTable& other) const
{
    for (std::size_t i = 0; i < GameLimits::AREA_COUNT; ++i)
    {
        if (current[i] != other.current[i])
        {
            return false;
        }
        if (initial[i] != other.initial[i])
        {
            return false;
        }
    }
    return true;
}

FundingTable& FundingTable::operator=(const FundingTable& other)
{
    if (this == &other)
    {
        return *this;
    }
    for (std::size_t i = 0; i < GameLimits::AREA_COUNT; ++i)
    {
        current[i] = other.current[i];
        initial[i] = other.initial[i];
    }
    return *this;
}

void FundingTable::captureInitial()
{
    for (std::size_t i = 0; i < GameLimits::AREA_COUNT; ++i)
    {
        initial[i] = current[i];
    }
}
