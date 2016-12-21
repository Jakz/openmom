//
//  CombatView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CombatView.h"

#include "Game.h"
#include "LocalPlayer.h"
#include "Combat.h"

#include "SDLHelper.h"
#include "UnitDetailView.h"
#include "ViewManager.h"
#include "Gfx.h"
#include "Texture.h"
#include "UnitDraw.h"

enum combat_lbx_statics : lbx_index
{
  cmbt_roads = 0
};



constexpr u16 W = 10;
constexpr u16 H = 20;
constexpr u16 OX = 0;
constexpr u16 OY = 8;
const int dirs[][2] = {{0,-2},{0,-1},{1,-1},{1,0},{0,1},{1,1},{0,2},{-1,1},{0,1},{-1,0},{-1,-1},{0,-1}};


ScreenCoord coordsForTile(u16 x, u16 y) { return ScreenCoord(32*x + OX + (y % 2 == 0 ? 0 : 16), 8*y + OY); }


struct entry_comparator
{
  bool operator()(const std::unique_ptr<CombatView::TileGfxEntry>& e1, const std::unique_ptr<CombatView::TileGfxEntry>& e2) const
  {
    if (e1->priority() < 0 && e2->priority() >= 0)
      return true;
    
    if (e1->x() < e2->x())
      return true;
    else if (e1->y() < e2->y())
      return true;
    else
      return e1->priority() > e2->priority();
  }
};


class UnitGfxEntry : public CombatView::TileGfxEntry
{
private:
  CombatUnit* unit;
  
public:
  UnitGfxEntry(CombatUnit* unit) : CombatView::TileGfxEntry(10), unit(unit) { }
  
  u16 x() const override { return unit->x(); }
  u16 y() const override { return unit->y(); }
  
  void draw() override
  {
    ScreenCoord coords = coordsForTile(unit->x(), unit->y());
    
    if (unit->selected)
    {
      Gfx::draw(SpriteInfo(TextureID::COMBAT_MISC_TILES, 0, 1), coords.x, coords.y);
    }
    
    //if (player->shouldDrawSelectedArmy() || player->)
    UnitDraw::drawUnitIsoCombat(unit->getUnit(), coords.x, coords.y - 17, Dir::EAST, UnitDraw::CombatAction::STAY);
  }
};

class StaticGfxEntry : public CombatView::TileGfxEntry
{
private:
  SpriteInfo info;
  u16 _x, _y;
  
public:
  StaticGfxEntry(SpriteInfo info, u16 x, u16 y) : CombatView::TileGfxEntry(0), info(info), _x(x), _y(y) { }
  
  u16 x() const override { return _x; }
  u16 y() const override { return _y; }
  
  void draw() override
  {
    u16 height = info.sh();
    u16 offset = info.sh() <= 26 ? 16 : 20;
    ScreenCoord coords = coordsForTile(_x, _y);
    Gfx::drawAnimated(info, coords.x, coords.y - height + offset);
  }
};

class FixedGfxEntry : public CombatView::TileGfxEntry
{
private:
  SpriteInfo info;
  u16 _x, _y;
  
public:
  FixedGfxEntry(SpriteInfo info, u16 x, u16 y) : CombatView::TileGfxEntry(-1), info(info), _x(x), _y(y) { }
  
  u16 x() const override { return 0; }
  u16 y() const override { return 0; }
  
  void draw() override
  {
    Gfx::draw(info, _x, _y);
  }
};


CombatView::CombatView(ViewManager* gvm) : View(gvm), hover(Coord(-1,-1)), entriesDirty(true)
{
  
}

void CombatView::activate()
{
  Player* p1 = *g->getPlayers().begin();
  Player* p2 = *std::next(g->getPlayers().begin());
  
  this->combat = new Combat(*p1->getArmies().begin(), *p2->getArmies().begin(), &g->combatMechanics);
  this->entriesDirty = true;
  
  /*for (CombatUnit* unit : combat->getUnits())
    addGfxEntry(new UnitGfxEntry(unit));*/
  
  //addGfxEntry(new StaticGfxEntry(LSI(CMBTCITY, 22), 2, 6));
  addGfxEntry(new StaticGfxEntry(LSI(CMBTCITY, 2), 1, 6));
  addMainBuilding(LSI(CMBTCITY, 17));
  addGfxEntry(new StaticGfxEntry(LSI(CMBTCITY, 18), 4, 6));

  
  addGfxEntry(new StaticGfxEntry(LSI(CMBTCITY, 3), 3, 6));
  addGfxEntry(new StaticGfxEntry(LSI(CMBTCITY, 4), 5, 6));
  
  addRoads();


}

void CombatView::deactivate()
{
  entries.clear();
}

void CombatView::addRoads() { addGfxEntry(new FixedGfxEntry(LSI(CMBTCITY, cmbt_roads), 14, 40)); }
void CombatView::addMainBuilding(SpriteInfo info) { addGfxEntry(new StaticGfxEntry(info, 2, 6)); }

