#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <string>

class Player;
class Inventory;
class Calendar;

class SaveSystem {
public:
    // Save game state to a file. Returns true on success.
    static bool Save(const std::string& filepath,
                     const Player* player,
                     const Inventory* inventory,
                     const Calendar* calendar,
                     int gold);

    // Load game state from a file. Returns true on success.
    static bool Load(const std::string& filepath,
                     Player* player,
                     Inventory* inventory,
                     Calendar* calendar,
                     int& gold);

    static constexpr const char* DEFAULT_SAVE_PATH = "saves/save.dat";
};

#endif // SAVESYSTEM_H
