# Harvest Quest — Game Data

This directory contains game content data files for the Harvest Quest project.

## Structure

```
data/
├── crops.json          # Crop definitions and growth data
├── recipes.json        # Crafting recipes
├── npcs.json           # NPC definitions and schedules
├── enemies.json        # Enemy types and behavior
└── items.json          # Item definitions
```

## Format

All data files follow JSON format and are validated against schemas
in the `schemas/` directory when available.

See the [Atlas Forge project documentation](https://github.com/shifty81/AtlasForge)
for data file conventions.