void CombatView::draw()
{
  if (entriesDirty)
  {
    std::sort(entries.begin(), entries.end(), entry_comparator());
    entriesDirty = false;
  }
  
  /*if (subState != SubState.SPELL_CAST && player.spellTarget() != null)
  {
    subState = SubState.SPELL_CAST;
  }
  else if (subState == SubState.SPELL_CAST && player.spellTarget() == null)
  {
    subState = SubState.MAIN;
  }
  
  if (player.spellTarget() != null)
    Gfx.applet.setCursor(1);
  else
    Gfx.applet.setCursor(0);
  
  if (player.combat == null)
    player.combat = new Combat(player.armies.get(0),player.game.players.get(1).armies.get(0));
   */

  for (u16 y = 0; y < H; ++y)
    for (u16 x = 0; x < W; ++x)
    {
      if (x != 9 || y%2 == 0)
      {
        const auto coords = coordsForTile(x, y);
        
        //TODO: not 0,0 but player.combat.tiles[x][y]/8 %8
        Gfx::draw(TextureID::COMBAT_GRASS_ARCANUS, 0, 0, coords.x, coords.y);
        Gfx::draw(TextureID::COMBAT_MISC_TILES, 0, 0, coords.x, coords.y);
      }
    }
  
  if (hover.x != -1)
  {
    ScreenCoord hoverCoords = coordsForTile(hover.x, hover.y);
    //TODO: working?
    Gfx::drawAnimated(TSI(COMBAT_MISC_TILES, 0, 1), hoverCoords.x, hoverCoords.y,0);
    
    Fonts::drawString(Fonts::format("%u,%u", hover.x, hover.y), FontFaces::Small::WHITE, 5, 5, ALIGN_LEFT);
  }
  

  /*
  if (reachable != null)
  {
    for (int i : reachable)
    {
      int nx = (0x00FF0000 & i) >> 16;
      int ny = i & 0x0000FFFF;
      
      int x = tileX(nx,ny);
      int y = tileY(nx,ny);
      
      if (player.combat.positionEmpty(nx,ny))
        Gfx.draw(Texture.COMBAT_MISC_TILES, 0, 1, x, y);
      else
        Gfx.draw(Texture.COMBAT_MISC_TILES, 0, 2, x, y);
    }
  }
   
  
  Collections.sort(player.combat.units);
  for (CombatUnit cunit : player.combat.units)
  {
    int x = tileX(cunit.x,cunit.y);
    int y = tileY(cunit.x,cunit.y);
    
    if (cunit.selected)
    {
      Gfx.draw(Texture.COMBAT_MISC_TILES, 0, 1, x, y);
      Gfx.draw(Texture.COMBAT_MISC_TILES, 0, 1, x, y);
      //Texture.drawAnimated(Texture.COMBAT_MISC_TILES, 2, x, y, 0);
      
    }
    
    if (player.drawSelectedArmy || player.selectedCombatUnit != cunit)
      UnitDraw.drawUnitIsoCombat(cunit.unit, x, y-17, cunit.facing, UnitDraw.CombatAction.STAY);
  }
  
  if (player.spellTarget() != null && player.spellTarget() != Target.NONE)
    Fonts.drawString("Spell", Fonts.Face.TEAL_MEDIUM, 20, 20, Align.LEFT);
  */
  
  for (const auto& entry : entries)
    entry->draw();

  Gfx::drawClipped(TextureID::COMBAT_BACKDROP, 0, 200-36, 0, 0, 320, 36);

  /*
  if (subState == SubState.SPELL_CAST)
  {
    Gfx.drawClipped(Texture.COMBAT_BACKDROP, 320-82, 200-36, 320, 0, 82, 36);
    Fonts.drawStringBounded("Select a target for "+player.currentCast().props.name+" spell.", Face.WHITE_TINY, 320-74, 200-36+4, 70, Align.LEFT);
    // TODO: finire, manca bg apposito
  }
  
  // draw combat enchantments
  int pos[][] = {{7,179,0},{246,179,0}};
  List<SpellCast> ench = player.combat.spells;
  
  // TODO: decidere posizioni mago a sx e a dx
  for (SpellCast cast : ench)
  {
    int[] pp = cast.owner == player ? pos[0] : pos[1];
    
    Gfx.draw(TextureIDs.get(cast.spell), pp[0]+pp[2]++*17, pp[1]);
  }
  */
}

