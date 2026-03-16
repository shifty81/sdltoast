#include "Game.h"
#include "Renderer.h"
#include "Input.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "SpriteSheet.h"
#include "Logger.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/NPC.h"
#include "../world/Map.h"
#include "../world/WorldGenerator.h"
#include "../world/Tile.h"
#include "../systems/Combat.h"
#include "../systems/Farming.h"
#include "../systems/Inventory.h"
#include "../systems/Calendar.h"
#include "../systems/Crafting.h"
#include "../systems/SaveSystem.h"
#include "../systems/Energy.h"
#include "../systems/Skills.h"
#include "../systems/Quest.h"
#include "../systems/Fishing.h"
#include "../ui/HUD.h"
#include <raylib.h>
#include <iostream>

Game::Game()
    : m_running(false)
    , m_windowWidth(0)
    , m_windowHeight(0)
    , m_gold(0)
    , m_showInventory(false)
    , m_showCrafting(false)
    , m_craftingIndex(0)
    , m_damageCooldown(0.0f)
    , m_dialogueChoiceIndex(0)
{
}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize(const std::string& title, int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;

    // Initialize Raylib window
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(width, height, title.c_str());
    if (!IsWindowReady()) {
        Logger::Instance().Error("Window creation failed");
        return false;
    }

    SetTargetFPS(TARGET_FPS);

    // Initialize subsystems
    m_renderer = std::make_unique<Renderer>();
    if (!m_renderer->Initialize(width, height)) {
        Logger::Instance().Error("Renderer initialization failed");
        return false;
    }

    m_input = std::make_unique<Input>();
    m_assetManager = std::make_unique<AssetManager>();
    m_audioManager = std::make_unique<AudioManager>();

    if (!m_assetManager->Initialize(m_renderer.get())) {
        Logger::Instance().Error("Asset manager initialization failed");
        return false;
    }

    if (!m_audioManager->Initialize()) {
        Logger::Instance().Error("Audio manager initialization failed");
        return false;
    }

    // Initialize tile registry (CRITICAL for smart tiles)
    TileRegistry::Initialize();

    // Load sprite sheets (will gracefully fallback if files don't exist)
    SpriteSheetManager::Instance().LoadDefaultAssets(m_renderer.get());

    // Initialize game objects
    m_player = std::make_unique<Player>();
    m_player->SetPosition(400, 300); // Start in center
    
    // Initialize game systems
    m_hud = std::make_unique<HUD>();
    m_calendar = std::make_unique<Calendar>();
    m_inventory = std::make_unique<Inventory>();
    m_crafting = std::make_unique<Crafting>();
    m_energy = std::make_unique<Energy>();
    m_skills = std::make_unique<Skills>();
    m_questSystem = std::make_unique<QuestSystem>();
    m_fishingSystem = std::make_unique<FishingSystem>();
    
    // Generate world using new system
    m_currentMap = std::make_unique<Map>(25, 19);
    
    Logger::Instance().Info("");
    Logger::Instance().Info("=== HARVEST QUEST ===");
    Logger::Instance().Info("Press 1 = Farm | 2 = Dungeon | 3 = Overworld");
    Logger::Instance().Info("============================");
    Logger::Instance().Info("");
    
    // Default: Generate farm
    WorldGenerator generator(12345);
    generator.GenerateFarm(m_currentMap.get(), 25, 19);
    Logger::Instance().Info("Generated: Farm (default)");

    // Spawn initial NPCs on the farm
    SpawnNPCs();

    m_running = true;

    Logger::Instance().Info("Game initialized successfully!");
    Logger::Instance().Info("");
    Logger::Instance().Info("Controls:");
    Logger::Instance().Info("  WASD/Arrows - Move");
    Logger::Instance().Info("  Space - Attack");
    Logger::Instance().Info("  T - Till soil");
    Logger::Instance().Info("  R - Water tile");
    Logger::Instance().Info("  F - Plant crop");
    Logger::Instance().Info("  H - Harvest crop");
    Logger::Instance().Info("  C - Chop tree");
    Logger::Instance().Info("  G - Cast fishing line");
    Logger::Instance().Info("  N - Advance day (sleep)");
    Logger::Instance().Info("  I - Toggle inventory");
    Logger::Instance().Info("  P - Toggle crafting menu");
    Logger::Instance().Info("  E - Talk to NPC");
    Logger::Instance().Info("  F5 - Save game");
    Logger::Instance().Info("  F9 - Load game");
    Logger::Instance().Info("  1 - Generate Farm");
    Logger::Instance().Info("  2 - Generate Dungeon");
    Logger::Instance().Info("  3 - Generate Overworld");
    Logger::Instance().Info("  ESC - Quit");
    Logger::Instance().Info("");
    
    return true;
}

