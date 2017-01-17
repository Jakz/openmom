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
#include "CombatMap.h"

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



constexpr u16 W = Combat::W;
constexpr u16 H = Combat::H;
constexpr u16 OX = 0;
constexpr u16 OY = 8;

constexpr int TILE_WIDTH = 32;
constexpr int TILE_HEIGHT = 16;

enum priority : priority_t
{
  priority_fixed = -1,
  priority_behind_units = 100,
  priority_static = priority_behind_units,
  priority_units = 200,
  priority_front_units = 300,
  
  priority_stone_wall_behind = priority_behind_units,
  priority_stone_wall_front = priority_front_units,
  
  priority_fire_wall_behind = priority_stone_wall_behind - 2,
  priority_fire_wall_front = priority_stone_wall_front + 2,
  
  priority_darkness_wall_behind = priority_stone_wall_behind - 1,
  priority_darkness_wall_front = priority_stone_wall_front + 1,
};

struct StoneWallGfxSpec
{
  SpriteInfo sprite;
  priority_t priority;
  ScreenCoord anchor;
  StoneWallGfxSpec() = default;
  //TODO: anchors must be fixed
  StoneWallGfxSpec(SpriteInfo sprite, priority_t priority) : sprite(sprite), priority(priority), anchor(2,9) { }
};

std::unordered_map<WallType, StoneWallGfxSpec, enum_hash> stoneWall = {
  { WallType::NORTH_CORNER, { LSI(CITYWALL, 0), priority_stone_wall_behind } },
  { WallType::NORTH_EAST_WALL1, { LSI(CITYWALL, 4), priority_stone_wall_behind } },
  { WallType::NORTH_EAST_WALL2, { LSI(CITYWALL, 5), priority_stone_wall_behind } },
  { WallType::EAST_CORNER, { LSI(CITYWALL, 6), priority_stone_wall_front } },
  { WallType::SOUTH_EAST_WALL1, { LSI(CITYWALL, 10), priority_stone_wall_front } },
  { WallType::SOUTH_EAST_GATE, { LSI(CITYWALL, 11), priority_stone_wall_front } },
  { WallType::SOUTH_CORNER, { LSI(CITYWALL, 9), priority_stone_wall_front } },
  { WallType::SOUTH_WEST_WALL1, { LSI(CITYWALL, 7), priority_stone_wall_front } },
  { WallType::SOUTH_WEST_WALL2, { LSI(CITYWALL, 8), priority_stone_wall_front } },
  { WallType::WEST_CORNER, { LSI(CITYWALL, 3), priority_stone_wall_front } },
  { WallType::NORTH_WEST_WALL1, { LSI(CITYWALL, 1), priority_stone_wall_behind } },
  { WallType::NORTH_WEST_WALL2, { LSI(CITYWALL, 2), priority_stone_wall_behind } }
};

struct NonSolidWallSpec
{
  SpriteInfo sprite1, sprite2;
  bool behind1, behind2;
  ScreenCoord anchor1, anchor2;
  
  NonSolidWallSpec() = default;
  //TODO: anchors must be fixed
  NonSolidWallSpec(SpriteInfo sprite1, bool behind1) : sprite1(sprite1), behind1(behind1), sprite2(LSI_PLACEHOLD), anchor1(2,11) { }
  NonSolidWallSpec(SpriteInfo sprite1, bool behind1, SpriteInfo sprite2, bool behind2) : sprite1(sprite1), behind1(behind1), sprite2(sprite2), behind2(behind2), anchor1(2,9), anchor2(2,11) { }
};

std::unordered_map<WallType, NonSolidWallSpec, enum_hash> nonSolidWall = {
  { WallType::NORTH_CORNER, { LSI(CITYWALL, 36), true } },
  { WallType::NORTH_EAST_WALL1, { LSI(CITYWALL, 40), true } },
  { WallType::NORTH_EAST_WALL2, { LSI(CITYWALL, 41), true } },
  { WallType::EAST_CORNER, { LSI(CITYWALL, 42), true, LSI(CITYWALL, 49), false } },
  { WallType::SOUTH_EAST_WALL1, { LSI(CITYWALL, 46), false } },
  { WallType::SOUTH_EAST_GATE, { LSI(CITYWALL, 47), false } },
  { WallType::SOUTH_CORNER, { LSI(CITYWALL, 45), false } },
  { WallType::SOUTH_WEST_WALL1, { LSI(CITYWALL, 43), false } },
  { WallType::SOUTH_WEST_WALL2, { LSI(CITYWALL, 44), false } },
  { WallType::WEST_CORNER, { LSI(CITYWALL, 39), true, LSI(CITYWALL, 48), false } },
  { WallType::NORTH_WEST_WALL1, { LSI(CITYWALL, 38), true } },
  { WallType::NORTH_WEST_WALL2, { LSI(CITYWALL, 37), true } }
};