void CombatView::drawUnitProps(CombatUnit *unit, bool onTheLeft)
{
  const ScreenCoord coords = onTheLeft ? ScreenCoord(235, 5) : ScreenCoord(5, 5);
  
  /*
  Gfx.canvas.fill(0xA0101010);
  Gfx.canvas.stroke(94,114,138,255);
  Gfx.canvas.strokeWeight(2.0f);
  //Gfx.canvas.rect(ox,oy,80,50);
  Fonts.drawString(Integer.toString(unitProps.unit.getProperty(Property.MELEE)), Fonts.Face.YELLOW_TINY_STROKE, ox+7, oy+13, Align.RIGHT);
  Fonts.drawString(Integer.toString(unitProps.unit.getProperty(Property.RANGED)), Fonts.Face.YELLOW_TINY_STROKE, ox+7, oy+21, Align.RIGHT);
  Fonts.drawString(Integer.toString(unitProps.unit.getProperty(Property.SHIELDS)), Fonts.Face.YELLOW_TINY_STROKE, ox+55, oy+13, Align.RIGHT);
  Fonts.drawString(Integer.toString(unitProps.unit.getProperty(Property.RESIST)), Fonts.Face.YELLOW_TINY_STROKE, ox+55, oy+21, Align.RIGHT);
  // movement
  float percent = unitProps.unit.percentHealth();
  int wi = (int)(80 * percent);
  Fonts.drawString("Health", Fonts.Face.YELLOW_TINY_STROKE, ox+7, 42, Align.LEFT);
  Gfx.canvas.stroke(1.5f);
  Gfx.canvas.fill(0xFFD02000);
  Gfx.canvas.stroke(0xFFFF4000);
  //Gfx.canvas.rect(ox+33+wi, oy+39, 80-wi, 8);
  Gfx.canvas.fill(0xFF00D020);
  Gfx.canvas.stroke(0xFF00FF40);
  //Gfx.canvas.rect(ox+33, oy+39, wi, 8);
  
  
  Fonts.drawString(unitProps.unit.name(), Fonts.Face.YELLOW_TINY_STROKE, ox+37, oy+5, Align.CENTER);
  
  Gfx.draw(Texture.UNIT_DETAIL_PROPS_EMPTY, 0, 0, ox+15, oy+13);
  Gfx.draw(Texture.UNIT_DETAIL_PROPS_EMPTY, 0, 4, ox+15, oy+21);
  Gfx.draw(Texture.UNIT_DETAIL_PROPS_EMPTY, 0, 9, ox+63, oy+13);
  Gfx.draw(Texture.UNIT_DETAIL_PROPS_EMPTY, 0, 11, ox+63, oy+21);
   */
}

void CombatView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  CombatUnit* unit = combat->unitAtTile(hover.x, hover.y);
  
  if (b == MouseButton::BUTTON_LEFT /* && player->isCurrentlyPlaying()*/)
  {
    Target target = player->getSpellTarget();
    
    if (unit && target == Target::FRIENDLY_UNIT && unit->getOwner() == player) // TODO: this check should be fixed by overloading operator==
      g->castSpell(unit->getUnit(), player, true);
    else if (unit && target == Target::ENEMY_UNIT && unit->getOwner() != player)
      g->castSpell(unit->getUnit(), player, true);
    else if (unit && unit->hasMoves())
    {
      /*
      if (unit != combat.selectedUnit && unit.owner == player)
      {
        combat.select(unit);
        reachable = combat.reachable(unit);
      }
      else if (combat.selectedUnit != null && unit != combat.selectedUnit && combat.relativeFacing(combat.selectedUnit, unit) != -1)
      {
        combat.attack(combat.selectedUnit,unit);
      }
      else if (combat.selectedUnit == unit)
        combat.deselect();
       */
    }
    /*
    else if (combat.selectedUnit != null && combat.selectedUnit.moves > 0)
    {
      // request unit movement
      combat.moveUnit(null, hoverX, hoverY);
      
      // recompute reachable if there are available moves
      if (combat.selectedUnit != null)
        reachable = combat.reachable(combat.selectedUnit);
    }*/
  }
  else if (b == MouseButton::BUTTON_RIGHT)
  {
    if (unit)
    {
      SDL::gvm->unitDetailView()->setUnit(unit->getUnit());
      SDL::gvm->switchOverview(VIEW_UNIT);
    }
  }
  
  /*
   if (combat.selectedUnit == null)
   reachable = null;
   */
}


void CombatView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  int th = 16;
  int tw = 32;
  
  if (x >= OX && x < tw*W + OX && y > OY && y < th*10 + OY)
  {
    int cx = x - OX;
    int cy = y - OY;
    int rx = cx / (tw/2);
    int ry = cy / (th/2);
    float ratio = tw/(float)th;
    int mx = cx % (tw/2);
    int my = cy % (th/2);
    
    hover = Coord(rx/2, ry);

    if ((rx+ry)%2 == 0)
    {
      if (mx <= ratio*(th/2 - my))
      {
        --hover.y;
        if (rx%2 == 0)
          --hover.x;
      }
    }
    else
    {
      if (rx%2 == 0)
        --hover.x;
      
      if (mx >= ratio*(my))
      {
        --hover.y;
        //if (rx%2 == 0)
        //	--hoverX;
        if (rx%2 == 0)
          ++hover.x;
      }
    }
    
    if (hover.x < 0 || hover.y < 0 || hover.y > H-1 || (hover.y%2 == 1 && hover.x == 9))
      hover.x = -1;
    
    //printf("HOVER: %d %d\n", hover.x, hover.y);
  }
}
