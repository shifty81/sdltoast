# Asset Guidelines

## Overview
This directory contains all game assets including sprites, tilesets, music, and sound effects.

## Directory Structure

```
assets/
├── sprites/        # Character and object sprites
├── tilesets/       # Tile graphics for maps
├── music/          # Background music tracks
├── sfx/            # Sound effects
└── maps/           # Map data files
```

## Asset Specifications

### Sprites (`sprites/`)

#### Character Sprites
- **Format**: PNG with transparency
- **Size**: 32x32 pixels per frame
- **Naming**: `character_action_direction_frame.png`
  - Example: `player_walk_down_01.png`

#### Animation Frames
- Walk cycle: 4 frames
- Attack: 3 frames
- Idle: 2 frames (or single frame)

#### Required Sprites
- Player character (all animations, 4 directions)
- NPCs (idle, walk)
- Enemies (idle, walk, attack)
- Items (crops, tools, weapons)

### Tilesets (`tilesets/`)

#### Tile Specifications
- **Format**: PNG
- **Tile size**: 16x16 pixels
- **Tileset layout**: Grid-based

#### Required Tilesets
- `grass.png` - Grass and farm terrain
- `dungeon.png` - Dungeon floors and walls
- `village.png` - Town buildings and paths
- `objects.png` - Trees, rocks, decorations

### Music (`music/`)

#### Format
- **Primary**: OGG Vorbis
- **Alternative**: MP3, WAV

#### Required Tracks
- `farm_day.ogg` - Daytime farming music
- `farm_night.ogg` - Nighttime ambient
- `village.ogg` - Town theme
- `dungeon_01.ogg` - First dungeon theme
- `boss_battle.ogg` - Boss fight music
- `title_screen.ogg` - Main menu music

#### Specifications
- Sample rate: 44100 Hz
- Stereo
- Loopable (seamless loop points)

### Sound Effects (`sfx/`)

#### Format
- **Primary**: WAV (uncompressed)
- **Alternative**: OGG (compressed)

#### Required Sounds
**Combat:**
- `sword_swing.wav`
- `sword_hit.wav`
- `player_hurt.wav`
- `enemy_defeat.wav`

**Farming:**
- `plant_seed.wav`
- `water_crop.wav`
- `harvest.wav`
- `till_soil.wav`

**UI:**
- `menu_select.wav`
- `menu_confirm.wav`
- `menu_cancel.wav`
- `item_pickup.wav`

**Environment:**
- `footstep_grass.wav`
- `door_open.wav`
- `chest_open.wav`

### Maps (`maps/`)

#### Format
Currently planned: JSON or custom text format

Example structure:
```json
{
  "name": "Farm",
  "width": 50,
  "height": 50,
  "tileSize": 16,
  "layers": {
    "ground": [...],
    "objects": [...],
    "collision": [...]
  },
  "entities": [
    {
      "type": "NPC",
      "name": "Farmer Joe",
      "x": 10,
      "y": 15
    }
  ]
}
```

## Art Style Guide

### Color Palette
- Bright, vibrant colors
- High saturation for visibility
- Distinct colors for different seasons:
  - Spring: Greens and pastels
  - Summer: Bright yellows and blues
  - Fall: Oranges and reds
  - Winter: Blues and whites

### Pixel Art Guidelines
- Clean, crisp pixels
- Consistent line thickness
- Anti-aliasing only on curves
- Readable at original size
- 16-bit inspired aesthetic

### Animation Principles
- Smooth transitions between frames
- Exaggerated movements for clarity
- Consistent timing (200ms per frame typical)
- Anticipation and follow-through

## Creating Assets

### Recommended Tools

#### Sprite Editing
- [Aseprite](https://www.aseprite.org/) - Pixel art animation
- [LibreSprite](https://libresprite.github.io/) - Free alternative to Aseprite
- [GIMP](https://www.gimp.org/) - General image editing
- [Piskel](https://www.piskelapp.com/) - Online pixel editor

#### Tileset Creation
- [Tiled](https://www.mapeditor.org/) - Tile map editor
- Aseprite - For creating individual tiles

#### Music Composition
- [LMMS](https://lmms.io/) - Free DAW
- [BeepBox](https://beepbox.co/) - Online chiptune maker
- [FamiStudio](https://famistudio.org/) - NES-style music

#### Sound Effects
- [Audacity](https://www.audacityteam.org/) - Audio editing
- [Bfxr](https://www.bfxr.net/) - Retro sound effect generator
- [ChipTone](https://sfbgames.itch.io/chiptone) - Chiptune SFX

## Placeholder Assets

Currently using colored rectangles as placeholders:
- Player: Blue (changes color with direction)
- Enemies: Red
- NPCs: Green
- Tiles: Green checkerboard

## Contributing Assets

### Submission Process
1. Follow the specifications above
2. Ensure assets are original or properly licensed
3. Place in appropriate directory
4. Update asset list in this file
5. Submit pull request

### Licensing
All contributed assets must be:
- Original work, or
- Public domain, or
- Licensed under compatible terms (CC0, CC-BY, MIT)

Please include attribution info in `CREDITS.md`

## Asset Credits

See [CREDITS.md](../CREDITS.md) for attribution of external assets.

---

*Need help creating assets? Join our community Discord! (Link TBD)*
