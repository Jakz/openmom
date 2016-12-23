#include "Dialogs.h"

#include "Gfx.h"
#include "Font.h"
#include "LBX.h"

using namespace std;

void dialogs::drawErrorDialog(const std::string& text)
{
  constexpr u32 MESSAGE_WIDTH = 174;
  constexpr u32 DIALOG_WIDTH = 186;
  constexpr u32 DIALOG_X = 68;
  
  Gfx::resetBuffer();
  Gfx::bindBuffer();
  int h = Fonts::drawStringBounded(text, FontFaces::Serif::GOLD_ERROR_MESSAGE, 4+MESSAGE_WIDTH/2, 5, MESSAGE_WIDTH, ALIGN_CENTER) + 3;
  int y = HEIGHT/2 - (h+29)/2;
  Gfx::bindCanvas();
  
  lbx::LBXSpriteDataWithPalette dialogBottom{lbx::Repository::spriteFor(LSI(RESOURCE,39)), lbx::Repository::spriteFor(LSI(RESOURCE,38))->getPalette()};
  
  Gfx::drawClipped(&dialogBottom, DIALOG_X, y+h, 0, 0, DIALOG_WIDTH, 9);
  Gfx::drawClipped(LSI(RESOURCE, 38), DIALOG_X, y, 0, 0, DIALOG_WIDTH, h);
  Gfx::mergeBuffer(4, 4, DIALOG_X + (DIALOG_WIDTH - MESSAGE_WIDTH)/2, y+8, DIALOG_WIDTH, h+10);
}
