#!/usr/bin/env python3
"""
Generate the world tileset for Harvest Quest.

Produces assets/tilesets/world_tileset.png — a 512×128 pixel art PNG
containing 64 tiles (16 columns × 4 rows) at 32×32 pixels each.

Style: Stardew Valley / Zelda: A Link to the Past
       Limited palette, no gradients, no outlines, clean silhouettes.

Tile ID layout (matches Map::GetTileSpriteId in src/world/Map.cpp):

  Row 0 | 0  Grass     1  Dirt      2  Soil      3  Water(f0)
        | 4  Stone     5  Sand      6  Floor     7–15 Wall variants
  Row 1 | 16–19 Walls  20 Door     21–23 Water(f1–f3)
        | 24 PathH    25 PathV     26–29 Path corners
        | 30 Crop0    31 Crop1
  Row 2 | 32–34 Crop2–4  35–38 Path T-junctions  39 Crossroads
        | 40 Bush     41 Rock      42 TallGrass
        | 43 Flower1  44 Flower2   45 FenceH  46 FenceV  47 Stump
  Row 3 | 48–49 (grass)  50 Tree   51 Trunk  52 Canopy  53–63 (grass)

Usage:
    python3 tools/generate_tileset.py          # writes assets/tilesets/world_tileset.png
    python3 tools/generate_tileset.py out.png   # writes to custom path

Requires: Pillow  (pip install Pillow)
"""

import os
import random
import sys
from PIL import Image

# ---------------------------------------------------------------------------
# Palette – 44 colours total, SNES-inspired
# ---------------------------------------------------------------------------

# Greens (grass / foliage)
GREEN_LIGHT  = (125, 177, 68)
GREEN_MID    = (91, 139, 49)
GREEN_DARK   = (62, 105, 36)

# Browns (dirt)
BROWN_LIGHT  = (196, 164, 101)
BROWN_MID    = (156, 121, 69)
BROWN_DARK   = (107, 80, 48)

# Soil (tilled)
SOIL_LIGHT   = (130, 95, 60)
SOIL_MID     = (100, 70, 42)
SOIL_DARK    = (75, 50, 28)

# Water
WATER_LIGHT  = (107, 190, 255)
WATER_MID    = (74, 139, 204)
WATER_DARK   = (45, 94, 138)
WATER_FOAM   = (170, 220, 255)

# Stone / grey
STONE_LIGHT  = (176, 176, 184)
STONE_MID    = (138, 138, 148)
STONE_DARK   = (100, 100, 112)

# Sand
SAND_LIGHT   = (232, 216, 160)
SAND_MID     = (204, 188, 132)
SAND_DARK    = (168, 152, 104)

# Floor (dungeon)
FLOOR_LIGHT  = (140, 130, 120)
FLOOR_MID    = (112, 104, 96)
FLOOR_DARK   = (88, 80, 74)

# Wall
WALL_LIGHT   = (110, 108, 120)
WALL_MID     = (82, 80, 92)
WALL_DARK    = (58, 56, 66)
WALL_DEEP    = (40, 38, 48)

# Door
DOOR_LIGHT   = (180, 130, 70)
DOOR_MID     = (140, 95, 45)
DOOR_DARK    = (100, 65, 28)

# Path
PATH_LIGHT   = (190, 170, 130)
PATH_MID     = (160, 140, 105)
PATH_DARK    = (130, 112, 82)

# Crop accents
CROP_SPROUT  = (80, 160, 50)
CROP_GROW    = (60, 140, 35)
CROP_MATURE  = (50, 120, 30)
CROP_FRUIT   = (220, 60, 50)
CROP_YELLOW  = (230, 200, 50)

# Tree
TRUNK_LIGHT  = (130, 90, 50)
TRUNK_MID    = (100, 65, 35)
TRUNK_DARK   = (72, 46, 22)
CANOPY_LIGHT = (80, 155, 48)
CANOPY_MID   = (55, 120, 35)
CANOPY_DARK  = (38, 88, 22)

