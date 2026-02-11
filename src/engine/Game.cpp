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
#include <iostream>

Game::Game()
    : m_running(false)
    , m_window(nullptr)
    , m_lastFrameTime(0)
{
}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize(const std::string& title, int width, int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        Logger::Instance().Error(std::string("SDL initialization failed: ") + SDL_GetError());
        return false;
    }

    // Create window
    m_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        Logger::Instance().Error(std::string("Window creation failed: ") + SDL_GetError());
        return false;
    }

    // Initialize subsystems
    m_renderer = std::make_unique<Renderer>();
    if (!m_renderer->Initialize(m_window)) {
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
    m_lastFrameTime = SDL_GetTicks();

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
    while (m_running) {
        Uint32 frameStart = SDL_GetTicks();

        HandleEvents();
        
        float deltaTime = (frameStart - m_lastFrameTime) / 1000.0f;
        m_lastFrameTime = frameStart;
        
        Update(deltaTime);
        Render();

        // Frame rate limiting
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - frameTime);
        }
    }
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_running = false;
        }
        
        m_input->ProcessEvent(event);
    }
}

void Game::Update(float deltaTime) {
    m_input->Update();
    
    if (m_input->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_running = false;
    }

    // World generation hotkeys
    if (m_input->IsKeyPressed(SDL_SCANCODE_1)) {
        WorldGenerator generator;
        generator.GenerateFarm(m_currentMap.get(), 25, 19);
        Logger::Instance().Info("Generated: Farm");
    }
    if (m_input->IsKeyPressed(SDL_SCANCODE_2)) {
        WorldGenerator generator;
        generator.GenerateDungeon(m_currentMap.get(), 25, 19);
        Logger::Instance().Info("Generated: Dungeon");
    }
    if (m_input->IsKeyPressed(SDL_SCANCODE_3)) {
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

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
    Logger::Instance().Info("Game shutdown complete");
}
