#include <cassert>

#include "../history/month_record.h"

void test_month_record()
{
    MonthRecord record;

    assert(record.getTurn() == 0);
    assert(record.getGdp() == 0.0);
    assert(record.getInflation() == 0.0);
    assert(record.getUnemployment() == 0.0);
    assert(record.getSupport() == 0.0);
    assert(record.getEventId() == "none");

    MonthRecord valid(
        5,
        12.5,
        7.5,
        4.0,
        60.0,
        "EconomicEvent"
    );

    assert(valid.getTurn() == 5);
    assert(valid.getGdp() == 12.5);
    assert(valid.getInflation() == 7.5);
    assert(valid.getUnemployment() == 4.0);
    assert(valid.getSupport() == 60.0);
    assert(valid.getEventId() == "EconomicEvent");

    MonthRecord invalid(-10, 1.0, 2.0, 3.0, 4.0, "");

    assert(invalid.getTurn() == 0);
    assert(invalid.getEventId() == "none");

    MonthRecord copied(valid);

    assert(copied == valid);
    assert(!(copied == invalid));

    MonthRecord sameAsValid(
        5,
        12.5,
        7.5,
        4.0,
        60.0,
        "EconomicEvent"
    );

    assert(valid == sameAsValid);
}