ScreenCoord coordsForTile(u16 x, u16 y) { return ScreenCoord(32*x + OX + (y % 2 == 0 ? 0 : 16), 8*y + OY); }

struct entry_comparator
{
  bool operator()(const std::unique_ptr<CombatView::TileGfxEntry>& e1, const std::unique_ptr<CombatView::TileGfxEntry>& e2) const
  {
    if (e1->priority() < 0 && e2->priority() >= 0)
      return true;
    else if (e1 == e2)
      return false;
    
    if (e1->x() != e2->x())
      return e1->x() < e2->x();
    else if (e1->y() != e2->y())
      return e1->y() < e2->y();
    else
    {
      assert(e1->priority() != e2->priority());
      return e1->priority() < e2->priority();
    }
  }
};


class UnitGfxEntry : public CombatView::TileGfxEntry
{
private:
  CombatUnit* unit;
  
public:
  UnitGfxEntry(CombatUnit* unit) : CombatView::TileGfxEntry(priority_units), unit(unit) { }
  
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
protected:
  bool animated;
  SpriteInfo info;
  u16 _x, _y;
  ScreenCoord _anchor;

  void draw(SpriteInfo info)
  {
    ScreenCoord coords = coordsForTile(_x, _y);
    
    if (animated)
      Gfx::drawAnimated(info, coords + _anchor);
    else
      Gfx::draw(info, coords + _anchor);
  }
  
public:
  StaticGfxEntry(priority_t priority, SpriteInfo info, u16 x, u16 y, u16 ax, u16 ay, bool animated = false) :
  CombatView::TileGfxEntry(priority), info(info), _x(x), _y(y), _anchor(-ax, TILE_HEIGHT/2 - info.sh() + ay), animated(animated) { }
  
  StaticGfxEntry(SpriteInfo info, u16 x, u16 y, u16 ax, u16 ay, bool animated = false) : StaticGfxEntry(priority_static, info, x, y, ax, ay, animated) { }
  
  u16 x() const override { return _x; }
  u16 y() const override { return _y; }
  
  /*void setAnchorY(s16 offset) { _anchor.y = TILE_HEIGHT/2 - info.sh() + offset; }
  void setOffsetX(s16 offset) { _anchor.x = -offset; }
  void setOffset(s16 x, s16 y) { _anchor = ScreenCoord(-x, TILE_HEIGHT/2 - info.sh() + y); }*/

  void draw() override { draw(info); }
};

class StoneWallGfxEntry : public StaticGfxEntry
{
private:
  /* TODO: 3 types of stone wall are present in LBX, probably one per kind of housing? */
  static s16 offsetForHouseType(HouseType type)
  {
    switch (type)
    {
      case HouseType::NORMAL: return 0;
      case HouseType::MUD: return 12;
      case HouseType::TREE: return 24;
    }
  }
  
  
  const CombatTile* const tile;
public:
  StoneWallGfxEntry(const CombatTile* tile, HouseType houseType) : StoneWallGfxEntry(stoneWall[tile->stoneWall], tile, houseType) { }

  StoneWallGfxEntry(const StoneWallGfxSpec& spec, const CombatTile* tile, HouseType houseType) :
  StaticGfxEntry(spec.priority, spec.sprite.relative(offsetForHouseType(houseType)), tile->x(), tile->y(), spec.anchor.x, spec.anchor.y), tile(tile) { }

  using StaticGfxEntry::draw;
  void draw() override {
    draw(tile->isStoneWallDestroyed ? info.frame(1) : info);
  }
};

