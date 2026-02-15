// Harvest Quest — NPC system unit tests
// Tests friendship, schedule, and proximity detection
// (NPC logic only; Render stubs provided via engine linking)

#include "entities/NPC.h"
#include <cassert>
#include <iostream>
#include <cmath>

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

// ---- Basic NPC properties ----

TEST(test_default_name) {
    NPC npc;
    ASSERT_EQ(npc.GetName(), "NPC");
}

TEST(test_set_name) {
    NPC npc;
    npc.SetName("Farmer Gus");
    ASSERT_EQ(npc.GetName(), "Farmer Gus");
}

TEST(test_npc_is_active_by_default) {
    NPC npc;
    ASSERT_TRUE(npc.IsActive());
}

TEST(test_npc_default_size) {
    NPC npc;
    float w, h;
    npc.GetSize(w, h);
    ASSERT_EQ(static_cast<int>(w), 32);
    ASSERT_EQ(static_cast<int>(h), 32);
}

// ---- Friendship system ----

TEST(test_initial_friendship) {
    NPC npc;
    ASSERT_EQ(npc.GetFriendship(), 0);
}

TEST(test_add_friendship) {
    NPC npc;
    npc.AddFriendship(3);
    ASSERT_EQ(npc.GetFriendship(), 3);
}

TEST(test_add_friendship_accumulates) {
    NPC npc;
    npc.AddFriendship(2);
    npc.AddFriendship(3);
    ASSERT_EQ(npc.GetFriendship(), 5);
}

TEST(test_friendship_cap_at_max) {
    NPC npc;
    npc.AddFriendship(15); // MAX_FRIENDSHIP is 10
    ASSERT_EQ(npc.GetFriendship(), 10);
}

TEST(test_friendship_no_negative) {
    NPC npc;
    npc.AddFriendship(3);
    npc.AddFriendship(-5);
    ASSERT_EQ(npc.GetFriendship(), 0);
}

TEST(test_friendship_add_then_cap) {
    NPC npc;
    npc.AddFriendship(8);
    npc.AddFriendship(5); // 8 + 5 = 13 -> capped to 10
    ASSERT_EQ(npc.GetFriendship(), 10);
}

// ---- Schedule system ----

TEST(test_schedule_sets_destination) {
    NPC npc;
    npc.SetPosition(100.0f, 100.0f);
    npc.AddScheduleEntry(8, 200.0f, 200.0f);
    npc.SetCurrentHour(8);
    // NPC should now be moving toward (200, 200)
    // After one update, it should have moved closer
    float x1, y1;
    npc.GetPosition(x1, y1);
    npc.Update(1.0f); // 1 second of movement
    float x2, y2;
    npc.GetPosition(x2, y2);
    // Should have moved closer to destination
    float dist1 = std::sqrt((200.0f - x1) * (200.0f - x1) + (200.0f - y1) * (200.0f - y1));
    float dist2 = std::sqrt((200.0f - x2) * (200.0f - x2) + (200.0f - y2) * (200.0f - y2));
    ASSERT_TRUE(dist2 < dist1);
}

TEST(test_schedule_picks_latest_entry) {
    NPC npc;
    npc.SetPosition(0.0f, 0.0f);
    npc.AddScheduleEntry(0, 100.0f, 100.0f);  // midnight
    npc.AddScheduleEntry(8, 200.0f, 200.0f);  // 8am
    npc.AddScheduleEntry(18, 300.0f, 300.0f); // 6pm

    // At hour 10, should use the 8am entry (latest entry <= 10)
    npc.SetCurrentHour(10);
    npc.Update(1.0f);
    float x, y;
    npc.GetPosition(x, y);
    // Should be moving toward (200, 200), not (300, 300) or (100, 100)
    float distTo200 = std::sqrt((200.0f - x) * (200.0f - x) + (200.0f - y) * (200.0f - y));
    float distTo300 = std::sqrt((300.0f - x) * (300.0f - x) + (300.0f - y) * (300.0f - y));
    ASSERT_TRUE(distTo200 < distTo300);
}

