#include "Calendar.h"

Calendar::Calendar()
    : m_day(1)
    , m_season(Season::SPRING)
    , m_year(1)
{
}

void Calendar::AdvanceDay() {
    m_day++;
    if (m_day > DAYS_PER_SEASON) {
        m_day = 1;
        m_season = static_cast<Season>((static_cast<int>(m_season) + 1) % 4);
        if (m_season == Season::SPRING) {
            m_year++;
        }
    }
}

std::string Calendar::GetSeasonName() const {
    switch (m_season) {
        case Season::SPRING: return "Spring";
        case Season::SUMMER: return "Summer";
        case Season::FALL:   return "Fall";
        case Season::WINTER: return "Winter";
        default: return "Unknown";
    }
}
