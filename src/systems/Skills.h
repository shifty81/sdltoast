#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include <array>

/**
 * Skill types — each tracks XP and level independently.
 */
enum class SkillType { FARMING, COMBAT, MINING, FISHING, FORAGING, COUNT };

/**
 * Skill progression system.
 * Each skill goes from level 0 to MAX_LEVEL.
 * XP thresholds increase with each level.
 */
class Skills {
public:
    Skills();

    // Query
    int GetLevel(SkillType skill) const;
    int GetXP(SkillType skill) const;
    int GetXPForNextLevel(SkillType skill) const;

    // Add XP to a skill; returns true if the skill leveled up
    bool AddXP(SkillType skill, int amount);

    // Static helpers
    static std::string GetSkillName(SkillType skill);
    static int XPRequiredForLevel(int level);

    // Setters for save/load
    void SetLevel(SkillType skill, int level);
    void SetXP(SkillType skill, int xp);

    // Constants
    static constexpr int MAX_LEVEL = 10;
    static constexpr int SKILL_COUNT = static_cast<int>(SkillType::COUNT);

private:
    struct SkillData {
        int level = 0;
        int xp = 0;
    };

    int SkillIndex(SkillType skill) const;

    std::array<SkillData, static_cast<size_t>(SkillType::COUNT)> m_skills;
};

#endif // SKILLS_H
