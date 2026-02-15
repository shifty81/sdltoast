#ifndef QUEST_H
#define QUEST_H

#include <string>
#include <vector>

// A single objective within a quest
struct QuestObjective {
    std::string description;
    int requiredCount;
    int currentCount;

    bool IsComplete() const { return currentCount >= requiredCount; }
};

// Reward given on quest completion
struct QuestReward {
    std::string itemName;
    int quantity;
    int gold;
};

// Quest status
enum class QuestStatus { AVAILABLE, ACTIVE, COMPLETED, FAILED };

// A quest definition with objectives and rewards
struct Quest {
    std::string id;
    std::string title;
    std::string description;
    QuestStatus status;
    std::vector<QuestObjective> objectives;
    QuestReward reward;

    bool IsComplete() const;
};

class QuestSystem {
public:
    QuestSystem();

    // Add a new quest to the system
    void AddQuest(const Quest& quest);

    // Activate a quest by id (moves from AVAILABLE to ACTIVE)
    bool ActivateQuest(const std::string& id);

    // Update progress on a quest objective
    bool UpdateObjective(const std::string& questId, int objectiveIndex, int amount);

    // Check and mark quests as completed when all objectives are met
    void CheckCompletion(const std::string& questId);

    // Access
    const Quest* GetQuest(const std::string& id) const;
    std::vector<const Quest*> GetActiveQuests() const;
    std::vector<const Quest*> GetCompletedQuests() const;
    std::vector<const Quest*> GetAvailableQuests() const;
    int GetQuestCount() const { return static_cast<int>(m_quests.size()); }

private:
    std::vector<Quest> m_quests;

    Quest* FindQuest(const std::string& id);
    void InitQuests();
};

#endif // QUEST_H
