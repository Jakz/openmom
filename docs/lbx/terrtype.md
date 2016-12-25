# TERRTYPE.LBX

This file contains a mapping from adjacency direction mask to specific tile in `TERRAIN.LBX`.

The file has 2 arrays of `256 x u16` (`512` bytes):

1. first entry starts at offset `0x226` and contains mapping for shores, desert and tundra files.
2. second entry starts at offset `0x426` and contains mapping for hills and mountains.

The original games computes a 8 adjacency mask which goes clockwise and it's 1 if the tile has the neighbour of the specified type. Graphically it works like the following:

    1      2     4
    
    128    T     8
    
    64    32    16

So for example a shore which has terrain on the 3 south neighbours will have a mask of `64 | 32 | 16 = 112`. So `array1[112]` of the LBX file will contain the tile index for that specific graphics. To make it work for desert and tundra we must take into account the offset, for example to obtain a tundra tile we must subtract `2` and then add `0x25A` which is the first tile of tundra graphics.

The second array works in a similar fashion but for and mountains and hills, here the adjacency matrix doesn't mean to have grasslands but to have mountains on that neighbour tile.