void Game::Run() {
    while (m_running && !WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        HandleEvents();
        Update(deltaTime);
        Render();
    }
}

void Game::HandleEvents() {
    m_input->Update();
}

void Game::Update(float deltaTime) {
    if (m_input->IsKeyPressed(KEY_ESCAPE)) {
        m_running = false;
    }

    // World generation hotkeys
    if (m_input->IsKeyPressed(KEY_ONE)) {
        WorldGenerator generator;
        generator.GenerateFarm(m_currentMap.get(), 25, 19);
        m_enemies.clear();
        SpawnNPCs();
        Logger::Instance().Info("Generated: Farm");
    }
    if (m_input->IsKeyPressed(KEY_TWO)) {
        WorldGenerator generator;
        generator.GenerateDungeon(m_currentMap.get(), 25, 19);
        m_npcs.clear();
        SpawnEnemies();
        Logger::Instance().Info("Generated: Dungeon (with enemies)");
    }
    if (m_input->IsKeyPressed(KEY_THREE)) {
        WorldGenerator generator;
        generator.GenerateOverworld(m_currentMap.get(), 25, 19, Biome::PLAINS);
        m_enemies.clear();
        m_npcs.clear();
        Logger::Instance().Info("Generated: Overworld");
    }

    // Toggle inventory
    if (m_input->IsKeyPressed(KEY_I)) {
        m_showInventory = !m_showInventory;
        if (m_showInventory) m_showCrafting = false;
    }

    // Toggle crafting
    if (m_input->IsKeyPressed(KEY_P)) {
        m_showCrafting = !m_showCrafting;
        if (m_showCrafting) m_showInventory = false;
        m_craftingIndex = 0;
    }

    // Advance day
    if (m_input->IsKeyPressed(KEY_N)) {
        AdvanceDay();
    }

    // Farming actions
    HandleFarmingActions();

    // Tree chopping
    HandleTreeChopping();

    // Combat actions
    HandleCombatActions();

    // Crafting actions
    HandleCrafting();

    // NPC interaction
    HandleNPCInteraction();

    // Fishing
    HandleFishing();

    // Save/Load
    HandleSaveLoad();

    // Update player
    if (m_player) {
        // Save previous position for collision resolution
        float prevX, prevY;
        m_player->GetPosition(prevX, prevY);
        
        m_player->Update(deltaTime, m_input.get());
        
        // Collision detection with tile system
        float px, py;
        m_player->GetPosition(px, py);
        
        // Get actual player dimensions
        float pw, ph;
        m_player->GetSize(pw, ph);
        
        // Check horizontal movement (keep previous Y)
        if (m_currentMap->IsAreaSolid(px, prevY, pw, ph)) {
            px = prevX;
        }
        
        // Check vertical movement (use resolved X)
        if (m_currentMap->IsAreaSolid(px, py, pw, ph)) {
            py = prevY;
        }
        
        m_player->SetPosition(px, py);
    }

    // Update enemies
    for (auto& enemy : m_enemies) {
        if (enemy && enemy->IsActive()) {
            // Update target to player position
            float px, py;
            m_player->GetPosition(px, py);
            enemy->SetTarget(px, py);
            enemy->Update(deltaTime);
        }
    }

    // Update NPCs
    for (auto& npc : m_npcs) {
        if (npc && npc->IsActive()) {
            npc->Update(deltaTime);
        }
    }

    // Enemy-player contact damage
    if (m_damageCooldown > 0.0f) {
        m_damageCooldown -= deltaTime;
    } else {
        for (auto& enemy : m_enemies) {
            if (Combat::EnemyContact(enemy.get(), m_player.get(), 1)) {
                m_damageCooldown = Combat::DAMAGE_COOLDOWN;
                Logger::Instance().Info("Player hit! Health: " + std::to_string(m_player->GetHealth()));
                break;
            }
        }
    }

    // Update map
    if (m_currentMap) {
        m_currentMap->Update(deltaTime);
    }

    // Update HUD
    UpdateHUD();
}

