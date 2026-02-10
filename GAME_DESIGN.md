# Harvest Quest - Game Design Document

## Concept
A top-down action-adventure RPG that combines the dungeon exploration and combat of *The Legend of Zelda: A Link to the Past* with the farming, crafting, and social simulation aspects of *Stardew Valley*.

## Core Gameplay Loop

### Day Phase
1. **Morning**: Wake up on your farm
2. **Farming**: Tend to crops (water, plant, harvest)
3. **Exploration**: Venture into nearby dungeons and wilderness
4. **Combat**: Fight monsters using sword, bow, and magical tools
5. **Gathering**: Collect resources from the world
6. **Social**: Interact with NPCs in the village
7. **Evening**: Return home, craft items, manage inventory
8. **Night**: Sleep to restore health and start new day

## Key Features

### From Zelda: A Link to the Past
- **Top-down perspective** with 2D sprite-based graphics
- **Action combat system** - Real-time sword fighting and dodging
- **Heart-based health system** - Collect heart containers to increase max HP
- **Dungeon exploration** - Multi-room dungeons with puzzles and bosses
- **Item progression** - Find tools that unlock new areas (hookshot, bombs, etc.)
- **Secrets and hidden areas** - Breakable walls, hidden chests
- **Boss battles** - Epic encounters requiring pattern recognition
- **Magic system** - MP-based special abilities

### From Stardew Valley
- **Farming mechanics** - Plant, water, and harvest crops
- **Seasonal calendar** - 4 seasons, 28 days each
- **Day/night cycle** - Energy management throughout the day
- **NPC relationships** - Friendship and romance systems
- **Crafting system** - Combine resources to create items
- **Fishing mini-game** - Catch fish in rivers and lakes
- **Mining and foraging** - Gather resources from the world
- **Town/Village hub** - Central location with shops and NPCs
- **Skill progression** - Level up farming, combat, mining, fishing skills
- **Community center/quests** - Long-term goals and bundles

## Game World

### The Farm (Home Base)
- Player's starting location
- Expandable farmland for crops
- Barn for animals
- Workshop for crafting
- Storage chests

### Village of Meadowbrook
- General store for buying seeds and supplies
- Blacksmith for upgrading tools and weapons
- Tavern for socializing
- 10-15 unique NPCs with daily schedules
- Quest board for side missions

### Wilderness Areas
- Forest (foraging, wood gathering)
- Mountains (mining, caves)
- Beach/Lake (fishing)
- Ancient Ruins (combat encounters)

### Dungeons (3-5 Themed)
1. **Forest Temple** - Nature themed, vine puzzles
2. **Crystal Caverns** - Ice/water themed, sliding puzzles
3. **Desert Pyramid** - Fire themed, light reflection puzzles
4. **Shadow Catacombs** - Dark themed, key/door puzzles
5. **Sky Tower** - Wind themed, hookshot-based traversal

## Progression Systems

### Combat Progression
- Start with basic wooden sword
- Find/craft better weapons: Iron Sword, Silver Sword, Master Sword
- Learn special techniques: Spin Attack, Charge Attack
- Acquire tools: Bow, Bombs, Hookshot, Boomerang
- Increase heart containers (3 hearts → 20 hearts max)
- Magic meter for special abilities

### Farming Progression
- Start with basic seeds (parsnips, potatoes)
- Unlock seasonal crops through exploration and relationships
- Upgrade watering can (covers more tiles)
- Build sprinklers for automation
- Raise animals for eggs, milk, wool
- Process crops into artisan goods

### Skills (Level 1-10 each)
- **Farming**: Increased crop quality, faster growth
- **Combat**: More damage, special techniques
- **Mining**: Better ore drops, less energy per swing
- **Fishing**: Easier catches, better fish
- **Foraging**: Rare item finds, increased yields

## Core Game Systems

### Time Management
- Each day is 20 minutes of real-time
- Energy system limits daily activities
- Must sleep before 2 AM or pass out
- Seasons change every 28 days

### Resource Management
- Gold for purchasing items
- Energy for performing actions
- Materials for crafting
- Seeds for farming
- Health during combat

### Relationship System
- 10 friendship hearts per NPC
- Give gifts to increase friendship
- Heart events unlock story scenes
- Romance options for 6+ NPCs
- Marriage and children possible

## Technical Implementation (SDL2)

### Core Components
- **Game Engine**: Custom SDL2-based engine
- **Rendering**: 2D sprite renderer with tile-based maps
- **Input**: Keyboard/gamepad support
- **Audio**: SDL_mixer for music and sound effects
- **Save System**: JSON-based save files

### Asset Requirements
- **Sprites**: Character animations (walk, attack, idle)
- **Tiles**: Ground, walls, decorations (16x16 or 32x32)
- **Items**: Tools, weapons, crops, resources
- **UI Elements**: Health bars, inventory screens, dialogue boxes
- **Music**: Background tracks for different areas
- **Sound Effects**: Combat, farming, interactions

