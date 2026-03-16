// Harvest Quest — Animal Husbandry system unit tests

#include "systems/AnimalHusbandry.h"
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

TEST(test_add_animal) {
    AnimalHusbandrySystem sys;
    int idx = sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    ASSERT_EQ(idx, 0);
    ASSERT_EQ(sys.GetAnimalCount(), 1);
}

TEST(test_add_multiple_animals) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    sys.AddAnimal("Bessie", AnimalType::COW);
    sys.AddAnimal("Woolly", AnimalType::SHEEP);
    ASSERT_EQ(sys.GetAnimalCount(), 3);
}

TEST(test_add_animal_empty_name) {
    AnimalHusbandrySystem sys;
    int idx = sys.AddAnimal("", AnimalType::CHICKEN);
    ASSERT_EQ(idx, -1);
    ASSERT_EQ(sys.GetAnimalCount(), 0);
}

TEST(test_add_animal_invalid_type) {
    AnimalHusbandrySystem sys;
    int idx = sys.AddAnimal("Bad", AnimalType::COUNT);
    ASSERT_EQ(idx, -1);
}

TEST(test_get_animal) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Bessie", AnimalType::COW);
    const Animal* a = sys.GetAnimal(0);
    ASSERT_TRUE(a != nullptr);
    ASSERT_EQ(a->name, "Bessie");
    ASSERT_EQ(a->type, AnimalType::COW);
    ASSERT_EQ(a->happiness, 5);
    ASSERT_EQ(a->daysOwned, 0);
}

TEST(test_get_animal_invalid) {
    AnimalHusbandrySystem sys;
    ASSERT_TRUE(sys.GetAnimal(-1) == nullptr);
    ASSERT_TRUE(sys.GetAnimal(0) == nullptr);
}

TEST(test_remove_animal) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    ASSERT_TRUE(sys.RemoveAnimal(0));
    ASSERT_EQ(sys.GetAnimalCount(), 0);
}

TEST(test_remove_animal_invalid) {
    AnimalHusbandrySystem sys;
    ASSERT_FALSE(sys.RemoveAnimal(0));
    ASSERT_FALSE(sys.RemoveAnimal(-1));
}

TEST(test_feed_animal) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    ASSERT_TRUE(sys.FeedAnimal(0));
    ASSERT_TRUE(sys.GetAnimal(0)->fedToday);
}

TEST(test_feed_animal_twice) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    ASSERT_TRUE(sys.FeedAnimal(0));
    ASSERT_FALSE(sys.FeedAnimal(0));  // Already fed
}

TEST(test_pet_animal) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Bessie", AnimalType::COW);
    ASSERT_TRUE(sys.PetAnimal(0));
    ASSERT_TRUE(sys.GetAnimal(0)->pettedToday);
}

TEST(test_pet_animal_twice) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Bessie", AnimalType::COW);
    ASSERT_TRUE(sys.PetAnimal(0));
    ASSERT_FALSE(sys.PetAnimal(0));  // Already petted
}

TEST(test_advance_day_happy) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    sys.FeedAnimal(0);
    sys.PetAnimal(0);
    sys.AdvanceDay();
    const Animal* a = sys.GetAnimal(0);
    ASSERT_EQ(a->happiness, 6);   // Fed + petted → +1
    ASSERT_EQ(a->daysOwned, 1);
    ASSERT_FALSE(a->fedToday);    // Reset
    ASSERT_FALSE(a->pettedToday); // Reset
}

TEST(test_advance_day_unfed) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    // Don't feed or pet
    sys.AdvanceDay();
    ASSERT_EQ(sys.GetAnimal(0)->happiness, 3);  // 5 - 2
}

TEST(test_advance_day_fed_not_petted) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    sys.FeedAnimal(0);
    // Don't pet
    sys.AdvanceDay();
    ASSERT_EQ(sys.GetAnimal(0)->happiness, 5);  // No change
}

TEST(test_happiness_capped_at_max) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    // Start at 5, feed+pet for 6 days → 5+6 = 11, capped at 10
    for (int i = 0; i < 6; ++i) {
        sys.FeedAnimal(0);
        sys.PetAnimal(0);
        sys.AdvanceDay();
    }
    ASSERT_EQ(sys.GetAnimal(0)->happiness, AnimalHusbandrySystem::MAX_HAPPINESS);
}

TEST(test_happiness_floor_at_zero) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Clucky", AnimalType::CHICKEN);
    // Start at 5, skip feeding for 4 days → 5-8 = -3, floored at 0
    for (int i = 0; i < 4; ++i) {
        sys.AdvanceDay();
    }
    ASSERT_EQ(sys.GetAnimal(0)->happiness, 0);
}

TEST(test_can_produce_happy_and_old) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Bessie", AnimalType::COW);
    // Advance 3 days with care
    for (int i = 0; i < AnimalHusbandrySystem::MIN_PRODUCE_DAYS; ++i) {
        sys.FeedAnimal(0);
        sys.PetAnimal(0);
        sys.AdvanceDay();
    }
    ASSERT_TRUE(sys.CanProduce(0));
}

