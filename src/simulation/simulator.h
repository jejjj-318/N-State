#ifndef N_STATE_SIMULATOR_H
#define N_STATE_SIMULATOR_H

#include "../country/country.h"

#include <string>

class ISimulator
{
public:
    ISimulator();
    ISimulator(const ISimulator& other);
    virtual ~ISimulator();

    virtual std::string identify() const = 0;
    virtual void advanceMonth(Country& country) const = 0;
    virtual void advanceMonth(Country& country, int months)
        const;
};

class MonthSimulator final : public ISimulator
{
public:
    MonthSimulator();
    MonthSimulator(const MonthSimulator& other);
    ~MonthSimulator() override;

    std::string identify() const override;
    void advanceMonth(Country& country) const override;
};

#endif