void Game::HandleFarmingActions() {
    if (!m_player || !m_currentMap) return;

    float px, py;
    m_player->GetPosition(px, py);
    float pw, ph;
    m_player->GetSize(pw, ph);

    // Get tile in front of player (center of player)
    int tileX, tileY;
    m_currentMap->WorldToTile(px + pw / 2, py + ph / 2, tileX, tileY);

    if (m_input->IsKeyPressed(KEY_T)) {
        if (m_energy && !m_energy->HasEnergy(Energy::COST_TILL)) {
            m_actionText = "Too tired to till!";
        } else if (m_currentMap->TillSoil(tileX, tileY)) {
            if (m_energy) m_energy->Consume(Energy::COST_TILL);
            if (m_skills) m_skills->AddXP(SkillType::FARMING, 5);
            m_actionText = "Tilled soil!";
            Logger::Instance().Info("Tilled soil at (" + std::to_string(tileX) + "," + std::to_string(tileY) + ")");
        }
    }

    if (m_input->IsKeyPressed(KEY_R)) {
        if (m_energy && !m_energy->HasEnergy(Energy::COST_WATER)) {
            m_actionText = "Too tired to water!";
        } else if (m_currentMap->WaterTile(tileX, tileY)) {
            if (m_energy) m_energy->Consume(Energy::COST_WATER);
            if (m_skills) m_skills->AddXP(SkillType::FARMING, 3);
            m_actionText = "Watered tile!";
            Logger::Instance().Info("Watered tile at (" + std::to_string(tileX) + "," + std::to_string(tileY) + ")");
        }
    }

    if (m_input->IsKeyPressed(KEY_F)) {
        if (m_energy && !m_energy->HasEnergy(Energy::COST_PLANT)) {
            m_actionText = "Too tired to plant!";
        } else if (m_currentMap->PlantCrop(tileX, tileY, static_cast<int>(CropType::PARSNIP))) {
            if (m_energy) m_energy->Consume(Energy::COST_PLANT);
            if (m_skills) m_skills->AddXP(SkillType::FARMING, 4);
            m_actionText = "Planted Parsnip!";
            Logger::Instance().Info("Planted crop at (" + std::to_string(tileX) + "," + std::to_string(tileY) + ")");
        }
    }

    if (m_input->IsKeyPressed(KEY_H)) {
        Tile* tile = m_currentMap->GetTileAt(tileX, tileY);
        if (tile && tile->GetSoilState() == SoilState::HARVEST) {
            if (m_energy && !m_energy->HasEnergy(Energy::COST_HARVEST)) {
                m_actionText = "Too tired to harvest!";
            } else {
                CropType type = static_cast<CropType>(tile->GetCropType());
                std::string cropName = FarmingSystem::GetCropName(type);
                int cropValue = FarmingSystem::GetCropValue(type);

                m_currentMap->HarvestCrop(tileX, tileY);
                if (m_energy) m_energy->Consume(Energy::COST_HARVEST);
                if (m_skills) m_skills->AddXP(SkillType::FARMING, 8);
                m_inventory->AddItem(cropName, 1);
                m_gold += cropValue;
                m_actionText = "Harvested " + cropName + "! +" + std::to_string(cropValue) + "g";
                Logger::Instance().Info("Harvested " + cropName + " for " + std::to_string(cropValue) + " gold");

                // Update quest objective
                if (m_questSystem) {
                    m_questSystem->UpdateObjective("farm_beginnings", 0, 1);
                    m_questSystem->CheckCompletion("farm_beginnings");
                }
            }
        }
    }
}

