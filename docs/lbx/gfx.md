# Graphical Sprites

Sprites that are stored in LBX files support multiple features:

* varying size
* optional override of a part of standard palette
* optional multiple frames
* RLE encoding between frames to save memory

## General Structure

The whole structure of a _sprite_ is the following, mind that the offsets are used to determine and compute the correct position inside the binary file for the data which could be contiguous as described in the table below but it is not required. This means that to read the sprite data you shouldn't never assume that data is contiguous but always compute the correct offset, the _palette header_ for example could be stored anywhere so the correct way is to add `spriteHeader.paletteOffset` to the starting offset of the frame.


We assume that `n` is the amount of frames of the sprite (stored in the header). We assume that `c` is the length of the custom palette if present.

  Size | Count | Name | Description 
  :---: | :---: | :---: | :--- 
 18 | 1 | Sprite Header | This header contains main informations of the sprite
 `u32` | n+1 | Frame Offsets | Offsets to each frame of the sprite, last offset is a past the end marker
 8 | 1 | Palette Header | This header is optional and is present only if palette offset of sprite header is specified
 3 | c | Palette Colors | These are the custom colors for the sprite
 X | n | Frame Data | These are the data for each single frame of the sprite
 
## Sprite Header

The structure of the header is the following:

 Offset | Type | Description 
 :---:  | :---: | :--- 
 0x00 | `u16` | width of the sprite
 0x02 | `u16` | height of the sprite
 0x04 | `u16` | unknown (always 0)
 0x06 | `u16` | frame count
 0x08 | `u16` | frame loop restart
 0x0A | `u16` | unkown (always 0)
 0x0C | `u16` | unkown (always 0)
 0x0E | `u16` | palette offset (0 if no custom palette)
 0x10 | `u8` | unknown
 0x11 | `u8` | unknown
 | __18__ | | __total bytes__
 
Everything is self-explanatory just some pointings:

* _frame loop restart_ is used for animations composed by multiple frames to indicate where should the animation loop after first complete cycle, for example if sprite has 5 frames and frame loop restart if 2 then progression should be: _0 1 2 3 4 2 3 4 2 3 4_.
* last two unknown bytes are actually used for something but it's not clear for what so in case someone reverses their meaning please enlighten me.

## Palette Header

The palette header specifies the custom palette information for a sprite. This header is not always present as not all sprites have a custom palette. Its structure is the following:

Offset | Type | Description 
:---:  | :---: | :--- 
0x00 | `u16` | offset of palette colors
0x02 | `u16` | first index to override
0x04 | `u16` | count of colors to override
0x06 | `u16` | unknown

Each color found starting at specified offset is stored as 3 bytes RGB color in which each channel is stored as a value in `[0, 64)`. This means that you should multiply the value by 4 (or shift by 2) to obtain a standard RGB8 value.

So to generate a standard `u32` RGB color you would need to do something like

```
u32 color = (entry.r << 18) | (entry.g << 10) | entry.b << 2;
```

## Computing offsets

In the end, assuming

* `offset` is the starting offset of the sprite
* `frameOffsets`is the array of offsets to frame data
* `header` is the sprite header
* `paletteHeader` is the palette header

Then we have that 

* `frameOffsets` are stored at `offset + sizeof(SpriteHeader)`
* the palette header (if present) is stored at `offset + header.paletteOffset`
* palette colors (if present) are stored at `offset + header.paletteOffset + paletteHeader.offset`
* frame data for i-th frame is stored at offset + `frameOffsets[i]`
* frame data length for i-th frame is `frameOffsets[i+1] - frameOffsets[i]` (that's why there is an extra offset)