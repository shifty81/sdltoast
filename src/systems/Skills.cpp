#include "Skills.h"
#include <algorithm>

Skills::Skills() {
    // All skills start at level 0, 0 XP (default initialized by SkillData)
}

int Skills::SkillIndex(SkillType skill) const {
    return static_cast<int>(skill);
}

int Skills::GetLevel(SkillType skill) const {
    int idx = SkillIndex(skill);
    if (idx < 0 || idx >= SKILL_COUNT) return 0;
    return m_skills[idx].level;
}

int Skills::GetXP(SkillType skill) const {
    int idx = SkillIndex(skill);
    if (idx < 0 || idx >= SKILL_COUNT) return 0;
    return m_skills[idx].xp;
}

int Skills::GetXPForNextLevel(SkillType skill) const {
    int level = GetLevel(skill);
    if (level >= MAX_LEVEL) return 0;  // Already maxed
    return XPRequiredForLevel(level + 1) - GetXP(skill);
}

bool Skills::AddXP(SkillType skill, int amount) {
    int idx = SkillIndex(skill);
    if (idx < 0 || idx >= SKILL_COUNT) return false;
    if (amount <= 0) return false;

    auto& data = m_skills[idx];
    if (data.level >= MAX_LEVEL) return false;  // Already maxed

    data.xp += amount;

    // Check for level-ups (potentially multiple)
    bool leveledUp = false;
    while (data.level < MAX_LEVEL && data.xp >= XPRequiredForLevel(data.level + 1)) {
        data.level++;
        leveledUp = true;
    }

    return leveledUp;
}

std::string Skills::GetSkillName(SkillType skill) {
    switch (skill) {
        case SkillType::FARMING:  return "Farming";
        case SkillType::COMBAT:   return "Combat";
        case SkillType::MINING:   return "Mining";
        case SkillType::FISHING:  return "Fishing";
        case SkillType::FORAGING: return "Foraging";
        default:                  return "Unknown";
    }
}

int Skills::XPRequiredForLevel(int level) {
    // Cumulative XP required: level 1 = 100, level 2 = 300, level 3 = 600, ...
    // Formula: level * (level + 1) * 50
    if (level <= 0) return 0;
    return level * (level + 1) * 50;
}

void Skills::SetLevel(SkillType skill, int level) {
    int idx = SkillIndex(skill);
    if (idx < 0 || idx >= SKILL_COUNT) return;
    m_skills[idx].level = std::clamp(level, 0, MAX_LEVEL);
}

void Skills::SetXP(SkillType skill, int xp) {
    int idx = SkillIndex(skill);
    if (idx < 0 || idx >= SKILL_COUNT) return;
    m_skills[idx].xp = std::max(0, xp);
}
