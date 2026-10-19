#ifndef N_STATE_COUNTRY_H
#define N_STATE_COUNTRY_H

#include "../core/entity.h"
#include "../core/types.h"
#include "funding.h"

#include <chrono>
#include <string>

class Country : public Entity
{
public:
    Country();
    Country(const std::string& name, Difficulty difficulty);
    Country(const Country& other);
    ~Country() override;

    std::string identify() const override;

    Difficulty getDifficulty() const;

    std::chrono::year_month_day getStartDate() const;
    std::chrono::year_month_day getCurrentDate() const;

    int getTurn() const;
    bool isFinished() const;

    double getGdp() const;
    double getBudgetExpenditure() const;
    double getInflation() const;
    double getUnemployment() const;
    double getGovernmentSupport() const;

    double getFunding(FundingArea area) const;
    double getInitialFunding(FundingArea area) const;
    const FundingTable& getFundingTable() const;

    bool changeFunding(FundingArea area, double multiplier);
    bool changeFunding(FundingArea area, int percent);

    void setGdp(double value);
    void setBudgetExpenditure(double value);
    void setInflation(double value);
    void setUnemployment(double value);
    void setGovernmentSupport(double value);

    void advanceMonth();
    void advanceMonth(int months);

    bool operator==(const Country& other) const;

    static int maxTurns();

private:
    void applyPreset(Difficulty difficulty);
    static double clampValue(
        double value,
        double minimum,
        double maximum
    );

    Difficulty difficulty;

    std::chrono::year_month_day startDate;
    std::chrono::year_month_day currentDate;

    int turn;

    double gdp;
    double budgetExpenditure;
    double inflation;
    double unemployment;
    double governmentSupport;

    FundingTable funding;
};

#endif
