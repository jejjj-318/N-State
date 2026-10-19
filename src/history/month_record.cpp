#include "month_record.h"

MonthRecord::MonthRecord()
    : turn(0),
      gdp(0.0),
      inflation(0.0),
      unemployment(0.0),
      support(0.0),
      eventId("none")
{
}

MonthRecord::MonthRecord(
    int turn,
    double gdp,
    double inflation,
    double unemployment,
    double support,
    const std::string& eventId
)
    : turn(turn < 0 ? 0 : turn),
      gdp(gdp),
      inflation(inflation),
      unemployment(unemployment),
      support(support),
      eventId(eventId.empty() ? "none" : eventId)
{
}

MonthRecord::MonthRecord(const MonthRecord& other)
    : turn(other.turn),
      gdp(other.gdp),
      inflation(other.inflation),
      unemployment(other.unemployment),
      support(other.support),
      eventId(other.eventId)
{
}

MonthRecord::~MonthRecord() = default;

int MonthRecord::getTurn() const
{
    return turn;
}

double MonthRecord::getGdp() const
{
    return gdp;
}

double MonthRecord::getInflation() const
{
    return inflation;
}

double MonthRecord::getUnemployment() const
{
    return unemployment;
}

double MonthRecord::getSupport() const
{
    return support;
}

const std::string& MonthRecord::getEventId() const
{
    return eventId;
}

bool MonthRecord::operator==(const MonthRecord& other) const
{
    return turn == other.turn &&
           gdp == other.gdp &&
           inflation == other.inflation &&
           unemployment == other.unemployment &&
           support == other.support &&
           eventId == other.eventId;
}