void Game::HandleTreeChopping() {
    if (!m_player || !m_currentMap) return;

    if (m_input->IsKeyPressed(KEY_C)) {
        float px, py;
        m_player->GetPosition(px, py);
        float pw, ph;
        m_player->GetSize(pw, ph);

        int tileX, tileY;
        m_currentMap->WorldToTile(px + pw / 2, py + ph / 2, tileX, tileY);

        if (m_energy && !m_energy->HasEnergy(Energy::COST_CHOP)) {
            m_actionText = "Too tired to chop!";
        } else if (m_currentMap->ChopTree(tileX, tileY)) {
            if (m_energy) m_energy->Consume(Energy::COST_CHOP);
            if (m_skills) m_skills->AddXP(SkillType::FORAGING, 6);
            m_inventory->AddItem("Wood", 3);
            m_actionText = "Chopped tree! +3 Wood";
            Logger::Instance().Info("Chopped tree at (" + std::to_string(tileX) + "," + std::to_string(tileY) + ")");

            // Update quest objective
            if (m_questSystem) {
                m_questSystem->UpdateObjective("lumberjack", 0, 3);
                m_questSystem->CheckCompletion("lumberjack");
            }
        }
    }
}

void Game::HandleCombatActions() {
    if (!m_player) return;

    if (m_input->IsKeyPressed(KEY_SPACE)) {
        if (m_energy && !m_energy->HasEnergy(Energy::COST_ATTACK)) {
            m_actionText = "Too tired to attack!";
        } else {
            if (m_energy) m_energy->Consume(Energy::COST_ATTACK);
            bool hitAny = false;
            for (auto& enemy : m_enemies) {
                if (Combat::PlayerAttack(m_player.get(), enemy.get(), 1)) {
                    hitAny = true;
                    if (m_skills) m_skills->AddXP(SkillType::COMBAT, 10);
                    if (!enemy->IsActive()) {
                        m_gold += Combat::ENEMY_KILL_GOLD;
                        m_actionText = "Enemy defeated! +" + std::to_string(Combat::ENEMY_KILL_GOLD) + "g";
                        Logger::Instance().Info("Enemy defeated! Gold: " + std::to_string(m_gold));

                        // Update quest objective
                        if (m_questSystem) {
                            m_questSystem->UpdateObjective("monster_slayer", 0, 1);
                            m_questSystem->CheckCompletion("monster_slayer");
                        }
                    } else {
                        m_actionText = "Hit enemy! HP: " + std::to_string(enemy->GetHealth());
                    }
                }
            }
            if (!hitAny) {
                m_actionText = "Attack!";
            }
        }
    }
}

void Game::AdvanceDay() {
    if (!m_calendar || !m_currentMap) return;

    m_calendar->AdvanceDay();

    // Restore energy on sleep
    if (m_energy) {
        m_energy->RestoreFull();
    }
    
    // Grow crops on the map
    int width = m_currentMap->GetWidth();
    int height = m_currentMap->GetHeight();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Tile* tile = m_currentMap->GetTileAt(x, y);
            if (tile && tile->GetSoilState() == SoilState::CROP) {
                int stage = tile->GetGrowthStage();
                CropType type = static_cast<CropType>(tile->GetCropType());
                int maxDays = FarmingSystem::GetGrowthDays(type);
                
                stage++;
                if (stage >= maxDays) {
                    tile->SetSoilState(SoilState::HARVEST);
                    tile->SetGrowthStage(Tile::MAX_GROWTH_STAGE);
                } else {
                    tile->SetGrowthStage(stage);
                }
            }
        }
    }

    m_actionText = m_calendar->GetSeasonName() + " " + std::to_string(m_calendar->GetDay()) + " - Day advanced!";
    Logger::Instance().Info("Day advanced: " + m_calendar->GetSeasonName() + " " + std::to_string(m_calendar->GetDay()));
}

