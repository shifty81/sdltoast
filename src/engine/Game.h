#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <memory>
#include <string>

class Renderer;
class Input;
class AssetManager;
class AudioManager;
class Player;
class Map;

/**
 * Main game class that manages the game loop and core systems
 */
class Game {
public:
    Game();
    ~Game();

    bool Initialize(const std::string& title, int width, int height);
    void Run();
    void Shutdown();

    // Game state
    bool IsRunning() const { return m_running; }
    void Quit() { m_running = false; }

    // Subsystem access
    Renderer* GetRenderer() const { return m_renderer.get(); }
    Input* GetInput() const { return m_input.get(); }
    AssetManager* GetAssets() const { return m_assetManager.get(); }
    AudioManager* GetAudio() const { return m_audioManager.get(); }

    // Game constants
    static constexpr int TARGET_FPS = 60;
    static constexpr int FRAME_TIME = 1000 / TARGET_FPS;

private:
    void HandleEvents();
    void Update(float deltaTime);
    void Render();

    bool m_running;
    SDL_Window* m_window;

    // Core subsystems
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Input> m_input;
    std::unique_ptr<AssetManager> m_assetManager;
    std::unique_ptr<AudioManager> m_audioManager;

    // Game objects
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Map> m_currentMap;

    // Timing
    Uint32 m_lastFrameTime;
};

#endif // GAME_H
