# Intro
This LBX contains static palettes for the game and all the fonts graphical daa.

##Fonts

There are 8 fonts defined in the lbx file, each font has 94 glyphs ordered in the same way of standard ASCII table starting from character `' '` (value `0x20` or `32`).

###Glyph Data
Glyph data is stored in first entry of the lbx starting at offset `0x320`. The structure of the entry is the following. Two notes:

* the offset is relative to the start of the entry
* there are some padding/uknown data between the elements, still need to be reversed

Offset | Length | Structure | Name | Description
:---: | :---: | :---: | :---: | :---: 
`0x16A` | `0x200` (`32`) | 8 x `u16` | Font Height | Glyph height for each font (one per font)
`0x19A` | `0x300` (`768`) | 8 x 96 x `u8` | Font Width | Glyph width for each font (96 bytes per font, 2 last bytes are always 0)


