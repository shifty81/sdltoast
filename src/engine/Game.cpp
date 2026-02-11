#include "Game.h"
#include "Renderer.h"
#include "Input.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "SpriteSheet.h"
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
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
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
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize subsystems
    m_renderer = std::make_unique<Renderer>();
    if (!m_renderer->Initialize(m_window)) {
        std::cerr << "Renderer initialization failed" << std::endl;
        return false;
    }

    m_input = std::make_unique<Input>();
    m_assetManager = std::make_unique<AssetManager>();
    m_audioManager = std::make_unique<AudioManager>();

    if (!m_assetManager->Initialize(m_renderer.get())) {
        std::cerr << "Asset manager initialization failed" << std::endl;
        return false;
    }

    if (!m_audioManager->Initialize()) {
        std::cerr << "Audio manager initialization failed" << std::endl;
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
    
    std::cout << "\n=== WORLD GENERATION DEMO ===" << std::endl;
    std::cout << "Press 1 = Farm | 2 = Dungeon | 3 = Overworld" << std::endl;
    std::cout << "============================\n" << std::endl;
    
    // Default: Generate farm
    WorldGenerator generator(12345);
    generator.GenerateFarm(m_currentMap.get(), 25, 19);
    std::cout << "Generated: Farm (default)" << std::endl;

    m_running = true;
    m_lastFrameTime = SDL_GetTicks();

    std::cout << "Game initialized successfully!" << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "  WASD/Arrows - Move" << std::endl;
    std::cout << "  1 - Generate Farm" << std::endl;
    std::cout << "  2 - Generate Dungeon" << std::endl;
    std::cout << "  3 - Generate Overworld" << std::endl;
    std::cout << "  ESC - Quit\n" << std::endl;
    
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
        std::cout << "Generated: Farm" << std::endl;
    }
    if (m_input->IsKeyPressed(SDL_SCANCODE_2)) {
        WorldGenerator generator;
        generator.GenerateDungeon(m_currentMap.get(), 25, 19);
        std::cout << "Generated: Dungeon" << std::endl;
    }
    if (m_input->IsKeyPressed(SDL_SCANCODE_3)) {
        WorldGenerator generator;
        generator.GenerateOverworld(m_currentMap.get(), 25, 19, Biome::PLAINS);
        std::cout << "Generated: Overworld" << std::endl;
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
        
        // Check all four corners of the player for solid tiles
        float pw = 32.0f;
        float ph = 32.0f;
        
        // Check horizontal movement
        int tlX, tlY, trX, trY, blX, blY, brX, brY;
        m_currentMap->WorldToTile(px, prevY, tlX, tlY);
        m_currentMap->WorldToTile(px + pw - 1, prevY, trX, trY);
        m_currentMap->WorldToTile(px, prevY + ph - 1, blX, blY);
        m_currentMap->WorldToTile(px + pw - 1, prevY + ph - 1, brX, brY);
        
        if (m_currentMap->IsSolid(tlX, tlY) || m_currentMap->IsSolid(trX, trY) ||
            m_currentMap->IsSolid(blX, blY) || m_currentMap->IsSolid(brX, brY)) {
            px = prevX; // Revert horizontal movement
        }
        
        // Check vertical movement
        m_currentMap->WorldToTile(px, py, tlX, tlY);
        m_currentMap->WorldToTile(px + pw - 1, py, trX, trY);
        m_currentMap->WorldToTile(px, py + ph - 1, blX, blY);
        m_currentMap->WorldToTile(px + pw - 1, py + ph - 1, brX, brY);
        
        if (m_currentMap->IsSolid(tlX, tlY) || m_currentMap->IsSolid(trX, trY) ||
            m_currentMap->IsSolid(blX, blY) || m_currentMap->IsSolid(brX, brY)) {
            py = prevY; // Revert vertical movement
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
    std::cout << "Game shutdown complete" << std::endl;
}
