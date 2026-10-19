#ifndef N_STATE_FUNDING_H
#define N_STATE_FUNDING_H

#include "../core/types.h"

class FundingTable
{
public:
    FundingTable();
    FundingTable(const FundingTable& other);
    ~FundingTable();

    double get(FundingArea area) const;
    double getInitial(FundingArea area) const;

    bool set(FundingArea area, double value);
    bool scale(FundingArea area, double multiplier);

    double total() const;
    double initialTotal() const;

    double operator[](FundingArea area) const;
    bool operator==(const FundingTable& other) const;
    FundingTable& operator=(const FundingTable& other);

    void captureInitial();

private:
    static int indexOf(FundingArea area);

    double current[GameLimits::AREA_COUNT];
    double initial[GameLimits::AREA_COUNT];
};

#endif
