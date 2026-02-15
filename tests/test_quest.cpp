// Harvest Quest — Quest system unit tests

#include "systems/Quest.h"
#include <cassert>
#include <iostream>

static int s_passed = 0;
static int s_failed = 0;

#define TEST(name) static void name()
#define RUN_TEST(name) do { \
    std::cout << "  " #name "... "; \
    try { name(); std::cout << "PASS" << std::endl; s_passed++; } \
    catch (...) { std::cout << "FAIL" << std::endl; s_failed++; } \
} while(0)
#define ASSERT_TRUE(expr)  do { if (!(expr)) throw 1; } while(0)
#define ASSERT_FALSE(expr) do { if (expr) throw 1; } while(0)
#define ASSERT_EQ(a, b)    do { if ((a) != (b)) throw 1; } while(0)

TEST(test_initial_quests_loaded) {
    QuestSystem qs;
    ASSERT_TRUE(qs.GetQuestCount() >= 6);
}

TEST(test_quest_starts_available) {
    QuestSystem qs;
    const Quest* q = qs.GetQuest("farm_beginnings");
    ASSERT_TRUE(q != nullptr);
    ASSERT_EQ(q->status, QuestStatus::AVAILABLE);
}

TEST(test_activate_quest) {
    QuestSystem qs;
    ASSERT_TRUE(qs.ActivateQuest("farm_beginnings"));
    const Quest* q = qs.GetQuest("farm_beginnings");
    ASSERT_EQ(q->status, QuestStatus::ACTIVE);
}

TEST(test_activate_nonexistent_quest) {
    QuestSystem qs;
    ASSERT_FALSE(qs.ActivateQuest("nonexistent"));
}

TEST(test_activate_already_active) {
    QuestSystem qs;
    qs.ActivateQuest("farm_beginnings");
    ASSERT_FALSE(qs.ActivateQuest("farm_beginnings"));
}

TEST(test_update_objective) {
    QuestSystem qs;
    qs.ActivateQuest("farm_beginnings");
    ASSERT_TRUE(qs.UpdateObjective("farm_beginnings", 0, 3));
    const Quest* q = qs.GetQuest("farm_beginnings");
    ASSERT_EQ(q->objectives[0].currentCount, 3);
    ASSERT_EQ(q->status, QuestStatus::ACTIVE);
}

TEST(test_update_objective_inactive_quest) {
    QuestSystem qs;
    ASSERT_FALSE(qs.UpdateObjective("farm_beginnings", 0, 1));
}

TEST(test_update_objective_invalid_index) {
    QuestSystem qs;
    qs.ActivateQuest("farm_beginnings");
    ASSERT_FALSE(qs.UpdateObjective("farm_beginnings", 99, 1));
}

TEST(test_quest_completion) {
    QuestSystem qs;
    qs.ActivateQuest("farm_beginnings");
    qs.UpdateObjective("farm_beginnings", 0, 5);
    const Quest* q = qs.GetQuest("farm_beginnings");
    ASSERT_EQ(q->status, QuestStatus::COMPLETED);
    ASSERT_TRUE(q->IsComplete());
}

TEST(test_objective_clamped_to_required) {
    QuestSystem qs;
    qs.ActivateQuest("farm_beginnings");
    qs.UpdateObjective("farm_beginnings", 0, 100);
    const Quest* q = qs.GetQuest("farm_beginnings");
    ASSERT_EQ(q->objectives[0].currentCount, q->objectives[0].requiredCount);
}

TEST(test_get_active_quests) {
    QuestSystem qs;
    qs.ActivateQuest("farm_beginnings");
    qs.ActivateQuest("monster_slayer");
    auto active = qs.GetActiveQuests();
    ASSERT_EQ(static_cast<int>(active.size()), 2);
}

TEST(test_get_completed_quests) {
    QuestSystem qs;
    qs.ActivateQuest("farm_beginnings");
    qs.UpdateObjective("farm_beginnings", 0, 5);
    auto completed = qs.GetCompletedQuests();
    ASSERT_EQ(static_cast<int>(completed.size()), 1);
}

TEST(test_get_available_quests) {
    QuestSystem qs;
    auto available = qs.GetAvailableQuests();
    ASSERT_EQ(static_cast<int>(available.size()), qs.GetQuestCount());
}

TEST(test_add_custom_quest) {
    QuestSystem qs;
    int before = qs.GetQuestCount();
    Quest custom;
    custom.id = "custom_quest";
    custom.title = "Custom Quest";
    custom.description = "A custom test quest.";
    custom.status = QuestStatus::AVAILABLE;
    custom.objectives = {{"Do something", 1, 0}};
    custom.reward = {"Wood", 10, 0};
    qs.AddQuest(custom);
    ASSERT_EQ(qs.GetQuestCount(), before + 1);
    ASSERT_TRUE(qs.GetQuest("custom_quest") != nullptr);
}

TEST(test_quest_is_complete_no_objectives) {
    Quest q;
    q.id = "empty";
    ASSERT_FALSE(q.IsComplete());
}

int main() {
    std::cout << "=== Quest Tests ===" << std::endl;
    RUN_TEST(test_initial_quests_loaded);
    RUN_TEST(test_quest_starts_available);
    RUN_TEST(test_activate_quest);
    RUN_TEST(test_activate_nonexistent_quest);
    RUN_TEST(test_activate_already_active);
    RUN_TEST(test_update_objective);
    RUN_TEST(test_update_objective_inactive_quest);
    RUN_TEST(test_update_objective_invalid_index);
    RUN_TEST(test_quest_completion);
    RUN_TEST(test_objective_clamped_to_required);
    RUN_TEST(test_get_active_quests);
    RUN_TEST(test_get_completed_quests);
    RUN_TEST(test_get_available_quests);
    RUN_TEST(test_add_custom_quest);
    RUN_TEST(test_quest_is_complete_no_objectives);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