TEST(test_npc_stops_at_destination) {
    NPC npc;
    npc.SetPosition(100.0f, 100.0f);
    npc.AddScheduleEntry(8, 101.0f, 101.0f); // Very close destination
    npc.SetCurrentHour(8);
    // Simulate many updates
    for (int i = 0; i < 100; ++i) {
        npc.Update(0.1f);
    }
    float x, y;
    npc.GetPosition(x, y);
    // Should have arrived at approximately the destination
    float dist = std::sqrt((101.0f - x) * (101.0f - x) + (101.0f - y) * (101.0f - y));
    ASSERT_TRUE(dist < 5.0f); // Within arrival threshold
}

TEST(test_inactive_npc_doesnt_move) {
    NPC npc;
    npc.SetPosition(100.0f, 100.0f);
    npc.SetActive(false);
    npc.AddScheduleEntry(8, 200.0f, 200.0f);
    npc.SetCurrentHour(8);
    npc.Update(1.0f);
    float x, y;
    npc.GetPosition(x, y);
    ASSERT_EQ(static_cast<int>(x), 100);
    ASSERT_EQ(static_cast<int>(y), 100);
}

// ---- Proximity detection ----

TEST(test_player_nearby_true) {
    NPC npc;
    npc.SetPosition(100.0f, 100.0f);
    // Within INTERACT_RANGE (48 pixels)
    ASSERT_TRUE(npc.IsPlayerNearby(110.0f, 110.0f));
}

TEST(test_player_nearby_false) {
    NPC npc;
    npc.SetPosition(100.0f, 100.0f);
    // Beyond INTERACT_RANGE (48 pixels)
    ASSERT_FALSE(npc.IsPlayerNearby(200.0f, 200.0f));
}

TEST(test_player_nearby_exact_range) {
    NPC npc;
    npc.SetPosition(0.0f, 0.0f);
    // At exactly 48 pixels distance (within range)
    ASSERT_TRUE(npc.IsPlayerNearby(48.0f, 0.0f));
}

TEST(test_player_nearby_just_outside) {
    NPC npc;
    npc.SetPosition(0.0f, 0.0f);
    // Just barely outside 48 pixels
    ASSERT_FALSE(npc.IsPlayerNearby(49.0f, 0.0f));
}

// ---- Dialogue integration ----

TEST(test_npc_has_dialogue) {
    NPC npc;
    Dialogue& dlg = npc.GetDialogue();
    dlg.AddLine("Hello!");
    dlg.AddLine("Nice weather today.");
    ASSERT_EQ(dlg.GetCurrentLine(), "Hello!");
    ASSERT_TRUE(dlg.HasNext());
}

int main() {
    std::cout << "=== NPC Tests ===" << std::endl;
    RUN_TEST(test_default_name);
    RUN_TEST(test_set_name);
    RUN_TEST(test_npc_is_active_by_default);
    RUN_TEST(test_npc_default_size);
    RUN_TEST(test_initial_friendship);
    RUN_TEST(test_add_friendship);
    RUN_TEST(test_add_friendship_accumulates);
    RUN_TEST(test_friendship_cap_at_max);
    RUN_TEST(test_friendship_no_negative);
    RUN_TEST(test_friendship_add_then_cap);
    RUN_TEST(test_schedule_sets_destination);
    RUN_TEST(test_schedule_picks_latest_entry);
    RUN_TEST(test_npc_stops_at_destination);
    RUN_TEST(test_inactive_npc_doesnt_move);
    RUN_TEST(test_player_nearby_true);
    RUN_TEST(test_player_nearby_false);
    RUN_TEST(test_player_nearby_exact_range);
    RUN_TEST(test_player_nearby_just_outside);
    RUN_TEST(test_npc_has_dialogue);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
