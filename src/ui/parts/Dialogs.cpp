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

static const SpriteInfo leftTop = LSI(RESOURCE, 5);
static const SpriteInfo leftBottom = LSI(RESOURCE, 6);
static const SpriteInfo top = LSI(RESOURCE, 7);
static const SpriteInfo rightTop = LSI(RESOURCE, 8);
static const SpriteInfo bottom = LSI(RESOURCE, 9);
static const SpriteInfo rightBottom = LSI(RESOURCE, 10);
static constexpr s16 BORDER_LENGTH = 2;


InfoMenu::InfoMenu(Delegate* delegate, u16 buttonWidth) :
_delegate(delegate), buttonWidth(buttonWidth), hovered(-1),
s(buttonWidth + leftTop.sw() + rightTop.sw() - 2, delegate->buttonCount()*buttonHeight() + top.sh() + bottom.sh())
{
}

Point InfoMenu::buttonBase() const { return Point( b.x + leftTop.sw(), b.y + top.sh() ); }

int InfoMenu::buttonHeight() const { return 14; }

int InfoMenu::hoveredButton(const Point &p)
{
  const int count = _delegate->buttonCount();
  const Point base = buttonBase();
  const Size size = Size(buttonWidth, buttonHeight()*count);
  
  if (Rect(base,size).contains(p))
    return (p.y - base.y) / buttonHeight();
  else
    return -1;
}


void InfoMenu::draw() const
{
  const Palette* palette = top.palette();
  
  //static const auto leftWidth = leftTop.sw();
  Gfx::bindPalette(palette);
  Gfx::drawClipped(leftTop, b.x, b.y, 0, 0, leftTop.sw(), s.h - leftBottom.sh());
  Gfx::draw(leftBottom, b.x, b.y + s.h - leftBottom.sh());
  Gfx::drawClipped(top, b.x + leftTop.sw(), b.y, 0, 0, s.w - leftTop.sw() - rightTop.sw(), top.sh());
  Gfx::drawClipped(rightTop, b.x + s.w - rightTop.sw(), b.y, 0, 0, rightTop.sw(), s.h - rightBottom.sh());
  Gfx::draw(rightBottom, b.x + s.w - rightBottom.sw(), b.y + s.h - rightBottom.sh());
  Gfx::drawClipped(bottom, b.x + leftBottom.sw(), b.y + s.h - bottom.sh(), 0, 0, s.w - leftBottom.sw() - rightBottom.sw(), bottom.sh());
  Gfx::unbindPalette();
  
  for (u16 index = 0; index < _delegate->buttonCount(); ++index)
  {
    int frame = hovered == index ? 1 : 0;

    const auto buttonGfx = LSI(RESOURCE, 12 + index).frame(frame), borderGfx = LSI(RESOURCE, 22 + index).frame(frame);
    const Point position = buttonBase() + Point(0, buttonHeight()*index );
    
    
    Gfx::drawClipped(buttonGfx, position.x, position.y, 0, 0, buttonWidth - BORDER_LENGTH, buttonGfx.sh());
    Gfx::draw(borderGfx, position + Point(buttonWidth - BORDER_LENGTH, 0));
    
    if (_delegate)
      _delegate->drawButton(index, index == hovered, Rect(position, Size(buttonWidth, buttonHeight())));
  }
}


void ItemDetailDialog::draw()
{
  static const SpriteInfo background = LSI(ITEMISC, 25);
  static const SpriteInfo pin = LSI(ITEMISC, 26);
}