void Game::SpawnEnemies() {
    m_enemies.clear();

    // Spawn enemies on walkable floor tiles
    int width = m_currentMap->GetWidth();
    int height = m_currentMap->GetHeight();
    int spawned = 0;

    for (int y = SPAWN_BORDER; y < height - SPAWN_BORDER && spawned < MAX_ENEMIES; ++y) {
        for (int x = SPAWN_BORDER; x < width - SPAWN_BORDER && spawned < MAX_ENEMIES; ++x) {
            const Tile* tile = m_currentMap->GetTileAt(x, y);
            if (tile && tile->GetType() == TileType::FLOOR && !tile->IsSolid()) {
                // Only spawn on some floor tiles (spread them out)
                if ((x + y) % SPAWN_SPACING == 0) {
                    auto enemy = std::make_unique<Enemy>();
                    float wx, wy;
                    m_currentMap->TileToWorld(x, y, wx, wy);
                    enemy->SetPosition(wx, wy);
                    enemy->SetPatrolOrigin(wx, wy);
                    enemy->SetSize(28, 28);
                    enemy->SetAIState(Enemy::AIState::PATROL);
                    m_enemies.push_back(std::move(enemy));
                    spawned++;
                }
            }
        }
    }
    Logger::Instance().Info("Spawned " + std::to_string(spawned) + " enemies");
}

void Game::SpawnNPCs() {
    m_npcs.clear();
    if (!m_currentMap) return;

    // Create a few NPCs on the farm
    struct NPCDef {
        const char* name;
        float x;
        float y;
    };

    NPCDef defs[] = {
        {"Farmer Gus", 200.0f, 150.0f},
        {"Merchant Lily", 500.0f, 300.0f},
        {"Blacksmith Oren", 350.0f, 450.0f},
    };

    for (const auto& def : defs) {
        auto npc = std::make_unique<NPC>();
        npc->SetName(def.name);
        npc->SetPosition(def.x, def.y);
        npc->SetSize(32, 32);

        // Build a small dialogue tree for each NPC
        Dialogue& dlg = npc->GetDialogue();

        // Node 0: greeting
        DialogueNode greet;
        greet.speakerLine = std::string(def.name) + ": Hello there, traveler!";
        greet.choices.push_back({"Tell me about yourself.", 1});
        greet.choices.push_back({"Goodbye.", -1});
        greet.nextNodeIndex = -1;
        dlg.AddNode(greet);

        // Node 1: about
        DialogueNode about;
        about.speakerLine = std::string(def.name) + ": I've lived in Meadowbrook all my life.";
        about.nextNodeIndex = -1;
        dlg.AddNode(about);

        // Simple schedule: stay at spawn, wander a bit mid-day
        npc->AddScheduleEntry(0, def.x, def.y);
        npc->AddScheduleEntry(8, def.x + 40.0f, def.y - 20.0f);
        npc->AddScheduleEntry(18, def.x, def.y);

        m_npcs.push_back(std::move(npc));
    }

    Logger::Instance().Info("Spawned " + std::to_string(m_npcs.size()) + " NPCs");
}

void Game::HandleCrafting() {
    if (!m_showCrafting || !m_crafting || !m_inventory) return;

    // Navigate recipes
    if (m_input->IsKeyPressed(KEY_UP)) {
        m_craftingIndex--;
        if (m_craftingIndex < 0) m_craftingIndex = m_crafting->GetRecipeCount() - 1;
    }
    if (m_input->IsKeyPressed(KEY_DOWN)) {
        m_craftingIndex++;
        if (m_craftingIndex >= m_crafting->GetRecipeCount()) m_craftingIndex = 0;
    }

    // Craft selected recipe with Enter
    if (m_input->IsKeyPressed(KEY_ENTER)) {
        if (m_crafting->Craft(m_craftingIndex, m_inventory.get())) {
            const CraftingRecipe& recipe = m_crafting->GetRecipe(m_craftingIndex);
            m_actionText = "Crafted " + recipe.resultName + "!";
            Logger::Instance().Info("Crafted: " + recipe.resultName);
        } else {
            m_actionText = "Not enough materials!";
        }
    }
}

