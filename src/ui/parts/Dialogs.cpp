#include "Dialogs.h"

#include "Gfx.h"
#include "Font.h"
#include "LBX.h"

using namespace std;
using namespace dialogs;

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
  
  Gfx::bindPalette(LSI(RESOURCE,38));
  Gfx::drawClipped(LSI(RESOURCE,39), DIALOG_X, y+h, 0, 0, DIALOG_WIDTH, 9);
  Gfx::drawClipped(LSI(RESOURCE,38), DIALOG_X, y, 0, 0, DIALOG_WIDTH, h);
  Gfx::mergeBuffer(4, 4, DIALOG_X + (DIALOG_WIDTH - MESSAGE_WIDTH)/2, y+8, DIALOG_WIDTH, h+10);
  Gfx::unbindPalette();
}

void InfoMenu::draw()
{
  static const SpriteInfo leftTop = LSI(RESOURCE, 5);
  static const SpriteInfo leftBottom = LSI(RESOURCE, 6);
  static const SpriteInfo top = LSI(RESOURCE, 7);
  static const SpriteInfo rightTop = LSI(RESOURCE, 8);
  static const SpriteInfo bottom = LSI(RESOURCE, 9);
  static const SpriteInfo rightBottom = LSI(RESOURCE, 10);
  
  const Palette* palette = top.palette();
  
  //static const auto leftWidth = leftTop.sw();
  Gfx::bindPalette(palette);
  Gfx::drawClipped(leftTop, b.x, b.y, 0, 0, leftTop.sw(), h - leftBottom.sh());
  Gfx::draw(leftBottom, b.x, b.y + h - leftBottom.sh());
  Gfx::drawClipped(top, b.x + leftTop.sw(), b.y, 0, 0, w - leftTop.sw() - rightTop.sw(), top.sh());
  Gfx::drawClipped(rightTop, b.x + w - rightTop.sw(), b.y, 0, 0, rightTop.sw(), h - rightBottom.sh());
  Gfx::draw(rightBottom, b.x + w - rightBottom.sw(), b.y + h - rightBottom.sh());
  Gfx::drawClipped(bottom, b.x + leftBottom.sw(), b.y + h - bottom.sh(), 0, 0, w - leftBottom.sw() - rightBottom.sw(), bottom.sh());
  Gfx::unbindPalette();
}
