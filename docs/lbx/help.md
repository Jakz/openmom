# HELP.LBX

This file contains help data for the various things inside the game. It contains 3 entries.

First 2 entries are standard GFX format entries which contains the backdrop for help message. Last entry contains help data in the following format:

    char[30] title of the paragraph
    char[14] lbx file name (optional graphics, `'\0'` for no graphics)
    u16 lbx entry index (optional sprite index for graphics of paragraph)
    s16 next entry (0 if none, -1 if directly next in the array, any other value specifies the specific index of next entry)

Although the LBX header specifies like 1600 paragraphs in reality just 806 seems to be valid and subsequent one are just garbage.