class NonSolidWallGfxEntry : public StaticGfxEntry
{
  const CombatTile* const tile;
  
public:
  NonSolidWallGfxEntry(const CombatTile* tile, SpriteInfo info, priority_t priority, ScreenCoord anchor) :
  StaticGfxEntry(priority, info, tile->x(), tile->y(), anchor.x, anchor.y, true), tile(tile) { }
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

void CombatView::prepareGraphics()
{
  for (u16 y = 0; y < H; ++y)
    for (u16 x = 0; x < W; ++x)
    {
      if (x != 9 || y%2 == 0)
      {
        const auto& tile = combat->tileAt(x, y);

        if (tile->stoneWall != WallType::NO_WALL)
        {
          //TODO: determine real house type
          addGfxEntry(new StoneWallGfxEntry(tile, HouseType::NORMAL));
        }
        
        if (tile->fireWall != WallType::NO_WALL)
        {
          const auto& spec = nonSolidWall[tile->fireWall];
          addGfxEntry(new NonSolidWallGfxEntry(tile, spec.sprite1, spec.behind1 ? priority_fire_wall_behind : priority_fire_wall_front, spec.anchor1));

          if (spec.sprite2.isValid())
          {
            addGfxEntry(new NonSolidWallGfxEntry(tile, spec.sprite2, spec.behind2 ? priority_fire_wall_behind : priority_fire_wall_front, spec.anchor2));
          }
        }
        
        if (tile->darknessWall != WallType::NO_WALL)
        {
          const auto& spec = nonSolidWall[tile->darknessWall];
          addGfxEntry(new NonSolidWallGfxEntry(tile, spec.sprite1.relative(14), spec.behind1 ? priority_darkness_wall_behind : priority_darkness_wall_front, spec.anchor1));

          if (spec.sprite2.isValid())
          {
            addGfxEntry(new NonSolidWallGfxEntry(tile, spec.sprite2.relative(14), spec.behind2 ? priority_darkness_wall_behind : priority_darkness_wall_front, spec.anchor2));
          }
        }
      }
    }
}

void CombatView::activate()
{
  Player* p1 = *g->getPlayers().begin();
  Player* p2 = *std::next(g->getPlayers().begin());
  
  this->combat = new Combat(*p1->getArmies().begin(), *p2->getArmies().begin(), &g->combatMechanics);
  this->combat->map()->placeFireWall(2, 4);
  //this->combat->map()->placeDarknessWall(2, 4);
  //this->combat->map()->placeStoneWall(2, 4);

  this->entriesDirty = true;
  
  //addGfxEntry(dummyUnit(2, 4));
  //addGfxEntry(dummyUnit(3, 7));

  
  //for (CombatUnit* unit : combat->getUnits())
  //  addGfxEntry(new UnitGfxEntry(unit));

  /*addMainBuilding(LSI(CMBTCITY, 17));

  addHouse(LSI(CMBTCITY, 2), 2, 7);
  addHouse(LSI(CMBTCITY, 3), 3, 6);
  addHouse(LSI(CMBTCITY, 4), 5, 6); */
  
  int bx = 2, by = 4;
  int ox = 2, oy = 9;
  
  /*auto* wall = new StaticGfxEntry(LSI(CITYWALL, 0), bx, by, ox, oy);
  wall->setPriority(5);
  addGfxEntry(wall);
  
  wall = new StaticGfxEntry(LSI(CITYWALL, 4), bx, by+1, ox, oy+1);
  wall->setPriority(5);
  addGfxEntry(wall);
  
  wall = new StaticGfxEntry(LSI(CITYWALL, 5), bx+1, by+2, ox, oy+1);
  addGfxEntry(wall);
  
  wall = new StaticGfxEntry(LSI(CITYWALL, 6), bx+1, by+3, ox, oy-1);
  addGfxEntry(wall);

  wall = new StaticGfxEntry(LSI(CITYWALL, 10), bx+1, by+4, ox, oy);
  addGfxEntry(wall);
  
  wall = new StaticGfxEntry(LSI(CITYWALL, 11), bx, by+5, ox, oy);
  addGfxEntry(wall);*/
  
  /*
  wall = new StaticGfxEntry(LSI(CITYWALL, 9), 6, 10);
  wall->setOffset(-2, 18);
  addGfxEntry(wall);*/
  
  //addRoads();

  prepareGraphics();

}

void CombatView::deactivate()
{
  entries.clear();
}

void CombatView::addRoads() { addGfxEntry(new FixedGfxEntry(LSI(CMBTCITY, cmbt_roads), 14, 40)); }
void CombatView::addMainBuilding(SpriteInfo info) { addGfxEntry(new StaticGfxEntry(info, 2, 6, -1, 14)); }
void CombatView::addHouse(SpriteInfo info, int x, int y) { addGfxEntry(new StaticGfxEntry(info, x, y, 0, 8)); }


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
        const auto& tile = combat->tileAt(x, y);
        
        //TODO: not 0,0 but player.combat.tiles[x][y]/8 %8
        Gfx::draw(LSI(CMBGRASS,tile->type), coords);
        Gfx::draw(TextureID::COMBAT_MISC_TILES, 0, 0, coords.x, coords.y);
      }
    }
  
  /*auto c = coordsForTile(2, 6);
  Gfx::rect(c.x, c.y, 30, 16, {255,0,0});*/

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
  
  if (hover.x != -1)
  {
    ScreenCoord hoverCoords = coordsForTile(hover.x, hover.y);
    //TODO: working?
    Gfx::drawAnimated(LSI(CMBTCITY, 67), hoverCoords.x, hoverCoords.y, 0);
    
    Fonts::drawString(Fonts::format("%u,%u", hover.x, hover.y), FontFaces::Small::WHITE, 5, 5, ALIGN_LEFT);
  }

  Gfx::drawClipped(TSI(COMBAT_BACKDROP,0,0), 0, 200-36, 0, 0, 320, 36);

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

UnitGfxEntry* CombatView::dummyUnit(s16 x, s16 y)
{
  Unit* unit = new RaceUnit(Data::unit("barbarian_spearmen")->as<RaceUnitSpec>());
  Army* army = new Army(player);
  unit->setArmy(army);
  CombatUnit* cunit = new CombatUnit(unit);
  cunit->setPosition(x, y);
  return new UnitGfxEntry(cunit);
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
