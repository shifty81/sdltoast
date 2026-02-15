// Harvest Quest — Farming system unit tests

#include "systems/Farming.h"
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

TEST(test_growth_days_parsnip) {
    ASSERT_EQ(FarmingSystem::GetGrowthDays(CropType::PARSNIP), 4);
}

TEST(test_growth_days_potato) {
    ASSERT_EQ(FarmingSystem::GetGrowthDays(CropType::POTATO), 6);
}

TEST(test_growth_days_tomato) {
    ASSERT_EQ(FarmingSystem::GetGrowthDays(CropType::TOMATO), 8);
}

TEST(test_crop_name_parsnip) {
    ASSERT_EQ(FarmingSystem::GetCropName(CropType::PARSNIP), "Parsnip");
}

TEST(test_crop_name_potato) {
    ASSERT_EQ(FarmingSystem::GetCropName(CropType::POTATO), "Potato");
}

TEST(test_crop_name_tomato) {
    ASSERT_EQ(FarmingSystem::GetCropName(CropType::TOMATO), "Tomato");
}

TEST(test_crop_value_parsnip) {
    ASSERT_EQ(FarmingSystem::GetCropValue(CropType::PARSNIP), 35);
}

TEST(test_crop_value_potato) {
    ASSERT_EQ(FarmingSystem::GetCropValue(CropType::POTATO), 80);
}

TEST(test_crop_value_tomato) {
    ASSERT_EQ(FarmingSystem::GetCropValue(CropType::TOMATO), 60);
}

TEST(test_crop_initial_state) {
    Crop crop;
    crop.type = CropType::PARSNIP;
    crop.stage = CropStage::SEED;
    crop.daysGrowing = 0;
    crop.watered = false;
    ASSERT_EQ(crop.stage, CropStage::SEED);
    ASSERT_EQ(crop.daysGrowing, 0);
    ASSERT_FALSE(crop.watered);
}

TEST(test_potato_more_valuable_than_parsnip) {
    ASSERT_TRUE(FarmingSystem::GetCropValue(CropType::POTATO) >
                FarmingSystem::GetCropValue(CropType::PARSNIP));
}

TEST(test_tomato_grows_longer_than_parsnip) {
    ASSERT_TRUE(FarmingSystem::GetGrowthDays(CropType::TOMATO) >
                FarmingSystem::GetGrowthDays(CropType::PARSNIP));
}

int main() {
    std::cout << "=== Farming Tests ===" << std::endl;
    RUN_TEST(test_growth_days_parsnip);
    RUN_TEST(test_growth_days_potato);
    RUN_TEST(test_growth_days_tomato);
    RUN_TEST(test_crop_name_parsnip);
    RUN_TEST(test_crop_name_potato);
    RUN_TEST(test_crop_name_tomato);
    RUN_TEST(test_crop_value_parsnip);
    RUN_TEST(test_crop_value_potato);
    RUN_TEST(test_crop_value_tomato);
    RUN_TEST(test_crop_initial_state);
    RUN_TEST(test_potato_more_valuable_than_parsnip);
    RUN_TEST(test_tomato_grows_longer_than_parsnip);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
