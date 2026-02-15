# Harvest Quest — Game Data

This directory contains game content data files for the Harvest Quest project.

## Structure

```
data/
├── crops.json          # Crop definitions and growth data
├── enemies.json        # Enemy types, stats, and loot
├── items.json          # Item definitions and categories
├── npcs.json           # NPC definitions and schedules
├── quests.json         # Quest definitions, objectives, and rewards
└── recipes.json        # Crafting recipes
```

## Format

All data files follow JSON format and are validated against schemas
in the `schemas/` directory when available.

See the [Atlas Forge project documentation](https://github.com/shifty81/AtlasForge)
for data file conventions.
