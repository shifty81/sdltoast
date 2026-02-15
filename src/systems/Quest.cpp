#include "Quest.h"
#include <algorithm>

bool Quest::IsComplete() const {
    if (objectives.empty()) return false;
    for (const auto& obj : objectives) {
        if (!obj.IsComplete()) return false;
    }
    return true;
}

QuestSystem::QuestSystem() {
    InitQuests();
}

void QuestSystem::InitQuests() {
    // Built-in quests are hardcoded for now, matching data/quests.json.
    // Future: load from JSON data file for data-driven configuration.

    // Quest 0: Farm Beginnings — Harvest your first crops
    m_quests.push_back({
        "farm_beginnings",
        "Farm Beginnings",
        "Harvest your first crops to get the farm started.",
        QuestStatus::AVAILABLE,
        {{"Harvest 5 crops", 5, 0}},
        {"Parsnip Soup", 1, 50}
    });

    // Quest 1: Monster Slayer — Defeat enemies in the dungeon
    m_quests.push_back({
        "monster_slayer",
        "Monster Slayer",
        "Prove your combat skills by defeating enemies.",
        QuestStatus::AVAILABLE,
        {{"Defeat 10 enemies", 10, 0}},
        {"", 0, 200}
    });

    // Quest 2: Lumberjack — Chop trees for wood
    m_quests.push_back({
        "lumberjack",
        "Lumberjack",
        "Gather wood by chopping trees around the farm.",
        QuestStatus::AVAILABLE,
        {{"Collect 20 Wood", 20, 0}},
        {"Fence", 3, 25}
    });

    // Quest 3: Stone Collector — Mine stone
    m_quests.push_back({
        "stone_collector",
        "Stone Collector",
        "Collect stone from rocks scattered across the land.",
        QuestStatus::AVAILABLE,
        {{"Collect 15 Stone", 15, 0}},
        {"Stone Wall", 2, 25}
    });

    // Quest 4: Master Crafter — Craft items
    m_quests.push_back({
        "master_crafter",
        "Master Crafter",
        "Prove your crafting prowess by crafting several items.",
        QuestStatus::AVAILABLE,
        {{"Craft 3 items", 3, 0}},
        {"Sprinkler", 1, 100}
    });

    // Quest 5: Community Helper — Talk to all villagers
    m_quests.push_back({
        "community_helper",
        "Community Helper",
        "Introduce yourself to the villagers of Meadowbrook.",
        QuestStatus::AVAILABLE,
        {{"Talk to 3 NPCs", 3, 0}},
        {"", 0, 150}
    });
}

void QuestSystem::AddQuest(const Quest& quest) {
    m_quests.push_back(quest);
}

Quest* QuestSystem::FindQuest(const std::string& id) {
    for (auto& q : m_quests) {
        if (q.id == id) return &q;
    }
    return nullptr;
}

const Quest* QuestSystem::GetQuest(const std::string& id) const {
    for (const auto& q : m_quests) {
        if (q.id == id) return &q;
    }
    return nullptr;
}

bool QuestSystem::ActivateQuest(const std::string& id) {
    Quest* q = FindQuest(id);
    if (!q) return false;
    if (q->status != QuestStatus::AVAILABLE) return false;
    q->status = QuestStatus::ACTIVE;
    return true;
}

bool QuestSystem::UpdateObjective(const std::string& questId, int objectiveIndex, int amount) {
    Quest* q = FindQuest(questId);
    if (!q) return false;
    if (q->status != QuestStatus::ACTIVE) return false;
    if (objectiveIndex < 0 || objectiveIndex >= static_cast<int>(q->objectives.size())) return false;

    q->objectives[objectiveIndex].currentCount += amount;
    if (q->objectives[objectiveIndex].currentCount > q->objectives[objectiveIndex].requiredCount) {
        q->objectives[objectiveIndex].currentCount = q->objectives[objectiveIndex].requiredCount;
    }

    CheckCompletion(questId);
    return true;
}

void QuestSystem::CheckCompletion(const std::string& questId) {
    Quest* q = FindQuest(questId);
    if (!q) return;
    if (q->status != QuestStatus::ACTIVE) return;
    if (q->IsComplete()) {
        q->status = QuestStatus::COMPLETED;
    }
}

std::vector<const Quest*> QuestSystem::GetActiveQuests() const {
    std::vector<const Quest*> result;
    for (const auto& q : m_quests) {
        if (q.status == QuestStatus::ACTIVE) result.push_back(&q);
    }
    return result;
}

std::vector<const Quest*> QuestSystem::GetCompletedQuests() const {
    std::vector<const Quest*> result;
    for (const auto& q : m_quests) {
        if (q.status == QuestStatus::COMPLETED) result.push_back(&q);
    }
    return result;
}

std::vector<const Quest*> QuestSystem::GetAvailableQuests() const {
    std::vector<const Quest*> result;
    for (const auto& q : m_quests) {
        if (q.status == QuestStatus::AVAILABLE) result.push_back(&q);
    }
    return result;
}
