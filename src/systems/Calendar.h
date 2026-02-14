#ifndef CALENDAR_H
#define CALENDAR_H

#include <string>

enum class Season { SPRING, SUMMER, FALL, WINTER };

class Calendar {
public:
    Calendar();
    
    void AdvanceDay();
    int GetDay() const { return m_day; }
    Season GetSeason() const { return m_season; }
    int GetYear() const { return m_year; }
    std::string GetSeasonName() const;

    // Setters for save/load
    void SetDay(int day) { m_day = day; }
    void SetSeason(Season season) { m_season = season; }
    void SetYear(int year) { m_year = year; }

private:
    int m_day;
    Season m_season;
    int m_year;
    static constexpr int DAYS_PER_SEASON = 28;
};

#endif // CALENDAR_H
