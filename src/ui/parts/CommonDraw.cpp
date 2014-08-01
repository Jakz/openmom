#include "CommonDraw.h"

#include "Player.h"
#include "UnitSpec.h"
#include "Unit.h"

#include "GUISettings.h"
#include "Texture.h"
#include "GfxData.h"
#include "Gfx.h"
#include "Font.h"

void CommonDraw::drawMovement(u16 v, u16 x, u16 y, u16 c)
{
  if (v > 0)
  {
    for (int i = v; i > 0; --i)
      if (i >= 10)
      {
        i -= 9;
        Gfx::draw(TextureID::UNIT_DETAIL_MOVEMENT, 0, c, x, y);
        x += 15;
      }
      else
      {
        Gfx::draw(TextureID::UNIT_DETAIL_MOVEMENT, 0, c, x, y);
        x += 8;
      }
  }
}

void CommonDraw::drawGoldUpkeep(u16 gold, u16 x, u16 y)
{
  if (gold > 0)
  {
    for (int i = gold; i > 0; --i)
      if (i >= 10)
      {
        i -= 9;
        Gfx::draw(TextureID::CITY_PRODUCTION, 2, 2, x, y);
        x += 15;
      }
      else
      {
        Gfx::draw(TextureID::CITY_PRODUCTION, 2, 0, x, y);
        x += 8;
      }
  }
}

void CommonDraw::drawUpkeep(const Upkeep& uk, u16 x, u16 y)
{
  if (uk.mana > 0)
  {
    for (int i = uk.mana; i > 0; --i)
      if (i >= 10)
      {
        i -= 9;
        Gfx::draw(TextureID::CITY_PRODUCTION, 3, 2, x, y);
        x += 15;
      }
      else
      {
        Gfx::draw(TextureID::CITY_PRODUCTION, 3, 0, x, y);
        x += 8;
      }
  }
  if (uk.gold > 0)
  {
    for (int i = uk.gold; i > 0; --i)
      if (i >= 10)
      {
        i -= 9;
        Gfx::draw(TextureID::CITY_PRODUCTION, 2, 2, x, y);
        x += 15;
      }
      else
      {
        Gfx::draw(TextureID::CITY_PRODUCTION, 2, 0, x, y);
        x += 8;
      }
  }
  if (uk.food > 0)
  {
    for (int i = uk.food; i > 0; --i)
      if (i >= 10)
      {
        i -= 9;
        Gfx::draw(TextureID::CITY_PRODUCTION, 0, 2, x, y);
        x += 15;
      }
      else
      {
        Gfx::draw(TextureID::CITY_PRODUCTION, 0, 0, x, y);
        x += 8;
      }
  }
}

void CommonDraw::drawWizardGem(const Player *player, u16 x, u16 y)
{
  Gfx::draw(TextureID::WIZARD_PORTRAITS_GEMS, player->color, player->wizard.ident, x, y);
}

struct PropertyDrawInfo
{
  const Property property;
  const s16 index;
  const std::string name;
};

/* TODO: complete for subtypes of melee and ranged attacks */
static const PropertyDrawInfo propDetails[] = {
  {Property::MELEE, 0, "Melee"},
  {Property::RANGED, -1, "Range"},
  {Property::SHIELDS, 9, "Armor"},
  {Property::RESIST, 11, "Resist"},
  {Property::HIT_POINTS, 10, "Hits"}
};

static const u16 PROPERTY_COUNT = sizeof(propDetails)/sizeof(propDetails[0]);

void CommonDraw::drawUnitProps(const UnitSpec* unit, u16 xx, u16 yy, s16 max)
{
  int tx = xx;
  int ty = yy;
  for (int i = 0; i < PROPERTY_COUNT; ++i)
  {
    Fonts::drawString(propDetails[i].name, FontFace::TEAL_SERIF, xx-43, yy - 1, ALIGN_LEFT);
    s16 tot = unit->getProperty(propDetails[i].property);
		
    if (tot > 0)
    {
      for (int j = 0; j < tot; ++j)
      {
        if (propDetails[i].property == Property::RANGED && static_cast<Ranged>(unit->getProperty(Property::RANGED_TYPE)) != Ranged::NONE)
          Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, GfxData::rangedGfxIndex(static_cast<Ranged>(unit->getProperty(Property::RANGED_TYPE))), xx, yy);
        else
          Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, propDetails[i].index, xx, yy);
        xx += 10;
        if ((j+1) % 5 == 0)
          xx += 2;
        
        if ((j+1) % max == 0)
        {
          xx = tx+3;
          yy = ty+2;
        }
      }
    }
    
    yy = ty + 11;
    ty = yy;
    xx = tx;
  }
}

