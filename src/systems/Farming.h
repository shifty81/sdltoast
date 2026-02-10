#ifndef FARMING_H
#define FARMING_H

enum class CropType { PARSNIP, POTATO, TOMATO };
enum class CropStage { SEED, SPROUT, GROWING, HARVEST };

class Crop {
public:
    CropType type;
    CropStage stage;
    int daysGrowing;
    bool watered;
};

#endif // FARMING_H
