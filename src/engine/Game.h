#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include <vector>

class Renderer;
class Input;
class AssetManager;
class AudioManager;
class Player;
class Enemy;
class Map;
class HUD;
class Calendar;
class Inventory;

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

private:
    void HandleEvents();
    void Update(float deltaTime);
    void Render();

    // Player actions
    void HandleFarmingActions();
    void HandleCombatActions();
    void AdvanceDay();
    void SpawnEnemies();
    void UpdateHUD();

    bool m_running;
    int m_windowWidth;
    int m_windowHeight;

    // Core subsystems
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Input> m_input;
    std::unique_ptr<AssetManager> m_assetManager;
    std::unique_ptr<AudioManager> m_audioManager;

    // Game objects
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Map> m_currentMap;
    std::vector<std::unique_ptr<Enemy>> m_enemies;

    // Game systems
    std::unique_ptr<HUD> m_hud;
    std::unique_ptr<Calendar> m_calendar;
    std::unique_ptr<Inventory> m_inventory;

    // State
    int m_gold;
    bool m_showInventory;
    float m_damageCooldown; // Prevent rapid damage from contact
    std::string m_actionText;

    // Spawn tuning
    static constexpr int MAX_ENEMIES = 5;
    static constexpr int SPAWN_BORDER = 2;
    static constexpr int SPAWN_SPACING = 5;
};

#endif // GAME_H
