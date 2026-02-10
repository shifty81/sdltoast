# Root PNG Detection - Testing Guide

## How to Test

### Test 1: Named Files
```bash
# Place files in root with recognized names
cp your_world.png world_tileset.png
cp your_chars.png character_tileset.png

# Build and run
mkdir -p build && cd build
cmake ..
cmake --build .
./HarvestQuest

# Expected output:
# Checking root directory for PNG files...
#   ✓ Loaded world tiles from: world_tileset.png
#   ✓ Loaded character sprites from: character_tileset.png
```

### Test 2: Any 2 PNGs
```bash
# Place any 2 PNGs with any names
cp tiles_v1.png .
cp sprites_v2.png .

# Build and run (same as above)

# Expected output:
#   ✓ Loaded world tiles from: sprites_v2.png (1st PNG in root)
#   ✓ Loaded character sprites from: tiles_v1.png (2nd PNG in root)
# (Alphabetical order)
```

### Test 3: Using Setup Script
```bash
# Place PNGs in root
cp your_world.png tileset.png
cp your_chars.png characters.png

# Run setup script
./setup_root_pngs.sh

# Output shows where files were copied
# Then build and run normally
```

### Test 4: Fallback (No PNGs)
```bash
# No PNG files in root
# Build and run

# Expected output:
# Checking root directory for PNG files...
# (No messages about loading from root)
# Failed to load sprite sheet assets/tilesets/world_tileset.png...
# (Game continues with colored rectangles)
```

## Verification Points

✅ Game starts without errors
✅ Console shows PNG detection messages
✅ Sprites render (if valid PNGs) or colored squares (if not found)
✅ No crashes regardless of PNG presence
✅ Can switch between worlds with keys 1/2/3

## Code Path

1. `Game::Initialize()` calls `SpriteSheetManager::Instance().LoadDefaultAssets(renderer)`
2. `LoadDefaultAssets()` calls `CheckAndLoadRootPngs(renderer)`
3. `CheckAndLoadRootPngs()` scans root directory for .png files
4. Files are loaded via `LoadSpriteSheet()` with error handling
5. If root loading fails, falls back to `assets/` directories
6. If all loading fails, game uses colored rectangle fallback

## Technical Notes

**Directory Reading:**
- Uses `opendir()`/`readdir()` on Unix-like systems
- Would need Windows-specific code for Windows (currently using #ifdef)
- Files are sorted alphabetically for consistent ordering

**Supported Extensions:**
- `.png` (lowercase) currently
- Could extend to `.PNG` (uppercase) if needed

**Load Priority:**
1. Root directory (named patterns)
2. Root directory (alphabetical order)
3. Assets directory (standard paths)
4. Colored rectangle fallback

## Expected Behavior

| Scenario | Result |
|----------|--------|
| 2 PNGs in root | Loads from root, shows checkmarks |
| 1 PNG in root | Loads 1, tries assets/ for other |
| 0 PNGs in root | Falls back to assets/ directory |
| No PNGs anywhere | Uses colored rectangles (no crash) |

## Build Test

```bash
# From project root
mkdir -p build && cd build
cmake ..
cmake --build .

# Should compile without errors
# Key files changed:
# - src/engine/SpriteSheet.cpp (added CheckAndLoadRootPngs)
# - src/engine/SpriteSheet.h (added function declaration)
```

## Console Output Example

```
=== WORLD GENERATION DEMO ===
Press 1 = Farm | 2 = Dungeon | 3 = Overworld
============================

Game initialized successfully!

Controls:
  WASD/Arrows - Move
  1 - Generate Farm
  2 - Generate Dungeon
  3 - Generate Overworld
  ESC - Quit

=== Loading Sprite Sheets ===
Checking root directory for PNG files...
  ✓ Loaded world tiles from: tiles.png
  ✓ Loaded character sprites from: characters.png
Loaded sprite sheet: tiles.png
  Size: 256x256
  Tiles: 16x16 (256 total)
  Tile size: 16x16
Loaded sprite sheet: characters.png
  Size: 64x64
  Tiles: 4x4 (16 total)
  Tile size: 16x16
=== Sprite Sheets Loaded ===
Generated: Farm (default)
```

---

**Status:** ✅ Implementation complete and ready for testing with actual PNG files!