# Flower centre
FLOWER_ORANGE = (200, 120, 40)

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

TILE = 32
COLS = 16
ROWS = 4
IMG_W = COLS * TILE   # 512
IMG_H = ROWS * TILE   # 128

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def tile_pos(tile_id):
    col = tile_id % COLS
    row = tile_id // COLS
    return col * TILE, row * TILE

def px(img, tx, ty, lx, ly, color):
    if 0 <= lx < TILE and 0 <= ly < TILE:
        img.putpixel((tx + lx, ty + ly), color)

def fill_tile(img, tid, color):
    tx, ty = tile_pos(tid)
    for y in range(TILE):
        for x in range(TILE):
            img.putpixel((tx + x, ty + y), color)

def fill_rect(img, tx, ty, x, y, w, h, color):
    for dy in range(h):
        for dx in range(w):
            px(img, tx, ty, x + dx, y + dy, color)

# ---------------------------------------------------------------------------
# Terrain tiles
# ---------------------------------------------------------------------------

def draw_grass(img, tid):
    tx, ty = tile_pos(tid)
    random.seed(42)
    for y in range(TILE):
        for x in range(TILE):
            if (x + y) % 4 == 0:
                c = GREEN_LIGHT
            elif (x * 3 + y * 7) % 5 == 0:
                c = GREEN_DARK
            elif (x + y) % 2 == 0:
                c = GREEN_MID
            else:
                c = GREEN_MID
            img.putpixel((tx + x, ty + y), c)
    blades = [(4,6),(12,3),(20,8),(28,5),(7,18),(15,22),
              (23,16),(2,28),(10,25),(18,30),(26,20),(30,12),
              (8,12),(16,14),(24,26),(6,24)]
    for bx, by in blades:
        if bx < TILE and by < TILE:
            px(img, tx, ty, bx, by, GREEN_LIGHT)
            if by > 0:
                px(img, tx, ty, bx, by - 1, GREEN_LIGHT)

def draw_dirt(img, tid):
    tx, ty = tile_pos(tid)
    random.seed(101)
    for y in range(TILE):
        for x in range(TILE):
            if (x * 5 + y * 3) % 7 == 0:
                c = BROWN_LIGHT
            elif (x + y * 2) % 5 == 0:
                c = BROWN_DARK
            else:
                c = BROWN_MID
            img.putpixel((tx + x, ty + y), c)
    for px_, py_ in [(6,10),(18,6),(26,22),(10,26),(22,14)]:
        px(img, tx, ty, px_, py_, BROWN_LIGHT)
        px(img, tx, ty, px_ + 1, py_, BROWN_LIGHT)

def draw_tilled_soil(img, tid):
    tx, ty = tile_pos(tid)
    for y in range(TILE):
        for x in range(TILE):
            phase = y % 8
            if phase < 3:
                c = SOIL_DARK
            elif phase < 5:
                c = SOIL_MID
            else:
                c = SOIL_LIGHT
            if (x + y) % 6 == 0:
                c = SOIL_DARK
            img.putpixel((tx + x, ty + y), c)

def draw_water(img, tid, frame=0):
    tx, ty = tile_pos(tid)
    random.seed(200 + frame)
    offset = frame * 3
    for y in range(TILE):
        for x in range(TILE):
            wave = ((x + offset) + (y * 2)) % 12
            if wave < 3:
                c = WATER_LIGHT
            elif wave < 6:
                c = WATER_MID
            elif wave < 9:
                c = WATER_DARK
            else:
                c = WATER_MID
            img.putpixel((tx + x, ty + y), c)
    foam = [(4 + frame*2,8),(16 - frame,4),(24 + frame,20),
            (8 - frame,24),(20 + frame,12),(28 - frame*2,28),
            (12 + frame,16),(2,14 + frame)]
    for fx, fy in foam:
        fx %= TILE; fy %= TILE
        px(img, tx, ty, fx, fy, WATER_FOAM)
        if fx + 1 < TILE:
            px(img, tx, ty, fx + 1, fy, WATER_LIGHT)

