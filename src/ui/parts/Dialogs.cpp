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
  
  lbx::LBXSpriteDataWithPalette dialogBottom{lbx::Repository::spriteFor(LSI(RESOURCE,39)), lbx::Repository::spriteFor(LSI(RESOURCE,38))->getPalette()};
  
  Gfx::drawClipped(&dialogBottom, DIALOG_X, y+h, 0, 0, DIALOG_WIDTH, 9);
  Gfx::drawClipped(LSI(RESOURCE, 38), DIALOG_X, y, 0, 0, DIALOG_WIDTH, h);
  Gfx::mergeBuffer(4, 4, DIALOG_X + (DIALOG_WIDTH - MESSAGE_WIDTH)/2, y+8, DIALOG_WIDTH, h+10);
}

void InfoMenu::draw()
{
  static const SpriteInfo leftTop_ = LSI(RESOURCE, 5);
  static const SpriteInfo leftBottom_ = LSI(RESOURCE, 6);
  static const SpriteInfo top = LSI(RESOURCE, 7);
  static const SpriteInfo rightTop_ = LSI(RESOURCE, 8);
  static const SpriteInfo bottom_ = LSI(RESOURCE, 9);
  static const SpriteInfo rightBottom_ = LSI(RESOURCE, 10);
  
  const Palette* palette = top.palette();
  
   const lbx::LBXSpriteDataWithPalette leftTop = lbx::LBXSpriteDataWithPalette(lbx::Repository::spriteFor(leftTop_), palette);
   const lbx::LBXSpriteDataWithPalette leftBottom = lbx::LBXSpriteDataWithPalette(lbx::Repository::spriteFor(leftBottom_), palette);
   const lbx::LBXSpriteDataWithPalette rightTop = lbx::LBXSpriteDataWithPalette(lbx::Repository::spriteFor(rightTop_), palette);
   const lbx::LBXSpriteDataWithPalette bottom = lbx::LBXSpriteDataWithPalette(lbx::Repository::spriteFor(bottom_), palette);
   const lbx::LBXSpriteDataWithPalette rightBottom = lbx::LBXSpriteDataWithPalette(lbx::Repository::spriteFor(rightBottom_), palette);

  
  //static const auto leftWidth = leftTop.sw();
  
  Gfx::drawClipped(&leftTop, b.x, b.y, 0, 0, leftTop.sw(), h - rightBottom.sh());
}
