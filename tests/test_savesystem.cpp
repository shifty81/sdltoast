// Harvest Quest — SaveSystem unit tests
// Tests save/load round-trip with file I/O

#include "systems/SaveSystem.h"
#include "entities/Player.h"
#include "systems/Inventory.h"
#include "systems/Calendar.h"
#include <cassert>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sys/stat.h>

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

static const char* TEST_SAVE_PATH = "/tmp/harvestquest_test_save.dat";

// Helper to remove a test file
static void CleanupTestFile() {
    std::remove(TEST_SAVE_PATH);
}

// ---- Basic save/load round-trip ----

TEST(test_save_creates_file) {
    CleanupTestFile();
    Player player;
    Inventory inventory;
    Calendar calendar;
    int gold = 100;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &player, &inventory, &calendar, gold));

    // Verify file exists
    std::ifstream file(TEST_SAVE_PATH);
    ASSERT_TRUE(file.is_open());
    file.close();
    CleanupTestFile();
}

TEST(test_save_null_player_fails) {
    Inventory inventory;
    Calendar calendar;
    int gold = 0;
    ASSERT_FALSE(SaveSystem::Save(TEST_SAVE_PATH, nullptr, &inventory, &calendar, gold));
}

TEST(test_save_null_inventory_fails) {
    Player player;
    Calendar calendar;
    int gold = 0;
    ASSERT_FALSE(SaveSystem::Save(TEST_SAVE_PATH, &player, nullptr, &calendar, gold));
}

TEST(test_save_null_calendar_fails) {
    Player player;
    Inventory inventory;
    int gold = 0;
    ASSERT_FALSE(SaveSystem::Save(TEST_SAVE_PATH, &player, &inventory, nullptr, gold));
}

TEST(test_load_nonexistent_file_fails) {
    Player player;
    Inventory inventory;
    Calendar calendar;
    int gold = 0;
    ASSERT_FALSE(SaveSystem::Load("/tmp/nonexistent_save_file.dat",
                                   &player, &inventory, &calendar, gold));
}

TEST(test_load_null_player_fails) {
    Inventory inventory;
    Calendar calendar;
    int gold = 0;
    ASSERT_FALSE(SaveSystem::Load(TEST_SAVE_PATH, nullptr, &inventory, &calendar, gold));
}

// ---- Round-trip: player position ----

TEST(test_roundtrip_player_position) {
    CleanupTestFile();
    Player savePlayer;
    savePlayer.SetPosition(150.5f, 200.3f);
    Inventory saveInv;
    Calendar saveCal;
    int saveGold = 0;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    float lx, ly;
    loadPlayer.GetPosition(lx, ly);
    // Check position matches (allowing small float precision margin)
    ASSERT_TRUE(std::abs(lx - 150.5f) < 0.1f);
    ASSERT_TRUE(std::abs(ly - 200.3f) < 0.1f);
    CleanupTestFile();
}

// ---- Round-trip: player health ----

TEST(test_roundtrip_player_health) {
    CleanupTestFile();
    Player savePlayer;
    savePlayer.SetMaxHealth(10);
    savePlayer.SetHealth(7);
    Inventory saveInv;
    Calendar saveCal;
    int saveGold = 0;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    ASSERT_EQ(loadPlayer.GetHealth(), 7);
    ASSERT_EQ(loadPlayer.GetMaxHealth(), 10);
    CleanupTestFile();
}

// ---- Round-trip: calendar ----

TEST(test_roundtrip_calendar) {
    CleanupTestFile();
    Player savePlayer;
    Inventory saveInv;
    Calendar saveCal;
    saveCal.SetDay(15);
    saveCal.SetSeason(Season::FALL);
    saveCal.SetYear(3);
    int saveGold = 0;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    ASSERT_EQ(loadCal.GetDay(), 15);
    ASSERT_EQ(loadCal.GetSeason(), Season::FALL);
    ASSERT_EQ(loadCal.GetYear(), 3);
    CleanupTestFile();
}

// ---- Round-trip: gold ----

TEST(test_roundtrip_gold) {
    CleanupTestFile();
    Player savePlayer;
    Inventory saveInv;
    Calendar saveCal;
    int saveGold = 9999;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    ASSERT_EQ(loadGold, 9999);
    CleanupTestFile();
}

// ---- Round-trip: inventory ----

TEST(test_roundtrip_inventory_single_item) {
    CleanupTestFile();
    Player savePlayer;
    Inventory saveInv;
    saveInv.AddItem("Wood", 25);
    Calendar saveCal;
    int saveGold = 0;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    ASSERT_EQ(loadInv.GetItemCount("Wood"), 25);
    CleanupTestFile();
}

