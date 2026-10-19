#ifndef N_STATE_MONTH_RECORD_H
#define N_STATE_MONTH_RECORD_H

#include <string>

class MonthRecord
{
public:
    MonthRecord();
    MonthRecord(
        int turn,
        double gdp,
        double inflation,
        double unemployment,
        double support,
        const std::string& eventId
    );
    MonthRecord(const MonthRecord& other);
    ~MonthRecord();

    int getTurn() const;
    double getGdp() const;
    double getInflation() const;
    double getUnemployment() const;
    double getSupport() const;
    const std::string& getEventId() const;

    bool operator==(const MonthRecord& other) const;

private:
    int turn;
    double gdp;
    double inflation;
    double unemployment;
    double support;
    std::string eventId;
};

#endif
