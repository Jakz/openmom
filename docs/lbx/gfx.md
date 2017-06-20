# Graphical Sprites

Sprites that are stored in LBX files support multiple features:

* varying size
* optional override of a part of standard palette
* optional multiple frames
* RLE encoding between frames to save memory

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