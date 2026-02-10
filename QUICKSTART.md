# Quick Start Guide

Welcome to **Harvest Quest**! This guide will get you up and running in minutes.

## What is Harvest Quest?

Harvest Quest is a game that combines:
- **The Legend of Zelda: A Link to the Past** - Top-down combat and dungeon exploration
- **Stardew Valley** - Farming, crafting, and social simulation

## Installation

### 1. Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install git cmake build-essential \
    libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

**macOS:**
```bash
brew install cmake sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

**Windows:**
- Install [Visual Studio](https://visualstudio.microsoft.com/)
- Install [CMake](https://cmake.org/download/)
- Install SDL2 via [vcpkg](https://vcpkg.io/):
  ```powershell
  vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf
  ```

### 2. Clone the Repository

```bash
git clone https://github.com/shifty81/sdltoast.git
cd sdltoast
```

### 3. Build the Game

**Linux/macOS:**
```bash
./build.sh
```

**Windows (with Visual Studio):**
```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 4. Run the Game

```bash
cd build
./HarvestQuest        # Linux/macOS
HarvestQuest.exe      # Windows
```

## Controls

### Keyboard
- **WASD** or **Arrow Keys** - Move your character
- **Z** or **Space** - Attack / Interact
- **X** or **Shift** - Use equipped item
- **C** - Open inventory
- **ESC** - Pause / Quit

### Movement Tips
- The player sprite changes color based on the direction you're facing
- Blue = Down, Green = Up, Red = Left, Yellow = Right

## Current Features (Alpha)

### ✅ Implemented
- Top-down character movement (WASD/Arrows)
- Tile-based world rendering
- Basic game loop (60 FPS)
- Input handling system
- Health display (hearts in top-left)
- Modular code structure

### 🚧 In Progress
- Combat system (structure ready)
- Farming mechanics (structure ready)
- NPC interactions (structure ready)
- Calendar/seasons (structure ready)
- Inventory system (structure ready)

### 📋 Planned
- Sprite graphics
- Multiple maps/dungeons
- Boss battles
- Crafting system
- Music and sound effects
- Save/load system

## What You'll See

Right now, the game shows:
1. **Green checkerboard background** - The farm/world
2. **Colored square** - Your player character
3. **Red hearts** - Your health (top-left corner)

Move around with WASD or arrow keys. The player square changes color based on direction!

## Next Steps

### For Players
- Check out [GAME_DESIGN.md](GAME_DESIGN.md) to see the full vision
- Join the community (Discord link coming soon!)
- Report bugs via GitHub Issues

### For Developers
- Read [DEVELOPMENT.md](DEVELOPMENT.md) for contribution guidelines
- Check [assets/README.md](assets/README.md) for asset guidelines
- Start coding! See open issues for tasks

## Troubleshooting

### "SDL2 not found" error
Make sure SDL2 is installed. On Ubuntu:
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

### Build fails with linking errors
Ensure all SDL2 libraries are installed. Check CMakeLists.txt matches your SDL2 installation.

### Game runs but shows black screen
This is a known issue being worked on. Check that assets directory exists in the build folder.

## Getting Help

- **Documentation**: Check README.md, GAME_DESIGN.md, DEVELOPMENT.md
- **Issues**: https://github.com/shifty81/sdltoast/issues
- **Community**: Discord (coming soon)

## Contributing

We welcome contributions! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

See [DEVELOPMENT.md](DEVELOPMENT.md) for detailed guidelines.

## Credits

Inspired by:
- The Legend of Zelda: A Link to the Past (Nintendo)
- Stardew Valley (ConcernedApe)

Built with SDL2 and love ❤️

---

**Happy adventuring! 🌾⚔️**
