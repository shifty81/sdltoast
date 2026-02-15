# Harvest Quest 🌾⚔️
### An Atlas Forge Game Project

> *When Zelda: A Link to the Past and Stardew Valley had a baby...*

A top-down action-adventure RPG that seamlessly blends dungeon exploration and real-time combat with peaceful farming, crafting, and village life. Built as an [Atlas Forge](https://github.com/shifty81/AtlasForge) game project with Raylib in C++.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![Engine](https://img.shields.io/badge/engine-Atlas%20Forge-blueviolet.svg)
![Raylib](https://img.shields.io/badge/Raylib-5.5-green.svg)

## 🎮 Concept

Experience the perfect blend of two gaming classics:
- **From Zelda**: Top-down combat, dungeon exploration, heart-based health, epic boss battles, hidden secrets
- **From Stardew Valley**: Farming mechanics, seasonal calendar, NPC relationships, crafting, day/night cycle, skill progression

Tend to your crops in the morning, explore dangerous dungeons in the afternoon, and socialize with villagers in the evening. Every day brings new adventures!

## ✨ Features

### 🗡️ Combat & Exploration
- Real-time top-down combat with sword, bow, bombs, and magic
- **Procedurally generated dungeons** with rooms and corridors (Zelda-style)
- Progressive item unlocks (hookshot, boomerang, etc.)
- Heart-based health system
- Secret areas and hidden treasures
- Boss room generation with combat locks

### 🌱 Farming & Crafting
- **Smart tile system** - tiles react to semantic rules
- Plant, water, and harvest seasonal crops
- **Soil state machine** (Grass → Hoe → Watered → Crop → Harvest)
- Raise animals for resources
- Craft tools, weapons, and items
- Upgrade your farm and equipment
- Process crops into artisan goods

### 🌍 World Generation
- **Tiles are dumb, data is smart** - meaning-first approach
- **Procedural farm generation** with designated zones
- **Procedural dungeon generation** with auto-tiling
- **Noise-based overworld** generation with biomes
- Auto-tiling system for seamless terrain transitions
- Decoration pass for environmental detail

### 🏘️ Social & RPG Elements
- Befriend 15-20 unique NPCs with daily schedules
- Romance and marriage system
- Quest and achievement system
- 4 seasons with 28 days each
- Day/night cycle with energy management
- Skill progression (Farming, Combat, Mining, Fishing)

## 🚀 Getting Started

### Prerequisites

- **C++ Compiler**: GCC 9+, Clang 10+, or MSVC 2019+
- **CMake**: 3.15 or higher
- **Raylib**: Automatically fetched via CMake FetchContent (no manual install needed)
- **Linux only**: X11 and OpenGL development headers

### Installing Build Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install cmake build-essential libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

#### macOS (Homebrew)
```bash
brew install cmake
```

#### Windows
No additional dependencies needed - just CMake and a C++ compiler (MSVC).

### Building the Game

```bash
# Clone the repository
git clone https://github.com/shifty81/sdltoast.git
cd sdltoast

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run
./HarvestQuest
```

### Running the Game

After building, run the executable from the build directory:
```bash
./HarvestQuest  # Linux/macOS
HarvestQuest.exe  # Windows
```

**Try the Procedural Generation!**
- Press **1** to generate a Farm
- Press **2** to generate a Dungeon
- Press **3** to generate an Overworld
- Watch the smart tile system in action!

## 🎯 Controls

### Keyboard
- **Arrow Keys / WASD**: Move character
- **Z / Space**: Attack/Interact
- **X / Shift**: Use equipped item
- **C**: Open inventory
- **Esc**: Pause menu
- **1-5**: Quick item slots

### Gamepad (Xbox layout)
- **Left Stick / D-Pad**: Move character
- **A**: Attack/Interact
- **B**: Use equipped item
- **X**: Open inventory
- **Start**: Pause menu
- **Bumpers**: Cycle through items

## 📁 Project Structure

This project follows the [Atlas Forge](https://github.com/shifty81/AtlasForge) project layout:

```
harvestquest/
├── harvestquest.atlas        # Atlas project manifest (.atlas)
├── Plugin.toml               # Atlas Forge plugin descriptor
├── CMakeLists.txt            # Build configuration
├── build.sh                  # Build script (Atlas-style)
├── src/                      # Game source code
│   ├── main.cpp              # Entry point
│   ├── engine/               # Core engine systems
│   │   ├── Game.cpp/h
│   │   ├── Renderer.cpp/h
│   │   ├── Input.cpp/h
│   │   ├── AssetManager.cpp/h
│   │   └── AudioManager.cpp/h
│   ├── entities/             # Game entities
│   │   ├── Player.cpp/h
│   │   ├── Enemy.cpp/h
│   │   └── NPC.cpp/h
│   ├── systems/              # Game systems
│   │   ├── Combat.cpp/h
│   │   ├── Farming.cpp/h
│   │   ├── Inventory.cpp/h
│   │   └── Calendar.cpp/h
│   ├── world/                # World and maps
│   │   ├── Map.cpp/h
│   │   └── Dungeon.cpp/h
│   └── ui/                   # User interface
│       ├── HUD.cpp/h
│       └── Menu.cpp/h
├── worlds/                   # WorldGraph files (Atlas Forge format)
│   ├── overworld.worldgraph
│   ├── dungeon.worldgraph
│   └── farm.worldgraph
├── assets/                   # Game assets
│   ├── sprites/
│   ├── tilesets/
│   ├── music/
│   └── sfx/
├── data/                     # Game content data
├── schemas/                  # JSON schemas for validation
├── docs/                     # Documentation
├── tools/                    # Development utilities
├── GAME_DESIGN.md            # Detailed design document
└── README.md                 # This file
```

## 🛠️ Development

### Atlas Forge Integration

This project is structured as an [Atlas Forge](https://github.com/shifty81/AtlasForge) game project.
It can be opened in the Atlas Editor or run with the Atlas Runtime:

```bash
# Open in Atlas Editor
./AtlasEditor --project harvestquest.atlas

# Run with Atlas Runtime
./AtlasRuntime --project harvestquest.atlas
```

### Code Style
- C++20 standard (per Atlas Forge conventions)
- Follow [Atlas Forge naming conventions](https://github.com/shifty81/AtlasForge/blob/main/docs/ATLAS_NAMING_CONVENTIONS.md)
- Use RAII for resource management
- Document public APIs

### Contributing
Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Testing
```bash
cd build
ctest --output-on-failure
```

## 📖 Documentation

- [Atlas Forge Engine](https://github.com/shifty81/AtlasForge) - Engine reference and architecture
- [Primary Design Reference](sdltoastnew.rtf) - **Primary** design reference document for the project
- [SDLDEV Reference Folder](SDLDEV/README.md) - Reference folder recommendations and development notes
- [Game Design Document](GAME_DESIGN.md) - Detailed gameplay and technical specifications
- [World Generation System](docs/WORLD_GENERATION.md) - Procedural generation deep-dive
- [Tileset Integration Guide](docs/TILESET_INTEGRATION.md) - How to add your own tilesets
- [Development Guide](DEVELOPMENT.md) - Code architecture and contribution guidelines
- [Quick Start Guide](QUICKSTART.md) - Get up and running fast
- [Asset Guidelines](assets/README.md) - How to create game assets

## 🎨 Assets

Currently using placeholder assets. Final game will feature:
- 16-bit style pixel art
- Chiptuneesque music
- Retro sound effects

**NEW: Root Directory PNG Support! 🎉**

You can now place PNG tileset files **directly in the project root directory** and the game will automatically detect and use them!

**Quick Start:**
1. Place your PNG files in the root: `world_tileset.png` and `character_tileset.png`
2. Run the game - they'll be automatically loaded!

**Supported naming patterns:**
- World tiles: `world_tileset.png`, `tiles.png`, `tileset.png`, etc.
- Characters: `character_tileset.png`, `characters.png`, `sprites.png`, etc.

Or just drop **any 2 PNG files** in the root - first becomes world tiles, second becomes characters!

See [Root PNG Usage Guide](docs/ROOT_PNG_USAGE.md) for complete details.

**Alternative:** Place PNGs in standard locations:
- `assets/tilesets/world_tileset.png` (16x16 tiles)
- `assets/sprites/character_tileset.png` (16x16 or 32x32)

The game gracefully falls back to colored rectangles if tileset files are missing.

See [Tileset Integration Guide](docs/TILESET_INTEGRATION.md) for asset format details.

## 🗺️ Roadmap

- [x] Core game design
- [x] Project structure
- [x] Basic Raylib engine setup
- [x] Character movement and collision
- [x] **Procedural world generation system**
- [x] **Smart tile system (semantic tiles)**
- [x] **Auto-tiling algorithm**
- [x] **Dungeon generation (Zelda-style)**
- [x] **Farm generation (Stardew-style)**
- [x] Combat system with enemies
- [x] Farming mechanics (crop growth timing)
- [x] Inventory system (UI)
- [x] Crafting system
- [x] Quest system with objectives and rewards
- [x] NPC dialogue and branching conversations
- [x] NPC roster and schedules
- [x] Save/load system
- [ ] Sprite assets and animations
- [ ] Music and sound effects
- [ ] Alpha release

See [GAME_DESIGN.md](GAME_DESIGN.md) for detailed development phases.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

Inspired by:
- **The Legend of Zelda: A Link to the Past** by Nintendo
- **Stardew Valley** by ConcernedApe

Built with:
- [Atlas Forge](https://github.com/shifty81/AtlasForge) — Modular, data-driven game engine
- [Raylib](https://www.raylib.com/) — A simple and easy-to-use library to enjoy videogames programming

## 📞 Contact

Project Link: [https://github.com/shifty81/sdltoast](https://github.com/shifty81/sdltoast)
Engine Link: [https://github.com/shifty81/AtlasForge](https://github.com/shifty81/AtlasForge)

---

*An Atlas Forge game project — Cultivate your farm, explore dungeons, save the world! 🌾⚔️✨*
