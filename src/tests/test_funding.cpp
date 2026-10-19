#include <cassert>
#include <cmath>

#include "../country/funding.h"

namespace
{
    bool almostEqual(double a, double b)
    {
        return std::abs(a - b) < 1e-9;
    }
}

void test_funding()
{
    FundingTable table;

    assert(almostEqual(table.total(), 0.0));
    assert(almostEqual(table.initialTotal(), 0.0));

    // Проверяем все шесть направлений.
    assert(table.set(FundingArea::Industry, 0.10));
    assert(table.set(FundingArea::Agriculture, 0.20));
    assert(table.set(FundingArea::Education, 0.30));
    assert(table.set(FundingArea::Healthcare, 0.40));
    assert(table.set(FundingArea::Defense, 0.50));
    assert(table.set(FundingArea::Administration, 0.60));

    assert(almostEqual(table.get(FundingArea::Industry), 0.10));
    assert(almostEqual(table.get(FundingArea::Agriculture), 0.20));
    assert(almostEqual(table.get(FundingArea::Education), 0.30));
    assert(almostEqual(table.get(FundingArea::Healthcare), 0.40));
    assert(almostEqual(table.get(FundingArea::Defense), 0.50));
    assert(almostEqual(table.get(FundingArea::Administration), 0.60));

    assert(almostEqual(
        table[FundingArea::Industry],
        0.10
    ));

    assert(almostEqual(table.total(), 2.10));

    // Невалидное значение.
    assert(!table.set(FundingArea::Industry, -1.0));
    assert(almostEqual(table.get(FundingArea::Industry), 0.10));

    // Снимок начального финансирования.
    table.captureInitial();

    assert(almostEqual(
        table.getInitial(FundingArea::Industry),
        0.10
    ));
    assert(almostEqual(
        table.getInitial(FundingArea::Administration),
        0.60
    ));
    assert(almostEqual(table.initialTotal(), 2.10));

    // Масштабирование.
    assert(table.scale(FundingArea::Industry, 2.0));
    assert(almostEqual(table.get(FundingArea::Industry), 0.20));
    assert(almostEqual(table.total(), 2.20));

    // Initial не должен измениться после scale.
    assert(almostEqual(
        table.getInitial(FundingArea::Industry),
        0.10
    ));
    assert(almostEqual(table.initialTotal(), 2.10));

    // Невалидные коэффициенты.
    assert(!table.scale(FundingArea::Industry, 0.0));
    assert(!table.scale(FundingArea::Industry, -1.0));
    assert(almostEqual(table.get(FundingArea::Industry), 0.20));

    // Copy constructor.
    FundingTable copied(table);
    assert(copied == table);

    // Assignment operator.
    FundingTable assigned;
    assert(assigned.set(FundingArea::Defense, 9.0));

    assigned = table;

    assert(assigned == table);

    // Self-assignment.
    assigned = assigned;
    assert(assigned == table);

    // Equality / inequality.
    FundingTable different(table);

    assert(different.set(FundingArea::Healthcare, 999.0));
    assert(!(different == table));
}
