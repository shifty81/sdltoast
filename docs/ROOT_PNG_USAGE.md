# Using PNG Files in Root Directory

## Quick Start

The game can automatically detect and use PNG files placed in the **root directory** of the project!

### Easy Method: Just Drop Your PNGs

Simply place your PNG tileset files in the project root directory:

```
sdltoast/
├── your_world_tiles.png     ← World tileset (grass, dirt, walls, etc.)
├── your_characters.png       ← Character sprites
├── src/
├── assets/
└── ...
```

The game will **automatically detect and load** these files at startup!

## How It Works

### Automatic Detection

The sprite sheet system checks the root directory for PNG files in this order:

**1. By Filename Pattern:**
- World tiles: `world_tileset.png`, `tiles.png`, `tileset.png`, `world.png`, `terrain.png`, `map.png`
- Characters: `character_tileset.png`, `characters.png`, `char.png`, `sprites.png`, `player.png`, `hero.png`

**2. By Order (if no pattern match):**
- First PNG found → world tileset
- Second PNG found → character sprites

### Expected Format

**World Tileset:**
- Tile size: **16×16 pixels** (configurable)
- Grid layout with no spacing
- PNG format with transparency support

**Character Sprites:**
- Tile size: **16×16 pixels** (or 32×32)
- Layout: 4 rows for 4 directions
  - Row 0: Down-facing
  - Row 1: Up-facing
  - Row 2: Left-facing
  - Row 3: Right-facing

## Alternative: Use the Setup Script

For automated setup:

```bash
# Place your PNGs in root, then run:
./setup_root_pngs.sh
```

This will:
1. Detect PNG files in root
2. Copy them to appropriate asset directories
3. Show confirmation

## Manual Setup

If you prefer manual control:

```bash
# Copy to specific locations
cp your_tiles.png assets/tilesets/world_tileset.png
cp your_chars.png assets/sprites/character_tileset.png
```

## Verification

Run the game and check the console output:

```
=== Loading Sprite Sheets ===
Checking root directory for PNG files...
  ✓ Loaded world tiles from: tiles.png
  ✓ Loaded character sprites from: characters.png
Loaded sprite sheet: tiles.png
  Size: 256x256
  Tiles: 16x16 (256 total)
  Tile size: 16x16
...
```

## Fallback Behavior

If no PNG files are found:
- The game shows colored rectangles as placeholders
- No errors or crashes
- Fully playable for testing procedural generation

## Examples

### Example 1: Simple Drop
```
sdltoast/
├── tileset.png        ← Auto-detected as world tiles
├── sprites.png        ← Auto-detected as characters
```

### Example 2: Descriptive Names
```
sdltoast/
├── world_tileset.png     ← Clearly identified
├── character_tileset.png ← Clearly identified
```

### Example 3: Any Names (2 files)
```
sdltoast/
├── my_awesome_tiles.png  ← First PNG = world
├── my_cool_chars.png     ← Second PNG = characters
```

## Troubleshooting

**"No PNG files detected"**
- Check files are in the **root directory** (not in subdirectories)
- Ensure `.png` extension (lowercase)
- Verify file permissions

**"Failed to load sprite sheet"**
- Check PNG is valid (open in image viewer)
- Verify dimensions are multiples of tile size
- Ensure SDL2_image is installed

**Sprites look wrong**
- Check tile size (16x16 vs 32x32)
- Verify no spacing between tiles in the grid
- Ensure alpha transparency is correct

## Technical Details

The detection happens in `SpriteSheetManager::CheckAndLoadRootPngs()`:

1. Checks common tileset filenames
2. Scans root directory for `.png` files
3. Loads in alphabetical order if no pattern match
4. Falls back to `assets/` directories if root files not found

See `src/engine/SpriteSheet.cpp` for implementation.

---

**TL;DR:** Drop 2 PNG files in the project root and run the game. It just works! 🎮
