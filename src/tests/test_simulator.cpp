#include <cassert>
#include <cmath>
#include <memory>

#include "../simulation/simulator.h"

namespace
{
    bool almostEqual(double a, double b)
    {
        return std::abs(a - b) < 1e-9;
    }

    class TestSimulator final : public ISimulator
    {
    public:
        using ISimulator::advanceMonth;

        TestSimulator() = default;
        TestSimulator(const TestSimulator&) = default;
        ~TestSimulator() override = default;

        std::string identify() const override
        {
            return "TestSimulator";
        }

        void advanceMonth(Country& country) const override
        {
            country.advanceMonth();
        }
    };
}

void test_simulator()
{
    TestSimulator baseSimulator;

    assert(baseSimulator.identify() == "TestSimulator");

    Country country("N", Difficulty::Unstable);

    baseSimulator.advanceMonth(country);

    assert(country.getTurn() == 1);

    baseSimulator.advanceMonth(country, 3);

    assert(country.getTurn() == 4);

    baseSimulator.advanceMonth(country, 0);
    assert(country.getTurn() == 4);

    baseSimulator.advanceMonth(country, -10);
    assert(country.getTurn() == 4);

    TestSimulator copied(baseSimulator);
    assert(copied.identify() == "TestSimulator");

    std::unique_ptr<ISimulator> simulator =
        std::make_unique<MonthSimulator>();

    assert(simulator->identify() == "MonthSimulator");

    Country simulated("N", Difficulty::Unstable);

    assert(simulated.getTurn() == 0);
    assert(almostEqual(simulated.getGdp(), 1.20));
    assert(almostEqual(simulated.getInflation(), 5.0));
    assert(almostEqual(simulated.getUnemployment(), 6.0));
    assert(almostEqual(simulated.getGovernmentSupport(), 58.0));

    simulator->advanceMonth(simulated);

    assert(simulated.getTurn() == 1);
    assert(almostEqual(simulated.getGdp(), 1.2024));
    assert(almostEqual(simulated.getInflation(), 5.2));
    assert(almostEqual(simulated.getUnemployment(), 6.0));
    assert(almostEqual(simulated.getGovernmentSupport(), 57.98));

    simulator->advanceMonth(simulated, 2);

    assert(simulated.getTurn() == 3);
}