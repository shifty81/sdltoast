# 🎉 Quick Start: Using Your PNG Tilesets

## You Asked: "Can we use 2 PNGs in the root?"

## Answer: YES! Just Drop Them Here! ✅

### Step 1: Place Your PNG Files
```
sdltoast/
├── your_tiles.png        ← Your world tileset (grass, dirt, walls, etc.)
├── your_sprites.png      ← Your character sprites
├── src/
├── assets/
└── ...
```

### Step 2: Run the Game
```bash
./build.sh
# or manually:
mkdir build && cd build && cmake .. && cmake --build . && ./HarvestQuest
```

### Step 3: Done! 🎮
The game automatically finds and uses your PNG files!

---

## Naming Your Files (Optional)

For best recognition, use these names:

**World Tiles:**
- `world_tileset.png` ✓
- `tiles.png` ✓
- `tileset.png` ✓
- Or any name (will use first PNG alphabetically)

**Character Sprites:**
- `character_tileset.png` ✓
- `characters.png` ✓
- `sprites.png` ✓
- Or any name (will use second PNG alphabetically)

## File Format

**World Tileset:**
- 16×16 pixel tiles
- PNG with transparency
- Grid layout (e.g., 256×256 = 16×16 tiles)

**Character Sprites:**
- 16×16 pixel sprites
- PNG with transparency
- 4 rows for 4 directions (down, up, left, right)

## More Info

- [Complete Usage Guide](docs/ROOT_PNG_USAGE.md)
- [Tileset Format Details](docs/TILESET_INTEGRATION.md)
- [Testing Guide](docs/ROOT_PNG_TESTING.md)

## Fallback

No PNG files? No problem!
- Game shows colored rectangles
- All features work normally
- Procedural generation still works

---

**TL;DR:** Drop 2 PNG files here → Run game → See your graphics! 🚀
