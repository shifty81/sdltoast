#include "Game.h"
#include "Renderer.h"
#include "Input.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "../entities/Player.h"
#include "../world/Map.h"
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

    // Initialize game objects
    m_player = std::make_unique<Player>();
    m_player->SetPosition(400, 300); // Start in center
    
    m_currentMap = std::make_unique<Map>();
    m_currentMap->LoadFromFile("assets/maps/farm.map");

    m_running = true;
    m_lastFrameTime = SDL_GetTicks();

    std::cout << "Game initialized successfully!" << std::endl;
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

    // Update player
    if (m_player) {
        m_player->Update(deltaTime, m_input.get());
    }

    // Update map
    if (m_currentMap) {
        m_currentMap->Update(deltaTime);
    }
}

void Game::Render() {
    m_renderer->Clear(50, 100, 50); // Green background (grass)

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
