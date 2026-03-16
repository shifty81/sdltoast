#ifndef ANIMAL_HUSBANDRY_H
#define ANIMAL_HUSBANDRY_H

#include <string>
#include <vector>

/**
 * Types of animals that can be raised on the farm.
 */
enum class AnimalType { CHICKEN, COW, SHEEP, GOAT, PIG, COUNT };

/**
 * Represents a farm animal with happiness, age, and production state.
 */
struct Animal {
    std::string name;
    AnimalType type;
    int happiness;    // 0-10 happiness level
    int daysOwned;    // Age in days since purchase
    bool fedToday;    // Whether the animal was fed today
    bool pettedToday; // Whether the animal was petted today
};

/**
 * Describes a product that an animal can produce.
 */
struct AnimalProduct {
    std::string name;
    int baseValue;    // Base sell price
};

/**
 * Animal husbandry system — raising animals for produce.
 * Animals require daily feeding and care.
 * Happiness affects product quality and value.
 */
class AnimalHusbandrySystem {
public:
    AnimalHusbandrySystem();

    // Animal management
    int AddAnimal(const std::string& name, AnimalType type);
    bool RemoveAnimal(int index);
    Animal* GetAnimal(int index);
    const Animal* GetAnimal(int index) const;
    int GetAnimalCount() const { return static_cast<int>(m_animals.size()); }

    // Daily care
    bool FeedAnimal(int index);
    bool PetAnimal(int index);

    // Advance day — updates happiness based on care, resets daily flags
    void AdvanceDay();

    // Production
    bool CanProduce(int index) const;
    AnimalProduct GetProduct(AnimalType type) const;
    int GetProductValue(int index) const;

    // Costs
    static int GetPurchaseCost(AnimalType type);
    static std::string GetAnimalTypeName(AnimalType type);
    static int GetFeedEnergyCost();

    // Constants
    static constexpr int MAX_HAPPINESS = 10;
    static constexpr int MIN_PRODUCE_DAYS = 3;
    static constexpr int FEED_ENERGY_COST = 2;

private:
    std::vector<Animal> m_animals;
};

#endif // ANIMAL_HUSBANDRY_H
