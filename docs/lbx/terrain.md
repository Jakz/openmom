# TERRAIN.LBX

File terrain.lbx has 3 subentries:

1. tile gfx data
2. tile mapping between tile index and gfx index (which specifies also if it's animated)
3. tile minimap color

These 3 subentries are start at the three offsets specified by the default LBX offsets header.
Game tile indices are 761 per plane, so 1524 different tiles.

# TILE GFX DATA

- starts at `0x240`
- there are `192` bytes of padding between start and first tile
- there are `1761` tiles

## Each tile has this structure

    1 u16 tile width (always 20)
    1 u16 tile height (always 18)
    1 u32 unknown (always 0x0000DE0A)
    8 byte padding (always 0)
    20x18 byte tile data (index in standard palette)
    8 byte footer (always 0)

This yields a total of 384 bytes per tile.

# TILE MAPPING

The second entry of the file contains the mapping between game tile values and sprite indices. This is necessary because some tiles are animated so they have multiple sprites mapped to a single game value.

- starts at `0xA5480`
- 1524 entries `(761+1)*2`, there is an additional entry per plane which purpose is unknown

## Each entry has this structure

    1 byte - is animated / tile index multiplier
    1 byte - tile index base

Assume to have `u8 data[2]` for the entry, then gfx index is calculated as

    gfxIndex = ((data[0] & 0x7F) / 3 ) * 128 + data[1] - 2

So 7 bits from the first byte are used to calculate a multiplier which is added to second byte (we subtract 2 as it seems that there are 2 dummy tile entries at the begin of the sprite tile data).

Then a sprite is animated (and always has 4 successive frames in tile gfx data) if

    isAnimated = data[0] & 0x80

In the end the element at position `i` (so `0xA5480 + i*2`) in the entry specifies which gfx tile is mapped to it and if it's animated or not.

# TILE MINIMAP COLOR

Third entry is just an array of 1524 bytes which contain the index of the minimap color from the standard palette. This entry starts at `0xA6068`.