void Game::HandleNPCInteraction() {
    if (!m_player) return;

    if (m_input->IsKeyPressed(KEY_E)) {
        float px, py;
        m_player->GetPosition(px, py);

        for (auto& npc : m_npcs) {
            if (npc && npc->IsActive() && npc->IsPlayerNearby(px, py)) {
                Dialogue& dlg = npc->GetDialogue();
                if (!dlg.IsActive()) {
                    dlg.Start();
                    npc->AddFriendship(1);
                    m_dialogueChoiceIndex = 0;
                    const DialogueNode* node = dlg.GetCurrentNode();
                    if (node) {
                        m_actionText = node->speakerLine;
                        if (!node->choices.empty()) {
                            m_actionText += " [" + node->choices[0].text + "]";
                        }
                    }

                    // Update quest objective
                    if (m_questSystem) {
                        m_questSystem->UpdateObjective("community_helper", 0, 1);
                        m_questSystem->CheckCompletion("community_helper");
                    }
                } else {
                    // Advance or close dialogue
                    const DialogueNode* node = dlg.GetCurrentNode();
                    if (node && node->choices.empty()) {
                        dlg.Advance();
                    } else if (node && !node->choices.empty()) {
                        // Use Up/Down to navigate choices, E to confirm
                        dlg.SelectChoice(m_dialogueChoiceIndex);
                        m_dialogueChoiceIndex = 0;
                    }
                    const DialogueNode* next = dlg.GetCurrentNode();
                    if (next) {
                        m_actionText = next->speakerLine;
                        if (!next->choices.empty()) {
                            int idx = std::min(m_dialogueChoiceIndex, static_cast<int>(next->choices.size()) - 1);
                            m_actionText += " [" + next->choices[idx].text + "]";
                        }
                    } else {
                        m_actionText = npc->GetName() + ": See you later!";
                    }
                }
                break; // Only talk to closest NPC
            }
        }
    }

    // Navigate dialogue choices with Up/Down when in dialogue
    if (m_input->IsKeyPressed(KEY_UP)) {
        if (m_dialogueChoiceIndex > 0) m_dialogueChoiceIndex--;
    }
    if (m_input->IsKeyPressed(KEY_DOWN)) {
        m_dialogueChoiceIndex++;
    }
}

void Game::HandleFishing() {
    if (!m_player || !m_fishingSystem || !m_calendar) return;

    if (m_input->IsKeyPressed(KEY_G)) {
        // Check if near water
        float px, py;
        m_player->GetPosition(px, py);
        float pw, ph;
        m_player->GetSize(pw, ph);
        int tileX, tileY;
        m_currentMap->WorldToTile(px + pw / 2, py + ph / 2, tileX, tileY);

        // Check adjacent tiles for water
        bool nearWater = false;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                const Tile* adj = m_currentMap->GetTileAt(tileX + dx, tileY + dy);
                if (adj && adj->GetType() == TileType::WATER) {
                    nearWater = true;
                    break;
                }
            }
            if (nearWater) break;
        }

        if (!nearWater) {
            m_actionText = "No water nearby to fish!";
            return;
        }

        if (m_energy && !m_energy->HasEnergy(Energy::COST_FISH_CAST)) {
            m_actionText = "Too tired to fish!";
            return;
        }

        if (m_energy) m_energy->Consume(Energy::COST_FISH_CAST);
        int skillLevel = m_skills ? m_skills->GetLevel(SkillType::FISHING) : 0;
        int result = m_fishingSystem->AttemptCatch(m_calendar->GetSeason(), skillLevel);

        if (result >= 0) {
            const FishType* fish = m_fishingSystem->GetFish(result);
            if (fish) {
                m_inventory->AddItem(fish->name, 1);
                m_gold += fish->value;
                if (m_skills) m_skills->AddXP(SkillType::FISHING, fish->difficulty * 5);
                m_actionText = "Caught " + fish->name + "! +" + std::to_string(fish->value) + "g";
                Logger::Instance().Info("Caught: " + fish->name + " (value: " + std::to_string(fish->value) + ")");
            }
        } else {
            if (m_skills) m_skills->AddXP(SkillType::FISHING, 2);
            m_actionText = "The fish got away...";
        }
    }
}