void CommonDraw::drawUnitPropsComplete(const Unit* unit, u16 xx, u16 yy, s16 max)
{
  // TODO: mancano i bonus negativi (anneriscono l'icona credo?)
  
  if (!GUISettings::customUnitProps)
  {
    int tx = xx;
    int ty = yy;
    for (int i = 0; i < PROPERTY_COUNT; ++i)
    {
      Fonts::drawString(propDetails[i].name, FontFace::TEAL_SERIF, xx-43, yy - 1, ALIGN_LEFT);
      int tot = unit->getBaseProperty(propDetails[i].property)+unit->getBonusProperty(propDetails[i].property);
			
      if (tot > 0)
      {
        for (int j = 0; j < tot; ++j)
        {
          int row = j > unit->getBaseProperty(propDetails[i].property) - 1 ? 1 : 0;
          
          if (propDetails[i].property == Property::RANGED && static_cast<Ranged>(unit->getProperty(Property::RANGED_TYPE)) != Ranged::NONE)
            Gfx::draw(TextureID::UNIT_DETAIL_PROPS, row, GfxData::rangedGfxIndex(static_cast<Ranged>(unit->getProperty(Property::RANGED_TYPE))), xx, yy);
          else
            Gfx::draw(TextureID::UNIT_DETAIL_PROPS, row, propDetails[i].index, xx, yy);
          xx += 10;
          if ((j+1) % 5 == 0)
            xx += 2;
          
          if ((j+1) % max == 0)
          {
            xx = tx+3;
            yy = ty+2;
          }
        }
      }
      
      yy = ty + 11;
      ty = yy;
      xx = tx;
    }
  }
  else
  {
    xx += 5;
    yy += 2;
    
    int ty = yy;
    int tx = xx;
    
    Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 0, xx-43, yy-1);
    Fonts::drawString(Fonts::format("Melee: %d",unit->getProperty(Property::MELEE)), FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
    yy += 10;
    
    
    if (unit->getProperty(Property::RANGED) > 0)
    {
      Ranged rtype = static_cast<Ranged>(unit->getProperty(Property::RANGED_TYPE));
      if (rtype != Ranged::ARROW && rtype != Ranged::BULLET && rtype != Ranged::ROCK)
        Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 4, xx-43, yy-1);
      else
        Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, GfxData::rangedGfxIndex(rtype), xx-43, yy-1);
      
      Fonts::drawString(Fonts::format("Ranged: %d",unit->getProperty(Property::RANGED)), FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
    }
    yy += 10;
    //TextureID::draw(TextureID::UNIT_DETAIL_PROPS, 0, 4, xx-43, yy-1);
    
    //Fonts::drawString("Ranged: "+unit->getProperty(Property::RANGED), FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
    Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 9, xx-43, yy-1);
    Fonts::drawString("Armor", FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
    xx += 45;
    static Property pp1[] = {Property::SHIELDS, Property::SHIELDS_RANGED,Property::SHIELDS_CHAOS,Property::SHIELDS_DEATH,Property::SHIELDS_LIFE,Property::SHIELDS_NATURE,Property::SHIELDS_SORCERY};
    Property* pp = pp1;
    for (int i = 0; i < 7; ++i)
    {
      Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, GfxData::propertyGfxIndex(pp[i]), xx-43, yy-1);
      Fonts::drawString(Fonts::format("%d",unit->getProperty(pp[i])), FontFace::TEAL_SMALL, xx - 32, yy + 1, ALIGN_LEFT);
      xx += 22;
    }
    xx = tx;
    //Fonts::drawString("Armor: "+unit->getProperty(Property::ARMOR), FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
    yy += 10;
    Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 11, xx-43, yy-1);
    Fonts::drawString("Resist", FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
    xx += 60;
    static Property pp2[] = {Property::RESIST_CHAOS,Property::RESIST_DEATH,Property::RESIST_LIFE,Property::RESIST_NATURE,Property::RESIST_SORCERY};
    pp = pp2;
    for (int i = 0; i < 5; ++i)
    {
      Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 14+i, xx-43, yy-1);
      Fonts::drawString(Fonts::format("%d",unit->getProperty(pp[i])), FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
      xx += 26;
    }
    xx = tx;
    yy += 10;
    Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 10, xx-43, yy-1);
    Fonts::drawString(Fonts::format("Hits: %d",unit->getProperty(Property::HIT_POINTS)), FontFace::TEAL_SMALL, xx - 30, yy + 1, ALIGN_LEFT);
    yy = ty;
    Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 12, xx+50, yy-1);
    Fonts::drawString(Fonts::format("%d%% Hit",unit->getProperty(Property::TO_HIT)), FontFace::TEAL_SMALL, xx+63, yy+1, ALIGN_LEFT);
    xx += 50;
    Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 20, xx+50, yy-1);
    Fonts::drawString(Fonts::format("%d% Def",unit->getProperty(Property::TO_DEFEND)), FontFace::TEAL_SMALL, xx+63, yy+1, ALIGN_LEFT);
    xx = tx;
    yy += 10;
    if (unit->getProperty(Property::RANGED) > 0)
    {
      Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, GfxData::rangedGfxIndex(static_cast<Ranged>(unit->getProperty(Property::RANGED_TYPE))), xx+60, yy-1);
      Fonts::drawString(Fonts::format("Ammo: %d",unit->getProperty(Property::AMMO)), FontFace::TEAL_SMALL, xx+73, yy+1, ALIGN_LEFT);
    }
    yy += 10;
    yy += 10;
    yy += 10;
    Gfx::draw(TextureID::UNIT_DETAIL_PROPS, 0, 19, xx+60, yy-1);
    Fonts::drawString(Fonts::format("Figures: %d",unit->getProperty(Property::FIGURES)), FontFace::TEAL_SMALL, xx+73, yy+1, ALIGN_LEFT);
  }
}