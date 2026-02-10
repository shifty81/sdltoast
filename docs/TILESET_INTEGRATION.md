# Tileset Integration Guide

## Overview

The game now supports **actual sprite sheets/tilesets** for both world tiles and character sprites!

The system automatically falls back to colored rectangles if tileset files are not present, so the game will work either way.

## Expected Tileset Files

Place your tileset PNG files in these locations:

### World Tilesets
- **`assets/tilesets/world_tileset.png`** - Main world tiles (grass, dirt, water, etc.)
  - Tile size: **16x16 pixels**
  - Layout: Grid of tiles
  
- **`assets/tilesets/dungeon_tileset.png`** - Dungeon tiles (walls, floors, doors)
  - Tile size: **16x16 pixels**
  
- **`assets/tilesets/farm_tileset.png`** - Farm-specific tiles (soil, crops, etc.)
  - Tile size: **16x16 pixels**

### Character Sprites
- **`assets/sprites/character_tileset.png`** - Player and NPC sprites
  - Tile size: **16x16 pixels** or **32x32 pixels**
  - Layout (recommended):
    - Row 0: Down-facing animation (frames 0-3)
    - Row 1: Up-facing animation (frames 4-7)
    - Row 2: Left-facing animation (frames 8-11)
    - Row 3: Right-facing animation (frames 12-15)

## Tile ID Mapping

The system maps semantic tile types to sprite sheet IDs:

### World Tiles (world_tileset.png)
```
ID 0  = Grass
ID 1  = Dirt
ID 2  = Tilled Soil
ID 3  = Water
ID 4  = Stone
ID 5  = Sand
ID 6  = Dungeon Floor
ID 7+ = Walls (auto-tiled variants)
ID 20 = Door
ID 30+= Crops (growth stages)
ID 40+= Decorations
```

### Character Tiles (character_tileset.png)
```
ID 0-3   = Down-facing (idle + walk frames)
ID 4-7   = Up-facing (idle + walk frames)
ID 8-11  = Left-facing (idle + walk frames)
ID 12-15 = Right-facing (idle + walk frames)
```

## How It Works

### Automatic Loading
When the game starts, it attempts to load all tileset files:

```cpp
SpriteSheetManager::Instance().LoadDefaultAssets(renderer);
```

### Graceful Fallback
If a tileset file is missing:
- The game logs a message
- Continues using colored rectangles as placeholders
- No crash or error!

### Usage in Code

**World tiles:**
```cpp
// In Map::Render()
SpriteSheet* worldTiles = SpriteSheetManager::Instance().GetSpriteSheet("world_tiles");
if (worldTiles && worldTiles->IsLoaded()) {
    // Use actual sprites!
    worldTiles->RenderTile(renderer, tileId, x, y, width, height);
} else {
    // Fall back to colored rectangles
    RenderTileFallback(renderer, tile, x, y);
}
```

**Character sprites:**
```cpp
// In Player::Render()
SpriteSheet* charSheet = SpriteSheetManager::Instance().GetSpriteSheet("characters");
if (charSheet && charSheet->IsLoaded()) {
    charSheet->RenderTile(renderer, spriteId, x, y, width, height);
} else {
    RenderFallback(renderer);
}
```

## Creating Compatible Tilesets

### Requirements
1. **PNG format** with transparency
2. **Fixed tile size** (16x16 or 32x32)
3. **Grid layout** (no spacing between tiles)
4. **Power-of-2 dimensions recommended** (e.g., 256x256, 512x512)

### Example Tileset Structure

**world_tileset.png** (256x256, 16x16 tiles = 16x16 grid):
```
[0 ][1 ][2 ][3 ][4 ][5 ][6 ][7 ][8 ][9 ][10][11][12][13][14][15]
[16][17][18][19][20][21][22][23][24][25][26][27][28][29][30][31]
...
```

**character_tileset.png** (64x64, 16x16 tiles = 4x4 grid):
```
[Down0][Down1][Down2][Down3]  <- Walking down
[Up0  ][Up1  ][Up2  ][Up3  ]  <- Walking up
[Left0][Left1][Left2][Left3]  <- Walking left
[Right0][Right1][Right2][Right3] <- Walking right
```

## Testing Your Tilesets

1. **Place PNG files** in the correct directories
2. **Run the game**
3. **Check console output**:
   ```
   === Loading Sprite Sheets ===
   Loaded sprite sheet: assets/tilesets/world_tileset.png
     Size: 256x256
     Tiles: 16x16 (256 total)
     Tile size: 16x16
   ...
   ```
4. **Verify rendering** - You should see your sprites instead of colored squares!

## Troubleshooting

### "Failed to load sprite sheet"
- Check file path is correct
- Ensure PNG file exists
- Verify file permissions

### Sprites look wrong
- Check tile size matches (16x16 vs 32x32)
- Verify grid alignment (no spacing/padding)
- Ensure tile IDs map correctly

### Game still shows colored squares
- Check console for loading errors
- Verify files are in correct directories
- Make sure SDL2_image is installed

## Advanced: Custom Tilesets

You can add more tilesets:

```cpp
// In Game.cpp or custom loading code
SpriteSheetManager::Instance().LoadSpriteSheet(
    renderer, 
    "custom_name",          // Name to reference later
    "assets/path/file.png", // File path
    32,                     // Tile width
    32                      // Tile height
);

// Later, use it:
SpriteSheet* custom = SpriteSheetManager::Instance().GetSpriteSheet("custom_name");
```

## Where to Find Tilesets

### Free Resources
- **OpenGameArt.org** - Public domain tilesets
- **itch.io** - Many free and paid tilesets
- **Kenny.nl** - Free game assets
- **Liberated Pixel Cup** - Free tileset collection

### Create Your Own
- **Aseprite** - Pixel art editor
- **LibreSprite** - Free alternative
- **GIMP** - General image editing
- **Piskel** - Online pixel editor

## Current Status

✅ Sprite sheet system implemented
✅ Automatic loading on startup
✅ Graceful fallback if files missing
✅ Support for world and character tilesets
✅ Tile ID mapping system
⏳ Waiting for actual tileset PNG files
⏳ Animation system (future)

---

**Ready to use your tilesets!** Just drop the PNG files in the right directories and restart the game.
