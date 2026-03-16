#ifndef FISHING_H
#define FISHING_H

#include <string>
#include <vector>

// Forward declaration
enum class Season;

/**
 * Represents a fish that can be caught.
 */
struct FishType {
    std::string name;
    int value;          // Sell price
    int difficulty;     // 1-10 difficulty to catch
    bool springAvail;
    bool summerAvail;
    bool fallAvail;
    bool winterAvail;
};

/**
 * Fishing system — determines what can be caught and catch success.
 */
class FishingSystem {
public:
    FishingSystem();

    // Get the list of fish available in a given season
    std::vector<const FishType*> GetAvailableFish(Season season) const;

    // Attempt a catch. Returns the fish index on success, -1 on miss.
    // skillLevel (0-10) and difficulty affect success chance.
    int AttemptCatch(Season season, int skillLevel) const;

    // Access fish data
    const FishType* GetFish(int index) const;
    int GetFishCount() const { return static_cast<int>(m_fish.size()); }

    // Deterministic catch for testing — pass a roll in [0, 100)
    int AttemptCatchWithRoll(Season season, int skillLevel, int roll) const;

private:
    std::vector<FishType> m_fish;
    void InitFish();
};

#endif // FISHING_H