TEST(test_cannot_produce_too_young) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Bessie", AnimalType::COW);
    ASSERT_FALSE(sys.CanProduce(0));  // 0 days old
}

TEST(test_cannot_produce_unhappy) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Bessie", AnimalType::COW);
    // Advance 3 days without care → happiness drops below 3
    for (int i = 0; i < AnimalHusbandrySystem::MIN_PRODUCE_DAYS; ++i) {
        sys.AdvanceDay();
    }
    ASSERT_FALSE(sys.CanProduce(0));
}

TEST(test_product_types) {
    AnimalHusbandrySystem sys;
    ASSERT_EQ(sys.GetProduct(AnimalType::CHICKEN).name, "Egg");
    ASSERT_EQ(sys.GetProduct(AnimalType::COW).name, "Milk");
    ASSERT_EQ(sys.GetProduct(AnimalType::SHEEP).name, "Wool");
    ASSERT_EQ(sys.GetProduct(AnimalType::GOAT).name, "Goat Milk");
    ASSERT_EQ(sys.GetProduct(AnimalType::PIG).name, "Truffle");
}

TEST(test_product_value_scales_with_happiness) {
    AnimalHusbandrySystem sys;
    sys.AddAnimal("Low", AnimalType::COW);
    sys.AddAnimal("High", AnimalType::COW);

    // Make both old enough (3+ days)
    for (int i = 0; i < 3; ++i) {
        sys.FeedAnimal(0);
        sys.PetAnimal(0);
        sys.FeedAnimal(1);
        sys.PetAnimal(1);
        sys.AdvanceDay();
    }

    // Now make "High" happier by feeding+petting more days
    for (int i = 0; i < 4; ++i) {
        sys.FeedAnimal(0);  // Keep Low at 8 happiness
        sys.FeedAnimal(1);
        sys.PetAnimal(1);
        sys.AdvanceDay();
    }

    int valueLow = sys.GetProductValue(0);
    int valueHigh = sys.GetProductValue(1);
    ASSERT_TRUE(valueHigh >= valueLow);
}

TEST(test_purchase_costs) {
    ASSERT_EQ(AnimalHusbandrySystem::GetPurchaseCost(AnimalType::CHICKEN), 200);
    ASSERT_EQ(AnimalHusbandrySystem::GetPurchaseCost(AnimalType::COW), 500);
    ASSERT_EQ(AnimalHusbandrySystem::GetPurchaseCost(AnimalType::SHEEP), 400);
    ASSERT_EQ(AnimalHusbandrySystem::GetPurchaseCost(AnimalType::GOAT), 450);
    ASSERT_EQ(AnimalHusbandrySystem::GetPurchaseCost(AnimalType::PIG), 600);
}

TEST(test_animal_type_names) {
    ASSERT_EQ(AnimalHusbandrySystem::GetAnimalTypeName(AnimalType::CHICKEN), "Chicken");
    ASSERT_EQ(AnimalHusbandrySystem::GetAnimalTypeName(AnimalType::COW), "Cow");
    ASSERT_EQ(AnimalHusbandrySystem::GetAnimalTypeName(AnimalType::SHEEP), "Sheep");
    ASSERT_EQ(AnimalHusbandrySystem::GetAnimalTypeName(AnimalType::GOAT), "Goat");
    ASSERT_EQ(AnimalHusbandrySystem::GetAnimalTypeName(AnimalType::PIG), "Pig");
}

TEST(test_feed_energy_cost) {
    ASSERT_EQ(AnimalHusbandrySystem::GetFeedEnergyCost(), 2);
}

int main() {
    std::cout << "=== Animal Husbandry Tests ===" << std::endl;
    RUN_TEST(test_add_animal);
    RUN_TEST(test_add_multiple_animals);
    RUN_TEST(test_add_animal_empty_name);
    RUN_TEST(test_add_animal_invalid_type);
    RUN_TEST(test_get_animal);
    RUN_TEST(test_get_animal_invalid);
    RUN_TEST(test_remove_animal);
    RUN_TEST(test_remove_animal_invalid);
    RUN_TEST(test_feed_animal);
    RUN_TEST(test_feed_animal_twice);
    RUN_TEST(test_pet_animal);
    RUN_TEST(test_pet_animal_twice);
    RUN_TEST(test_advance_day_happy);
    RUN_TEST(test_advance_day_unfed);
    RUN_TEST(test_advance_day_fed_not_petted);
    RUN_TEST(test_happiness_capped_at_max);
    RUN_TEST(test_happiness_floor_at_zero);
    RUN_TEST(test_can_produce_happy_and_old);
    RUN_TEST(test_cannot_produce_too_young);
    RUN_TEST(test_cannot_produce_unhappy);
    RUN_TEST(test_product_types);
    RUN_TEST(test_product_value_scales_with_happiness);
    RUN_TEST(test_purchase_costs);
    RUN_TEST(test_animal_type_names);
    RUN_TEST(test_feed_energy_cost);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
