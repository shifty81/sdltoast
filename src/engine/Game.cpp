#include "Game.h"
#include "Renderer.h"
#include "Input.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "SpriteSheet.h"
#include "Logger.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../world/Map.h"
#include "../world/WorldGenerator.h"
#include "../world/Tile.h"
#include "../systems/Combat.h"
#include "../systems/Farming.h"
#include "../systems/Inventory.h"
#include "../systems/Calendar.h"
#include "../ui/HUD.h"
#include <raylib.h>
#include <iostream>

Game::Game()
    : m_running(false)
    , m_windowWidth(0)
    , m_windowHeight(0)
    , m_gold(0)
    , m_showInventory(false)
    , m_damageCooldown(0.0f)
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
    Logger::Instance().Info("  N - Advance day");
    Logger::Instance().Info("  I - Toggle inventory");
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
        Logger::Instance().Info("Generated: Farm");
    }
    if (m_input->IsKeyPressed(KEY_TWO)) {
        WorldGenerator generator;
        generator.GenerateDungeon(m_currentMap.get(), 25, 19);
        SpawnEnemies();
        Logger::Instance().Info("Generated: Dungeon (with enemies)");
    }
    if (m_input->IsKeyPressed(KEY_THREE)) {
        WorldGenerator generator;
        generator.GenerateOverworld(m_currentMap.get(), 25, 19, Biome::PLAINS);
        m_enemies.clear();
        Logger::Instance().Info("Generated: Overworld");
    }

    // Toggle inventory
    if (m_input->IsKeyPressed(KEY_I)) {
        m_showInventory = !m_showInventory;
    }

    // Advance day
    if (m_input->IsKeyPressed(KEY_N)) {
        AdvanceDay();
    }

    // Farming actions
    HandleFarmingActions();

    // Combat actions
    HandleCombatActions();

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
        if (m_currentMap->TillSoil(tileX, tileY)) {
            m_actionText = "Tilled soil!";
            Logger::Instance().Info("Tilled soil at (" + std::to_string(tileX) + "," + std::to_string(tileY) + ")");
        }
    }

    if (m_input->IsKeyPressed(KEY_R)) {
        if (m_currentMap->WaterTile(tileX, tileY)) {
            m_actionText = "Watered tile!";
            Logger::Instance().Info("Watered tile at (" + std::to_string(tileX) + "," + std::to_string(tileY) + ")");
        }
    }

    if (m_input->IsKeyPressed(KEY_F)) {
        if (m_currentMap->PlantCrop(tileX, tileY, static_cast<int>(CropType::PARSNIP))) {
            m_actionText = "Planted Parsnip!";
            Logger::Instance().Info("Planted crop at (" + std::to_string(tileX) + "," + std::to_string(tileY) + ")");
        }
    }

    if (m_input->IsKeyPressed(KEY_H)) {
        Tile* tile = m_currentMap->GetTileAt(tileX, tileY);
        if (tile && tile->GetSoilState() == SoilState::HARVEST) {
            CropType type = static_cast<CropType>(tile->GetCropType());
            std::string cropName = FarmingSystem::GetCropName(type);
            int cropValue = FarmingSystem::GetCropValue(type);

            m_currentMap->HarvestCrop(tileX, tileY);
            m_inventory->AddItem(cropName, 1);
            m_gold += cropValue;
            m_actionText = "Harvested " + cropName + "! +" + std::to_string(cropValue) + "g";
            Logger::Instance().Info("Harvested " + cropName + " for " + std::to_string(cropValue) + " gold");
        }
    }
}

void Game::HandleCombatActions() {
    if (!m_player) return;

    if (m_input->IsKeyPressed(KEY_SPACE)) {
        bool hitAny = false;
        for (auto& enemy : m_enemies) {
            if (Combat::PlayerAttack(m_player.get(), enemy.get(), 1)) {
                hitAny = true;
                if (!enemy->IsActive()) {
                    m_gold += Combat::ENEMY_KILL_GOLD;
                    m_actionText = "Enemy defeated! +" + std::to_string(Combat::ENEMY_KILL_GOLD) + "g";
                    Logger::Instance().Info("Enemy defeated! Gold: " + std::to_string(m_gold));
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

void Game::AdvanceDay() {
    if (!m_calendar || !m_currentMap) return;

    m_calendar->AdvanceDay();
    
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

void Game::UpdateHUD() {
    if (!m_hud || !m_player || !m_calendar) return;

    m_hud->SetHealth(m_player->GetHealth(), m_player->GetMaxHealth());
    m_hud->SetGold(m_gold);
    m_hud->SetDayInfo(m_calendar->GetDay(), m_calendar->GetSeasonName(), m_calendar->GetYear());
    m_hud->SetActionText(m_actionText);
    m_hud->SetShowInventory(m_showInventory);

    // Update inventory display
    m_hud->ClearInventoryLines();
    if (m_inventory) {
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
    m_hud.reset();
    m_calendar.reset();
    m_inventory.reset();
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