### Map Format
```
{
  "width": 50,
  "height": 50,
  "layers": [
    "ground",
    "objects",
    "collision"
  ],
  "tiles": [...],
  "entities": [...]
}
```

## Win Conditions

### Main Quest
1. Restore the farm to prosperity
2. Clear all 5 dungeons and defeat bosses
3. Collect the 5 elemental crystals
4. Defeat the final boss in the Dark Tower
5. Bring peace to the land

### Optional Goals
- Complete community center bundles
- Max all skills to level 10
- Marry an NPC
- Complete all side quests
- Catch all fish
- Grow all crops
- Discover all secrets

## Unique Selling Points

1. **Dual gameplay loops**: Relaxing farming by day, exciting combat by night
2. **Meaningful progression**: Combat tools help with farming, farming resources help with combat
3. **No time pressure**: Dungeons don't have time limits
4. **Seasonal variety**: Different crops and events each season
5. **Player choice**: Focus on farming, combat, or balanced approach
6. **Replayability**: Different farm layouts, relationship choices, skill builds

## Development Roadmap

### Phase 1: Core Engine (Weeks 1-3)
- SDL2 setup and window management
- Sprite rendering and animation
- Input handling
- Tile-based map system
- Camera/viewport

### Phase 2: Movement & World (Weeks 4-6)
- Character movement (8-directional or 4-directional)
- Collision detection
- Map loading from files
- Transitions between areas
- Basic NPC movement

### Phase 3: Combat System (Weeks 7-9)
- Sword attack animations
- Enemy AI (basic patrol, chase, attack)
- Health system
- Damage calculation
- Combat effects

### Phase 4: Farming System (Weeks 10-12)
- Soil tilling
- Seed planting
- Crop growth over time
- Watering mechanics
- Harvesting

### Phase 5: Core Systems (Weeks 13-16)
- Inventory management
- Day/night cycle
- Calendar system
- Energy system
- Save/load functionality

### Phase 6: Polish & Content (Weeks 17-20)
- UI design
- Dialogue system
- Quest system
- Sound effects and music
- Multiple dungeons and areas
- NPC schedules and events

## Technical Architecture

```
sdltoast/
├── src/
│   ├── main.cpp
│   ├── engine/
│   │   ├── Game.cpp/h          # Main game loop
│   │   ├── Renderer.cpp/h      # Rendering system
│   │   ├── Input.cpp/h         # Input handling
│   │   ├── AssetManager.cpp/h  # Resource loading
│   │   └── AudioManager.cpp/h  # Sound and music
│   ├── entities/
│   │   ├── Entity.cpp/h        # Base entity class
│   │   ├── Player.cpp/h        # Player character
│   │   ├── Enemy.cpp/h         # Enemy base class
│   │   └── NPC.cpp/h           # Non-player characters
│   ├── systems/
│   │   ├── Combat.cpp/h        # Combat mechanics
│   │   ├── Farming.cpp/h       # Farming mechanics
│   │   ├── Inventory.cpp/h     # Inventory system
│   │   ├── Calendar.cpp/h      # Time and season
│   │   ├── Crafting.cpp/h      # Crafting system
│   │   └── Dialogue.cpp/h      # NPC dialogue
│   ├── world/
│   │   ├── Map.cpp/h           # Tile map
│   │   ├── Tile.cpp/h          # Individual tiles
│   │   └── Dungeon.cpp/h       # Dungeon generation
│   └── ui/
│       ├── HUD.cpp/h           # Heads-up display
│       ├── Menu.cpp/h          # Menu system
│       └── DialogueBox.cpp/h   # Dialogue rendering
├── assets/
│   ├── sprites/
│   ├── tilesets/
│   ├── music/
│   ├── sfx/
│   └── maps/
├── CMakeLists.txt
└── README.md
```

## Target Platforms
- Windows (MSVC, MinGW)
- Linux (GCC, Clang)
- macOS (Clang)

## Dependencies
- SDL2 (core library)
- SDL2_image (PNG/sprite loading)
- SDL2_mixer (audio)
- SDL2_ttf (text rendering)
- nlohmann/json (save files, data)

## Art Style
- 16-bit inspired pixel art
- Character sprites: 16x16 or 32x32
- Tile size: 16x16
- Vibrant, colorful palette
- Distinct visual themes for each season and biome

## Audio Design
- Upbeat farming music (inspired by Stardew Valley)
- Epic dungeon themes (inspired by Zelda)
- Satisfying sound effects for actions
- Ambient sounds for atmosphere
- Character jingles for important NPCs

## Monetization & Distribution
- Free and open-source (MIT License)
- Community contributions welcome
- Potential for mod support
- Steam/itch.io distribution possible

## Success Metrics
- Engaging gameplay for 40+ hours
- Positive player feedback
- Active modding community
- Cross-platform stability
- 60 FPS performance target