void Game::HandleSaveLoad() {
    if (m_input->IsKeyPressed(KEY_F5)) {
        if (SaveSystem::Save(SaveSystem::DEFAULT_SAVE_PATH,
                             m_player.get(), m_inventory.get(),
                             m_calendar.get(), m_gold,
                             m_energy.get(), m_skills.get(),
                             m_questSystem.get())) {
            m_actionText = "Game saved!";
        } else {
            m_actionText = "Save failed!";
        }
    }

    if (m_input->IsKeyPressed(KEY_F9)) {
        if (SaveSystem::Load(SaveSystem::DEFAULT_SAVE_PATH,
                             m_player.get(), m_inventory.get(),
                             m_calendar.get(), m_gold,
                             m_energy.get(), m_skills.get(),
                             m_questSystem.get())) {
            m_actionText = "Game loaded!";
        } else {
            m_actionText = "No save file found!";
        }
    }
}

void Game::UpdateHUD() {
    if (!m_hud || !m_player || !m_calendar) return;

    m_hud->SetHealth(m_player->GetHealth(), m_player->GetMaxHealth());
    m_hud->SetGold(m_gold);
    if (m_energy) {
        m_hud->SetEnergy(m_energy->GetCurrent(), m_energy->GetMax());
    }
    m_hud->SetDayInfo(m_calendar->GetDay(), m_calendar->GetSeasonName(), m_calendar->GetYear());
    m_hud->SetActionText(m_actionText);
    m_hud->SetShowInventory(m_showInventory || m_showCrafting);

    // Update inventory/crafting display
    m_hud->ClearInventoryLines();
    if (m_showCrafting && m_crafting) {
        m_hud->AddInventoryLine("=== CRAFTING (Up/Down, Enter) ===");
        for (int i = 0; i < m_crafting->GetRecipeCount(); ++i) {
            const CraftingRecipe& recipe = m_crafting->GetRecipe(i);
            std::string line;
            if (i == m_craftingIndex) line += "> "; else line += "  ";
            line += recipe.resultName + " (";
            for (size_t j = 0; j < recipe.ingredients.size(); ++j) {
                if (j > 0) line += ", ";
                line += std::to_string(recipe.ingredients[j].quantity) + " " + recipe.ingredients[j].name;
            }
            line += ")";
            if (!m_crafting->CanCraft(i, m_inventory.get())) line += " [need more]";
            m_hud->AddInventoryLine(line);
        }
    } else if (m_showInventory && m_inventory) {
        for (const auto& item : m_inventory->GetItems()) {
            std::string line = item.name + " x" + std::to_string(item.quantity);
            m_hud->AddInventoryLine(line);
        }
    }
}

void Game::Render() {
    m_renderer->Clear(20, 20, 30); // Dark background

    // Render map
    if (m_currentMap) {
        m_currentMap->Render(m_renderer.get());
    }

    // Render enemies
    for (auto& enemy : m_enemies) {
        if (enemy && enemy->IsActive()) {
            enemy->Render(m_renderer.get());
        }
    }

    // Render NPCs
    for (auto& npc : m_npcs) {
        if (npc && npc->IsActive()) {
            npc->Render(m_renderer.get());
        }
    }

    // Render player
    if (m_player) {
        m_player->Render(m_renderer.get());
    }

    // Render HUD (on top of everything)
    if (m_hud) {
        m_hud->Render(m_renderer.get());
    }

    m_renderer->Present();
}

void Game::Shutdown() {
    m_enemies.clear();
    m_npcs.clear();
    m_hud.reset();
    m_calendar.reset();
    m_inventory.reset();
    m_crafting.reset();
    m_energy.reset();
    m_skills.reset();
    m_questSystem.reset();
    m_fishingSystem.reset();
    m_player.reset();
    m_currentMap.reset();
    
    // Cleanup sprite sheets
    SpriteSheetManager::Instance().Clear();
    
    m_audioManager.reset();
    m_assetManager.reset();
    m_input.reset();
    m_renderer.reset();

    if (IsWindowReady()) {
        CloseWindow();
    }

    Logger::Instance().Info("Game shutdown complete");
}
