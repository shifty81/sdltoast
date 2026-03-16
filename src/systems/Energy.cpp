#include "Energy.h"
#include <algorithm>

Energy::Energy()
    : m_current(DEFAULT_MAX)
    , m_max(DEFAULT_MAX)
{
}

bool Energy::Consume(int amount) {
    if (amount <= 0) return true;
    if (m_current < amount) return false;
    m_current -= amount;
    return true;
}

void Energy::Restore(int amount) {
    if (amount <= 0) return;
    m_current = std::min(m_current + amount, m_max);
}

void Energy::RestoreFull() {
    m_current = m_max;
}

void Energy::IncreaseMax(int amount) {
    if (amount <= 0) return;
    m_max += amount;
    // Also top up current by the same amount
    m_current = std::min(m_current + amount, m_max);
}

void Energy::SetCurrent(int current) {
    m_current = std::clamp(current, 0, m_max);
}

void Energy::SetMax(int max) {
    if (max < 1) max = 1;
    m_max = max;
    if (m_current > m_max) m_current = m_max;
}
