#include "Dialogs.h"

#include "Gfx.h"
#include "Font.h"
#include "LBX.h"
#include "GfxData.h"

#include "game/units/Items.h"

using namespace std;
using namespace dialogs;

void dialogs::drawErrorDialog(const std::string& text)
{
  static const FontSpriteSheet* font = fonts::SerifFont::withShadowAndSingle(fonts::base::SERIF_GOLD, {121,85,36}, {128,13,4});
  
  //TODO: check all positioning
  constexpr u32 MESSAGE_WIDTH = 168;//174;
  constexpr u32 DIALOG_WIDTH = 186;
  constexpr u32 DIALOG_X = 68;
  
  Gfx::resetBuffer();
  Gfx::bindBuffer();
  int h = Fonts::drawStringBounded(text, font, 3+MESSAGE_WIDTH/2, 5, MESSAGE_WIDTH, ALIGN_CENTER) + 3 + 1;
  int y = HEIGHT/2 - (h+29)/2;
  Gfx::bindCanvas();
  
  Gfx::bindPalette(LSI(RESOURCE,38));
  Gfx::drawClipped(LSI(RESOURCE,39), DIALOG_X, y+h, 0, 0, DIALOG_WIDTH, 9);
  Gfx::drawClipped(LSI(RESOURCE,38), DIALOG_X, y, 0, 0, DIALOG_WIDTH, h);
  Gfx::unbindPalette();
  Gfx::mergeBuffer(4, 4, DIALOG_X + (DIALOG_WIDTH - MESSAGE_WIDTH)/2, y+8, DIALOG_WIDTH, h+10);
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
  
  const auto& title = _delegate->title();
  Fonts::drawString(title, fonts::base::SERIF_GOLD, b.x + s.w/2, b.y + 3, ALIGN_CENTER); // TODO: check font face
  
  //TODO: check if button background are in correct order and such
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



void ItemDetailDialog::draw(const items::Item* item)
{
  static const SpriteInfo background = LSI(ITEMISC, 25);
  static const SpriteInfo pin = LSI(ITEMISC, 26);
  
  Gfx::draw(background, b);
  
  Gfx::draw(GfxData::itemGfxSpec(item->type(), item->gfx()), b.x + 11, b.y + 8);
  Fonts::drawString(item->name(), FontFaces::Serif::GOLD_SHADOW, b.x + 30, b.y + 11, ALIGN_LEFT);
  
  const char* properties[] = { "+1 Defense", "+1 Resistance" };
  
  for (size_t i = 0; i < sizeof(properties)/sizeof(properties[0]); ++i)
  {
    Gfx::draw(pin, b.x + 11, b.y + 34 + 11*i);
    //TODO: font color?
    Fonts::drawString(properties[i], FontFaces::Medium::TEAL_STROKE, b.x + 19, b.y + 32 + 11*i, ALIGN_LEFT);
  }
}

Size ItemDetailDialog::size() const
{
  const SpriteInfo bg = LSI(ITEMISC, 25);
  return Size(bg.sw(), bg.sh());
}