def draw_stone(img, tid):
    tx, ty = tile_pos(tid)
    random.seed(300)
    for y in range(TILE):
        for x in range(TILE):
            shifted = (x + (8 if (y // 16) % 2 == 1 else 0)) % 16
            by = y % 16
            if shifted == 0 or by == 0:
                c = STONE_DARK
            elif (shifted + by) % 7 == 0:
                c = STONE_LIGHT
            else:
                c = STONE_MID
            img.putpixel((tx + x, ty + y), c)

def draw_sand(img, tid):
    tx, ty = tile_pos(tid)
    random.seed(400)
    for y in range(TILE):
        for x in range(TILE):
            if (x * 3 + y * 7) % 11 == 0:
                c = SAND_DARK
            elif (x + y) % 3 == 0:
                c = SAND_LIGHT
            else:
                c = SAND_MID
            img.putpixel((tx + x, ty + y), c)

def draw_floor(img, tid):
    tx, ty = tile_pos(tid)
    for y in range(TILE):
        for x in range(TILE):
            by = y % 8
            shifted = (x + (8 if (y // 8) % 2 == 1 else 0)) % 16
            if shifted == 0 or by == 0:
                c = FLOOR_DARK
            elif (shifted + by) % 5 == 0:
                c = FLOOR_LIGHT
            else:
                c = FLOOR_MID
            img.putpixel((tx + x, ty + y), c)

# ---------------------------------------------------------------------------
# Wall tiles (auto-tiling variants)
# ---------------------------------------------------------------------------

def _wall_base(img, tid):
    tx, ty = tile_pos(tid)
    for y in range(TILE):
        for x in range(TILE):
            by = y % 8
            shifted = (x + (4 if (y // 8) % 2 == 1 else 0)) % 8
            if shifted == 0 or by == 0:
                c = WALL_DEEP
            elif (shifted + by) % 3 == 0:
                c = WALL_LIGHT
            else:
                c = WALL_MID
            img.putpixel((tx + x, ty + y), c)

def draw_wall(img, tid, top=True, bot=True, left=True, right=True):
    _wall_base(img, tid)
    tx, ty = tile_pos(tid)
    edge = 3
    if not top:
        for x in range(TILE):
            for ey in range(edge):
                px(img, tx, ty, x, ey, WALL_DARK)
    if not bot:
        for x in range(TILE):
            for ey in range(edge):
                px(img, tx, ty, x, TILE - 1 - ey, WALL_DEEP)
    if not left:
        for y in range(TILE):
            for ex in range(edge):
                px(img, tx, ty, ex, y, WALL_DARK)
    if not right:
        for y in range(TILE):
            for ex in range(edge):
                px(img, tx, ty, TILE - 1 - ex, y, WALL_DEEP)

def draw_all_walls(img):
    draw_wall(img,  7, True,  True,  True,  True)   # centre
    draw_wall(img,  8, False, True,  True,  True)   # N edge
    draw_wall(img,  9, True,  False, True,  True)   # S edge
    draw_wall(img, 10, True,  True,  True,  False)  # E edge
    draw_wall(img, 11, True,  True,  False, True)   # W edge
    draw_wall(img, 12, False, True,  True,  False)  # NE corner
    draw_wall(img, 13, False, True,  False, True)   # NW corner
    draw_wall(img, 14, True,  False, True,  False)  # SE corner
    draw_wall(img, 15, True,  False, False, True)   # SW corner
    draw_wall(img, 16, False, False, True,  True)   # horizontal
    draw_wall(img, 17, True,  True,  False, False)  # vertical
    draw_wall(img, 18, False, True,  False, False)  # end N
    draw_wall(img, 19, True,  False, False, False)  # end S

# ---------------------------------------------------------------------------
# Door
# ---------------------------------------------------------------------------

def draw_door(img, tid):
    draw_floor(img, tid)
    tx, ty = tile_pos(tid)
    fill_rect(img, tx, ty, 4, 0, 24, 28, DOOR_MID)
    fill_rect(img, tx, ty, 6, 2, 20, 24, DOOR_LIGHT)
    fill_rect(img, tx, ty, 8, 4, 7, 10, DOOR_MID)
    fill_rect(img, tx, ty, 17, 4, 7, 10, DOOR_MID)
    fill_rect(img, tx, ty, 8, 16, 7, 8, DOOR_MID)
    fill_rect(img, tx, ty, 17, 16, 7, 8, DOOR_MID)
    px(img, tx, ty, 14, 14, BROWN_LIGHT)
    px(img, tx, ty, 15, 14, BROWN_LIGHT)
    fill_rect(img, tx, ty, 4, 28, 24, 4, DOOR_DARK)

# ---------------------------------------------------------------------------
# Path tiles
# ---------------------------------------------------------------------------

def _path_base(img, tid):
    tx, ty = tile_pos(tid)
    for y in range(TILE):
        for x in range(TILE):
            if (x + y * 3) % 7 == 0:
                c = PATH_DARK
            elif (x * 2 + y) % 5 == 0:
                c = PATH_LIGHT
            else:
                c = PATH_MID
            img.putpixel((tx + x, ty + y), c)

def draw_path(img, tid, tg=False, bg=False, lg=False, rg=False):
    """Draw path tile.  tg/bg/lg/rg = grass on top/bottom/left/right."""
    _path_base(img, tid)
    tx, ty = tile_pos(tid)
    edge = 5
    def _grass_edge(x, y):
        c = GREEN_MID if (x + y) % 2 == 0 else GREEN_DARK
        return c
    if tg:
        for x in range(TILE):
            for ey in range(edge):
                if (x + ey) % 3 == 0 or ey < 2:
                    px(img, tx, ty, x, ey, _grass_edge(x, ey))
    if bg:
        for x in range(TILE):
            for ey in range(edge):
                if (x + ey) % 3 == 0 or ey < 2:
                    px(img, tx, ty, x, TILE - 1 - ey, _grass_edge(x, ey))
    if lg:
        for y in range(TILE):
            for ex in range(edge):
                if (y + ex) % 3 == 0 or ex < 2:
                    px(img, tx, ty, ex, y, _grass_edge(y, ex))
    if rg:
        for y in range(TILE):
            for ex in range(edge):
                if (y + ex) % 3 == 0 or ex < 2:
                    px(img, tx, ty, TILE - 1 - ex, y, _grass_edge(y, ex))

def draw_all_paths(img):
    draw_path(img, 24, tg=True,  bg=True)                 # straight H
    draw_path(img, 25, lg=True,  rg=True)                  # straight V
    draw_path(img, 26, tg=True,  rg=True)                  # corner NE
    draw_path(img, 27, tg=True,  lg=True)                  # corner NW
    draw_path(img, 28, bg=True,  rg=True)                  # corner SE
    draw_path(img, 29, bg=True,  lg=True)                  # corner SW
    draw_path(img, 35, tg=True)                             # T-junction N
    draw_path(img, 36, bg=True)                             # T-junction S
    draw_path(img, 37, rg=True)                             # T-junction E
    draw_path(img, 38, lg=True)                             # T-junction W
    _path_base(img, 39)                                     # crossroads

# ---------------------------------------------------------------------------
# Crop growth stages (drawn on tilled soil base)
# ---------------------------------------------------------------------------

def draw_crop(img, tid, stage):
    draw_tilled_soil(img, tid)
    tx, ty = tile_pos(tid)
    if stage == 0:
        for dx, dy in [(14,14),(18,16),(12,18),(20,14)]:
            px(img, tx, ty, dx, dy, BROWN_DARK)
    elif stage == 1:
        for sx, sy in [(12,16),(16,14),(20,16)]:
            px(img, tx, ty, sx, sy, CROP_SPROUT)
            px(img, tx, ty, sx, sy - 1, CROP_SPROUT)
            px(img, tx, ty, sx, sy - 2, GREEN_LIGHT)
    elif stage == 2:
        for ppx, ppy in [(10,16),(16,14),(22,16)]:
            for dy in range(-4, 1):
                px(img, tx, ty, ppx, ppy + dy, CROP_GROW)
            px(img, tx, ty, ppx - 1, ppy - 3, GREEN_LIGHT)
            px(img, tx, ty, ppx + 1, ppy - 3, GREEN_LIGHT)
            px(img, tx, ty, ppx, ppy - 5, GREEN_LIGHT)
    elif stage == 3:
        for ppx, ppy in [(10,18),(16,16),(22,18)]:
            for dy in range(-7, 1):
                px(img, tx, ty, ppx, ppy + dy, CROP_MATURE)
            px(img, tx, ty, ppx - 2, ppy - 4, GREEN_MID)
            px(img, tx, ty, ppx - 1, ppy - 5, GREEN_MID)
            px(img, tx, ty, ppx + 1, ppy - 5, GREEN_MID)
            px(img, tx, ty, ppx + 2, ppy - 4, GREEN_MID)
            px(img, tx, ty, ppx,     ppy - 6, GREEN_LIGHT)
            px(img, tx, ty, ppx - 1, ppy - 6, GREEN_LIGHT)
            px(img, tx, ty, ppx + 1, ppy - 6, GREEN_LIGHT)
    elif stage == 4:
        for ppx, ppy in [(10,18),(16,16),(22,18)]:
            for dy in range(-7, 1):
                px(img, tx, ty, ppx, ppy + dy, CROP_MATURE)
            px(img, tx, ty, ppx - 2, ppy - 4, GREEN_MID)
            px(img, tx, ty, ppx - 1, ppy - 5, GREEN_MID)
            px(img, tx, ty, ppx + 1, ppy - 5, GREEN_MID)
            px(img, tx, ty, ppx + 2, ppy - 4, GREEN_MID)
            px(img, tx, ty, ppx,     ppy - 7, GREEN_LIGHT)
            px(img, tx, ty, ppx - 1, ppy - 3, CROP_FRUIT)
            px(img, tx, ty, ppx,     ppy - 3, CROP_FRUIT)
            px(img, tx, ty, ppx + 1, ppy - 2, CROP_YELLOW)

# ---------------------------------------------------------------------------
# Decoration tiles (drawn on grass base)
# ---------------------------------------------------------------------------

def draw_bush(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    cx, cy = 16, 18
    for y in range(TILE):
        for x in range(TILE):
            dx = x - cx; dy = y - cy
            dist = (dx * dx) / 100 + (dy * dy) / 64
            if dist < 1.0:
                if dist < 0.3:
                    c = GREEN_LIGHT
                elif dist < 0.6:
                    c = GREEN_MID
                else:
                    c = GREEN_DARK
                px(img, tx, ty, x, y, c)
    px(img, tx, ty, 13, 14, GREEN_LIGHT)
    px(img, tx, ty, 18, 13, GREEN_LIGHT)

def draw_rock(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    cx, cy = 16, 20
    for y in range(TILE):
        for x in range(TILE):
            dx = x - cx; dy = (y - cy) * 1.4
            dist = dx * dx + dy * dy
            if dist < 80:
                if dist < 25:
                    c = STONE_LIGHT
                elif dist < 50:
                    c = STONE_MID
                else:
                    c = STONE_DARK
                px(img, tx, ty, x, y, c)
    px(img, tx, ty, 13, 17, STONE_LIGHT)
    px(img, tx, ty, 14, 17, STONE_LIGHT)

def draw_tall_grass(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    blades = [(6,26),(10,24),(14,26),(18,24),(22,26),(26,24),
              (8,25),(16,25),(24,25),(12,27),(20,27)]
    for bx, by in blades:
        for h in range(8):
            sway = 1 if h > 4 and bx % 3 == 0 else 0
            c = GREEN_LIGHT if h < 3 else GREEN_MID if h < 6 else GREEN_DARK
            px(img, tx, ty, bx + sway, by - h, c)
            if h < 5:
                px(img, tx, ty, bx + sway + 1, by - h, c)

def draw_flower_1(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    for fx, fy in [(10,20),(20,16),(14,26)]:
        for h in range(5):
            px(img, tx, ty, fx, fy + 1 + h, GREEN_DARK)
        for dx, dy in [(-1,0),(1,0),(0,-1),(0,1)]:
            px(img, tx, ty, fx + dx, fy + dy, CROP_FRUIT)
        px(img, tx, ty, fx, fy, CROP_YELLOW)

def draw_flower_2(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    for fx, fy in [(8,18),(22,22),(16,14)]:
        for h in range(4):
            px(img, tx, ty, fx, fy + 1 + h, GREEN_DARK)
        for dx, dy in [(-1,0),(1,0),(0,-1)]:
            px(img, tx, ty, fx + dx, fy + dy, CROP_YELLOW)
        px(img, tx, ty, fx, fy, FLOWER_ORANGE)

def draw_fence_h(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    fill_rect(img, tx, ty, 0, 14, 32, 2, TRUNK_MID)
    fill_rect(img, tx, ty, 0, 18, 32, 2, TRUNK_MID)
    fill_rect(img, tx, ty, 2, 10, 3, 14, TRUNK_DARK)
    fill_rect(img, tx, ty, 27, 10, 3, 14, TRUNK_DARK)
    fill_rect(img, tx, ty, 2, 10, 3, 2, TRUNK_LIGHT)
    fill_rect(img, tx, ty, 27, 10, 3, 2, TRUNK_LIGHT)

def draw_fence_v(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    fill_rect(img, tx, ty, 14, 0, 2, 32, TRUNK_MID)
    fill_rect(img, tx, ty, 18, 0, 2, 32, TRUNK_MID)
    fill_rect(img, tx, ty, 12, 2, 8, 3, TRUNK_DARK)
    fill_rect(img, tx, ty, 12, 27, 8, 3, TRUNK_DARK)
    fill_rect(img, tx, ty, 12, 2, 8, 1, TRUNK_LIGHT)
    fill_rect(img, tx, ty, 12, 27, 8, 1, TRUNK_LIGHT)

def draw_stump(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    fill_rect(img, tx, ty, 10, 16, 12, 10, TRUNK_DARK)
    fill_rect(img, tx, ty, 11, 17, 10, 8, TRUNK_MID)
    fill_rect(img, tx, ty, 9, 14, 14, 4, TRUNK_LIGHT)
    fill_rect(img, tx, ty, 10, 15, 12, 2, BROWN_LIGHT)
    px(img, tx, ty, 14, 15, TRUNK_MID)
    px(img, tx, ty, 16, 16, TRUNK_MID)

# ---------------------------------------------------------------------------
# Tree tiles (full / trunk / canopy)
# ---------------------------------------------------------------------------

def draw_tree_full(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    fill_rect(img, tx, ty, 13, 20, 6, 12, TRUNK_DARK)
    fill_rect(img, tx, ty, 14, 20, 4, 12, TRUNK_MID)
    fill_rect(img, tx, ty, 15, 21, 2, 10, TRUNK_LIGHT)
    cx, cy = 16, 12
    for y in range(TILE):
        for x in range(TILE):
            dx = x - cx; dy = (y - cy) * 1.1
            dist = dx * dx + dy * dy
            if dist < 120 and y < 22:
                if dist < 40:
                    c = CANOPY_LIGHT
                elif dist < 75:
                    c = CANOPY_MID
                else:
                    c = CANOPY_DARK
                px(img, tx, ty, x, y, c)
    for hx, hy in [(12,8),(18,6),(14,10),(20,10),(16,5)]:
        px(img, tx, ty, hx, hy, CANOPY_LIGHT)

def draw_tree_trunk(img, tid):
    draw_grass(img, tid)
    tx, ty = tile_pos(tid)
    fill_rect(img, tx, ty, 11, 0, 10, 32, TRUNK_DARK)
    fill_rect(img, tx, ty, 12, 0, 8, 32, TRUNK_MID)
    fill_rect(img, tx, ty, 14, 0, 4, 32, TRUNK_LIGHT)
    fill_rect(img, tx, ty, 8, 26, 4, 6, TRUNK_DARK)
    fill_rect(img, tx, ty, 20, 26, 4, 6, TRUNK_DARK)
    fill_rect(img, tx, ty, 9, 27, 3, 5, TRUNK_MID)
    fill_rect(img, tx, ty, 21, 27, 3, 5, TRUNK_MID)
    for bx, by in [(13,8),(17,14),(14,20),(18,4),(15,26)]:
        px(img, tx, ty, bx, by, TRUNK_DARK)
        px(img, tx, ty, bx, by + 1, TRUNK_DARK)

def draw_tree_canopy(img, tid):
    tx, ty = tile_pos(tid)
    for y in range(TILE):
        for x in range(TILE):
            img.putpixel((tx + x, ty + y), GREEN_MID)
    cx, cy = 16, 16
    for y in range(TILE):
        for x in range(TILE):
            dx = x - cx; dy = (y - cy) * 0.85
            dist = dx * dx + dy * dy
            if dist < 200:
                if dist < 50:
                    c = CANOPY_LIGHT
                elif dist < 110:
                    c = CANOPY_MID
                else:
                    c = CANOPY_DARK
                px(img, tx, ty, x, y, c)
    for hx, hy in [(10,10),(20,8),(14,14),(22,14),(16,6),
                    (8,16),(24,12),(12,20),(20,20)]:
        px(img, tx, ty, hx, hy, CANOPY_LIGHT)
        if hx + 1 < TILE:
            px(img, tx, ty, hx + 1, hy, CANOPY_LIGHT)

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def generate_tileset():
    img = Image.new('RGB', (IMG_W, IMG_H), GREEN_MID)

    # --- terrain (row 0, IDs 0–6) ---
    draw_grass(img, 0)
    draw_dirt(img, 1)
    draw_tilled_soil(img, 2)
    draw_water(img, 3, frame=0)
    draw_stone(img, 4)
    draw_sand(img, 5)
    draw_floor(img, 6)

    # --- walls (IDs 7–19) ---
    draw_all_walls(img)

    # --- door (ID 20) ---
    draw_door(img, 20)

    # --- water animation frames (IDs 21–23) ---
    draw_water(img, 21, frame=1)
    draw_water(img, 22, frame=2)
    draw_water(img, 23, frame=3)

    # --- path tiles (IDs 24–29, 35–39) ---
    draw_all_paths(img)

    # --- crop growth stages (IDs 30–34) ---
    for s in range(5):
        draw_crop(img, 30 + s, s)

    # --- decorations (IDs 40–47) ---
    draw_bush(img, 40)
    draw_rock(img, 41)
    draw_tall_grass(img, 42)
    draw_flower_1(img, 43)
    draw_flower_2(img, 44)
    draw_fence_h(img, 45)
    draw_fence_v(img, 46)
    draw_stump(img, 47)

    # --- trees (IDs 50–52) ---
    draw_tree_full(img, 50)
    draw_tree_trunk(img, 51)
    draw_tree_canopy(img, 52)

    # --- fill unused slots with grass ---
    used = set(list(range(0, 20)) + list(range(20, 30))
               + list(range(30, 35)) + list(range(35, 40))
               + list(range(40, 48)) + [50, 51, 52])
    for tid in range(COLS * ROWS):
        if tid not in used:
            draw_grass(img, tid)

    return img


if __name__ == '__main__':
    out_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
        'assets', 'tilesets', 'world_tileset.png')
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    img = generate_tileset()
    img.save(out_path)
    print(f"Tileset saved to: {out_path}")
    print(f"  Size : {img.size[0]}×{img.size[1]} px")
    print(f"  Grid : {COLS}×{ROWS} tiles  ({COLS * ROWS} total)")
    print(f"  Tile : {TILE}×{TILE} px")
