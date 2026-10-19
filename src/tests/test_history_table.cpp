#include <cassert>
#include <stdexcept>

#include "../history/history_table.h"
#include "../history/month_record.h"

void test_history_table()
{
    HistoryTable<MonthRecord> table;

    assert(table.size() == 0);
    assert(!table.contains(1));

    MonthRecord record1(
        1,
        10.0,
        2.0,
        3.0,
        60.0,
        "none"
    );

    MonthRecord record2(
        33,
        20.0,
        4.0,
        5.0,
        50.0,
        "SocialEvent"
    );

    table.add(1, record1);
    table.add(33, record2);

    assert(table.size() == 2);
    assert(table.contains(1));
    assert(table.contains(33));

    assert(table[1] == record1);
    assert(table[33] == record2);

    table[1] = record2;
    assert(table[1] == record2);

    const HistoryTable<MonthRecord>& constTable = table;

    assert(constTable[1] == record2);

    bool exceptionThrown = false;

    try
    {
        table[999];
    }
    catch (const std::out_of_range&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    MonthRecord updated(
        1,
        100.0,
        10.0,
        5.0,
        70.0,
        "EconomicEvent"
    );

    table.add(1, updated);

    assert(table.size() == 2);
    assert(table[1] == updated);

    MonthRecord record3(
        3,
        30.0,
        6.0,
        7.0,
        40.0,
        "PoliticalEvent"
    );

    table << record3;

    assert(table.size() == 3);
    assert(table.contains(3));
    assert(table[3] == record3);

    assert(table.remove(33));
    assert(!table.contains(33));
    assert(table.size() == 2);

    assert(!table.remove(999));
    assert(table.size() == 2);

    HistoryTable<MonthRecord> copied(table);

    assert(copied == table);

    HistoryTable<MonthRecord> assigned;
    assigned.add(
        100,
        MonthRecord(100, 1.0, 1.0, 1.0, 1.0, "X")
    );

    assigned = table;

    assert(assigned == table);

    assigned = assigned;

    assert(assigned == table);

    HistoryTable<MonthRecord> different(table);

    different.add(
        1,
        MonthRecord(
            1,
            999.0,
            10.0,
            10.0,
            10.0,
            "different"
        )
    );

    assert(!(different == table));

    HistoryTable<MonthRecord> other;

    other.add(1, updated);
    other.add(
        3,
        MonthRecord(
            3,
            30.0,
            6.0,
            7.0,
            40.0,
            "PoliticalEvent"
        )
    );
    other.add(
        50,
        MonthRecord(
            50,
            50.0,
            5.0,
            5.0,
            50.0,
            "none"
        )
    );

    HistoryTable<MonthRecord> intersection = table && other;

    assert(intersection.size() == 2);
    assert(intersection.contains(1));
    assert(intersection.contains(3));
    assert(!intersection.contains(50));

    table.clear();

    assert(table.size() == 0);
    assert(!table.contains(1));
    assert(!table.contains(3));
}