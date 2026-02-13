# Harvest Quest - Development Guide

## Setting Up Your Development Environment

### Required Tools
- C++ compiler (GCC 9+, Clang 10+, or MSVC 2019+)
- CMake 3.15+
- Raylib (automatically fetched via CMake FetchContent)
- Git

### IDE Recommendations
- **Visual Studio Code** with C++ extensions
- **CLion** (JetBrains)
- **Visual Studio** (Windows)
- **Xcode** (macOS)

## Building from Source

### Quick Start
```bash
# Clone the repository
git clone https://github.com/shifty81/sdltoast.git
cd sdltoast

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run the game
./HarvestQuest  # Linux/macOS
HarvestQuest.exe  # Windows
```

### Debug Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Release Build
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Project Architecture

### Engine Layer (`src/engine/`)
Core systems that power the game:
- **Game**: Main game loop and initialization
- **Renderer**: 2D graphics rendering with Raylib
- **Input**: Keyboard and gamepad input handling
- **AssetManager**: Loading and caching of textures/sprites
- **AudioManager**: Music and sound effects

### Entity Layer (`src/entities/`)
Game objects and characters:
- **Entity**: Base class for all game objects
- **Player**: Player character with movement and combat
- **Enemy**: Enemy AI and behavior
- **NPC**: Non-player characters with dialogue

### Systems Layer (`src/systems/`)
Game mechanics:
- **Combat**: Damage calculation and fighting mechanics
- **Farming**: Crop growth, planting, harvesting
- **Inventory**: Item management
- **Calendar**: Day/night cycle, seasons, time
- **Crafting**: Recipe system
- **Dialogue**: NPC conversation system

### World Layer (`src/world/`)
Environment and maps:
- **Map**: Tile-based world representation
- **Tile**: Individual tile properties
- **Dungeon**: Procedural dungeon generation

### UI Layer (`src/ui/`)
User interface:
- **HUD**: Heads-up display (health, energy, etc.)
- **Menu**: Menu systems
- **DialogueBox**: Text display for NPC dialogue

## Coding Standards

### Naming Conventions
- **Classes**: PascalCase (e.g., `PlayerController`)
- **Functions**: PascalCase (e.g., `GetHealth()`)
- **Variables**: camelCase with m_ prefix for members (e.g., `m_healthPoints`)
- **Constants**: UPPER_SNAKE_CASE (e.g., `MAX_HEALTH`)

### File Organization
- Header files: `.h` extension
- Implementation files: `.cpp` extension
- One class per file pair
- Headers include guards

### Code Style
```cpp
// Class example
class Player : public Entity {
public:
    Player();
    ~Player() override;
    
    void Update(float deltaTime) override;
    void Render(Renderer* renderer) override;
    
    int GetHealth() const { return m_health; }
    void SetHealth(int health) { m_health = health; }

private:
    int m_health;
    float m_x, m_y;
    
    static constexpr int MAX_HEALTH = 10;
};
```

## Adding New Features

### Adding a New Entity
1. Create header in `src/entities/EntityName.h`
2. Create implementation in `src/entities/EntityName.cpp`
3. Inherit from `Entity` base class
4. Implement `Update()` and `Render()` methods
5. Add to CMakeLists.txt

### Adding a New System
1. Create header in `src/systems/SystemName.h`
2. Create implementation in `src/systems/SystemName.cpp`
3. Add system to Game class if needed
4. Add to CMakeLists.txt

## Testing

### Manual Testing
Run the game and test your changes:
```bash
./HarvestQuest
```

### Performance Profiling
Use tools like:
- **Valgrind** (Linux) for memory leaks
- **Visual Studio Profiler** (Windows)
- **Instruments** (macOS)

## Debugging Tips

### Common Issues

**Raylib build dependencies not found**
```bash
# Ubuntu/Debian
sudo apt-get install cmake build-essential libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev

# macOS
brew install cmake

# Windows - no additional dependencies needed
```

**Linking errors**
- Ensure all source files are listed in CMakeLists.txt
- Check that Raylib was fetched successfully by CMake

**Runtime crashes**
- Enable debug symbols: `cmake -DCMAKE_BUILD_TYPE=Debug ..`
- Run with debugger: `gdb ./HarvestQuest`

## Git Workflow

### Feature Development
```bash
# Create feature branch
git checkout -b feature/new-feature

# Make changes and commit
git add .
git commit -m "Add new feature"

# Push to remote
git push origin feature/new-feature

# Create pull request on GitHub
```

### Commit Messages
Follow conventional commits:
- `feat: Add new farming mechanic`
- `fix: Fix player collision bug`
- `docs: Update README`
- `refactor: Improve rendering performance`

## Resources

### Raylib Documentation
- [Raylib Website](https://www.raylib.com/)
- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Raylib Examples](https://www.raylib.com/examples.html)

### Game Development
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [Red Blob Games](https://www.redblobgames.com/)

### C++ Best Practices
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)

## Design Reference

The primary design reference for the project is [sdltoastnew.rtf](sdltoastnew.rtf). Refer to this document for the latest game design specifications, architecture decisions, and development roadmap. The SDLDEV reference folder ([SDLDEV/README.md](SDLDEV/README.md)) captures additional recommendations and planning notes to incorporate as the roadmap evolves.

## Contributing

See [README.md](../README.md) for contribution guidelines.

## License

MIT License - see [LICENSE](../LICENSE) for details.