TEST(test_roundtrip_inventory_multiple_items) {
    CleanupTestFile();
    Player savePlayer;
    Inventory saveInv;
    saveInv.AddItem("Wood", 10);
    saveInv.AddItem("Stone", 5);
    saveInv.AddItem("Parsnip", 3);
    Calendar saveCal;
    int saveGold = 500;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    ASSERT_EQ(loadInv.GetItemCount("Wood"), 10);
    ASSERT_EQ(loadInv.GetItemCount("Stone"), 5);
    ASSERT_EQ(loadInv.GetItemCount("Parsnip"), 3);
    ASSERT_EQ(loadGold, 500);
    CleanupTestFile();
}

TEST(test_roundtrip_item_with_spaces) {
    CleanupTestFile();
    Player savePlayer;
    Inventory saveInv;
    saveInv.AddItem("Parsnip Soup", 2);
    saveInv.AddItem("Baked Potato", 1);
    Calendar saveCal;
    int saveGold = 0;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    ASSERT_EQ(loadInv.GetItemCount("Parsnip Soup"), 2);
    ASSERT_EQ(loadInv.GetItemCount("Baked Potato"), 1);
    CleanupTestFile();
}

// ---- Round-trip: full state ----

TEST(test_roundtrip_full_state) {
    CleanupTestFile();
    Player savePlayer;
    savePlayer.SetPosition(100.0f, 200.0f);
    savePlayer.SetMaxHealth(8);
    savePlayer.SetHealth(5);

    Inventory saveInv;
    saveInv.AddItem("Wood", 50);
    saveInv.AddItem("Stone Wall", 4);
    saveInv.AddItem("Tomato", 12);

    Calendar saveCal;
    saveCal.SetDay(21);
    saveCal.SetSeason(Season::WINTER);
    saveCal.SetYear(2);

    int saveGold = 3750;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    // Verify all state
    float lx, ly;
    loadPlayer.GetPosition(lx, ly);
    ASSERT_TRUE(std::abs(lx - 100.0f) < 0.1f);
    ASSERT_TRUE(std::abs(ly - 200.0f) < 0.1f);
    ASSERT_EQ(loadPlayer.GetHealth(), 5);
    ASSERT_EQ(loadPlayer.GetMaxHealth(), 8);

    ASSERT_EQ(loadInv.GetItemCount("Wood"), 50);
    ASSERT_EQ(loadInv.GetItemCount("Stone Wall"), 4);
    ASSERT_EQ(loadInv.GetItemCount("Tomato"), 12);

    ASSERT_EQ(loadCal.GetDay(), 21);
    ASSERT_EQ(loadCal.GetSeason(), Season::WINTER);
    ASSERT_EQ(loadCal.GetYear(), 2);

    ASSERT_EQ(loadGold, 3750);
    CleanupTestFile();
}

TEST(test_load_clears_previous_inventory) {
    CleanupTestFile();
    Player savePlayer;
    Inventory saveInv;
    saveInv.AddItem("Wood", 5);
    Calendar saveCal;
    int saveGold = 0;

    ASSERT_TRUE(SaveSystem::Save(TEST_SAVE_PATH, &savePlayer, &saveInv, &saveCal, saveGold));

    Player loadPlayer;
    Inventory loadInv;
    loadInv.AddItem("Garbage", 999); // Pre-existing item should be cleared
    Calendar loadCal;
    int loadGold = 0;

    ASSERT_TRUE(SaveSystem::Load(TEST_SAVE_PATH, &loadPlayer, &loadInv, &loadCal, loadGold));

    ASSERT_EQ(loadInv.GetItemCount("Garbage"), 0); // Should be cleared
    ASSERT_EQ(loadInv.GetItemCount("Wood"), 5);
    CleanupTestFile();
}

int main() {
    std::cout << "=== SaveSystem Tests ===" << std::endl;
    RUN_TEST(test_save_creates_file);
    RUN_TEST(test_save_null_player_fails);
    RUN_TEST(test_save_null_inventory_fails);
    RUN_TEST(test_save_null_calendar_fails);
    RUN_TEST(test_load_nonexistent_file_fails);
    RUN_TEST(test_load_null_player_fails);
    RUN_TEST(test_roundtrip_player_position);
    RUN_TEST(test_roundtrip_player_health);
    RUN_TEST(test_roundtrip_calendar);
    RUN_TEST(test_roundtrip_gold);
    RUN_TEST(test_roundtrip_inventory_single_item);
    RUN_TEST(test_roundtrip_inventory_multiple_items);
    RUN_TEST(test_roundtrip_item_with_spaces);
    RUN_TEST(test_roundtrip_full_state);
    RUN_TEST(test_load_clears_previous_inventory);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
