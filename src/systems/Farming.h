#ifndef FARMING_H
#define FARMING_H

#include <string>

enum class CropType { PARSNIP, POTATO, TOMATO };
enum class CropStage { SEED, SPROUT, GROWING, HARVEST };

class Crop {
public:
    CropType type;
    CropStage stage;
    int daysGrowing;
    bool watered;
};

class FarmingSystem {
public:
    // Get days required for a crop type to reach harvest
    static int GetGrowthDays(CropType type);
    // Get the name of a crop type
    static std::string GetCropName(CropType type);
    // Get the sell value for a harvested crop
    static int GetCropValue(CropType type);
};

#endif // FARMING_H
