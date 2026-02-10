#!/bin/bash
# Script to detect and setup PNG files from root directory

echo "=== PNG File Detection and Setup ==="
echo ""

# Check for PNG files in root
ROOT_PNGS=$(ls -1 *.png 2>/dev/null | wc -l)

if [ $ROOT_PNGS -eq 0 ]; then
    echo "No PNG files found in root directory."
    echo "Expected files:"
    echo "  - world_tileset.png or tiles.png (16x16 world tiles)"
    echo "  - character_tileset.png or characters.png (character sprites)"
    exit 0
fi

echo "Found $ROOT_PNGS PNG file(s) in root directory:"
ls -lh *.png 2>/dev/null

echo ""
echo "Setting up tilesets..."

# Create asset directories if they don't exist
mkdir -p assets/tilesets
mkdir -p assets/sprites

# Process each PNG file
for png in *.png; do
    echo ""
    echo "Processing: $png"
    
    # Determine destination based on filename
    if [[ $png == *"world"* ]] || [[ $png == *"tile"* ]] || [[ $png == *"map"* ]]; then
        # World tileset
        DEST="assets/tilesets/world_tileset.png"
        echo "  → Copying to $DEST (world tileset)"
        cp "$png" "$DEST"
    elif [[ $png == *"char"* ]] || [[ $png == *"sprite"* ]] || [[ $png == *"player"* ]]; then
        # Character sprites
        DEST="assets/sprites/character_tileset.png"
        echo "  → Copying to $DEST (character sprites)"
        cp "$png" "$DEST"
    else
        # Ask user or use first as world, second as character
        COUNT=$(ls -1 assets/tilesets/*.png assets/sprites/*.png 2>/dev/null | wc -l)
        if [ $COUNT -eq 0 ]; then
            DEST="assets/tilesets/world_tileset.png"
            echo "  → Copying to $DEST (first PNG = world tileset)"
            cp "$png" "$DEST"
        else
            DEST="assets/sprites/character_tileset.png"
            echo "  → Copying to $DEST (second PNG = character sprites)"
            cp "$png" "$DEST"
        fi
    fi
done

echo ""
echo "=== Setup Complete ==="
echo "Asset files:"
ls -lh assets/tilesets/*.png assets/sprites/*.png 2>/dev/null
echo ""
echo "Run 'cmake --build build' to rebuild with new assets."
