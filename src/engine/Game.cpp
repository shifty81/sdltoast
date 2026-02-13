#include "Game.h"
#include "Renderer.h"
#include "Input.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "SpriteSheet.h"
#include "Logger.h"
#include "../entities/Player.h"
#include "../world/Map.h"
#include "../world/WorldGenerator.h"
#include "../world/Tile.h"
#include <raylib.h>
#include <iostream>

Game::Game()
    : m_running(false)
    , m_windowWidth(0)
    , m_windowHeight(0)
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
    
    // Generate world using new system
    m_currentMap = std::make_unique<Map>(25, 19);
    
    Logger::Instance().Info("");
    Logger::Instance().Info("=== WORLD GENERATION DEMO ===");
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
        Logger::Instance().Info("Generated: Farm");
    }
    if (m_input->IsKeyPressed(KEY_TWO)) {
        WorldGenerator generator;
        generator.GenerateDungeon(m_currentMap.get(), 25, 19);
        Logger::Instance().Info("Generated: Dungeon");
    }
    if (m_input->IsKeyPressed(KEY_THREE)) {
        WorldGenerator generator;
        generator.GenerateOverworld(m_currentMap.get(), 25, 19, Biome::PLAINS);
        Logger::Instance().Info("Generated: Overworld");
    }

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

    // Update map
    if (m_currentMap) {
        m_currentMap->Update(deltaTime);
    }
}

void Game::Render() {
    m_renderer->Clear(20, 20, 30); // Dark background

    // Render map
    if (m_currentMap) {
        m_currentMap->Render(m_renderer.get());
    }

    // Render player
    if (m_player) {
        m_player->Render(m_renderer.get());
    }

    m_renderer->Present();
}

void Game::Shutdown() {
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
