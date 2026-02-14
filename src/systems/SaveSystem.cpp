#include "SaveSystem.h"
#include "../entities/Player.h"
#include "Inventory.h"
#include "Calendar.h"
#include "../engine/Logger.h"
#include <fstream>
#include <sstream>
#include <cerrno>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

// Simple text-based save format:
//   HARVEST_QUEST_SAVE
//   PLAYER <x> <y> <health> <maxHealth>
//   CALENDAR <day> <season> <year>
//   GOLD <gold>
//   ITEM <name> <quantity>
//   ...
//   END

static bool EnsureDirectory(const std::string& filepath) {
    // Extract directory part
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) return true; // no directory
    std::string dir = filepath.substr(0, pos);
    int result = 0;
#ifdef _WIN32
    result = _mkdir(dir.c_str());
#else
    result = mkdir(dir.c_str(), 0755);
#endif
    // mkdir returns 0 on success, or -1 if directory already exists (EEXIST)
    return (result == 0 || errno == EEXIST);
}

bool SaveSystem::Save(const std::string& filepath,
                      const Player* player,
                      const Inventory* inventory,
                      const Calendar* calendar,
                      int gold) {
    if (!player || !inventory || !calendar) return false;

    if (!EnsureDirectory(filepath)) {
        Logger::Instance().Error("SaveSystem: Cannot create save directory for: " + filepath);
        return false;
    }

    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Instance().Error("SaveSystem: Cannot open file for writing: " + filepath);
        return false;
    }

    file << "HARVEST_QUEST_SAVE\n";

    // Player
    float px, py;
    player->GetPosition(px, py);
    file << "PLAYER " << px << " " << py << " "
         << player->GetHealth() << " " << player->GetMaxHealth() << "\n";

    // Calendar
    file << "CALENDAR " << calendar->GetDay() << " "
         << static_cast<int>(calendar->GetSeason()) << " "
         << calendar->GetYear() << "\n";

    // Gold
    file << "GOLD " << gold << "\n";

    // Inventory items
    for (const auto& item : inventory->GetItems()) {
        file << "ITEM " << item.quantity << " " << item.name << "\n";
    }

    file << "END\n";
    file.close();

    Logger::Instance().Info("SaveSystem: Game saved to " + filepath);
    return true;
}

bool SaveSystem::Load(const std::string& filepath,
                      Player* player,
                      Inventory* inventory,
                      Calendar* calendar,
                      int& gold) {
    if (!player || !inventory || !calendar) return false;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Instance().Error("SaveSystem: Cannot open file for reading: " + filepath);
        return false;
    }

    std::string line;

    // Verify header
    if (!std::getline(file, line) || line != "HARVEST_QUEST_SAVE") {
        Logger::Instance().Error("SaveSystem: Invalid save file format");
        return false;
    }

    // Clear inventory before loading
    inventory->Clear();

    while (std::getline(file, line)) {
        if (line == "END") break;

        std::istringstream iss(line);
        std::string tag;
        iss >> tag;

        if (tag == "PLAYER") {
            float px, py;
            int health, maxHealth;
            iss >> px >> py >> health >> maxHealth;
            player->SetPosition(px, py);
            player->SetMaxHealth(maxHealth);
            player->SetHealth(health);
        } else if (tag == "CALENDAR") {
            int day, seasonInt, year;
            iss >> day >> seasonInt >> year;
            calendar->SetDay(day);
            calendar->SetSeason(static_cast<Season>(seasonInt));
            calendar->SetYear(year);
        } else if (tag == "GOLD") {
            iss >> gold;
        } else if (tag == "ITEM") {
            int quantity;
            iss >> quantity;
            // Item name is the rest of the line (may contain spaces)
            std::string name;
            std::getline(iss, name);
            // Trim leading space
            if (!name.empty() && name[0] == ' ') name = name.substr(1);
            if (!name.empty()) {
                inventory->AddItem(name, quantity);
            }
        }
    }

    file.close();
    Logger::Instance().Info("SaveSystem: Game loaded from " + filepath);
    return true;
}
