# World Generation System

## Philosophy: Tiles Are Dumb, Data Is Smart

**Never place tiles randomly.** Generate high-level structure first, then paint tiles based on semantic meaning.

## Pipeline

```
Seed → Biome → Logical Layout → Collision/Rules → Tile Painting → Decoration
```

## Key Components

### 1. Semantic Tile Types
- VOID, FLOOR, WALL, DOOR, WATER, SOIL, CROP, GRASS, etc.
- Each type has rules: solid, breakable, farmable

### 2. Tile Definitions
```cpp
TileDef { type, solid, breakable, farmable, name }
```

### 3. Dungeon Generation (Zelda-Style)
1. Generate rooms (logical)
2. Connect with corridors  
3. Carve FLOOR/WALL tiles
4. Auto-tile for visuals
5. Add combat locks

### 4. Farm Generation (Stardew-Style)
1. Define zones (soil, water, paths)
2. Soil state machine: GRASS → HOE → WATERED → CROP → HARVEST
3. Crop growth system

### 5. Auto-Tiling
Check 8 neighbors to select proper visual tile:
- Corners (inner/outer)
- Edges (top/bottom/left/right)
- Solid centers

## Usage

```cpp
WorldGenerator gen(seed);
gen.GenerateFarm(map, w, h);       // Farm
gen.GenerateDungeon(map, w, h);    // Dungeon
gen.GenerateOverworld(map, w, h);  // Overworld
```

## Try It!

Run the game and press:
- **1** - Generate Farm
- **2** - Generate Dungeon  
- **3** - Generate Overworld

Watch procedural generation in action!
