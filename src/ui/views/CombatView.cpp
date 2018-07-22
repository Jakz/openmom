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
#include "SpellEffectAnim.h"
#include "ViewManager.h"
#include "GfxData.h"
#include "Gfx.h"
#include "Texture.h"
#include "UnitDraw.h"

using namespace combat;

enum combat_lbx_statics
{
  roads_static = LBXI(CMBTCITY, 0),
  flying_fortess = LBXI(CMBTCITY, 113),
  
  water_tile_arcanus = LBXI(CMBTCITY, 109),
  water_tile_myrran = LBXI(RESOURCE, 71),
  
  tree = LBXI(CMBGRASS, 48),
  rock = LBXI(CMBGRASS, 53),
  
  hover_tile = LBXI(CMBTCITY, 67),
  selected_tile = LBXI(CMBTCITY, 68),

  hitpoints_background = LBXI(COMPIX, 18),
  
  lower_backdrop = LBXI(BACKGRND, 3),
};

/* default combat tiles lbx is made by 58 tiles
  4 default grass
  4 rough grass
  12*2 default/rough grass joinings
  16 hills
  5 trees
  5 rocks
*/

/* all graphical elements of the battlefield are kept in a list of sorted
   items, this allows to draw things in correct order: negative priorities
   are always drawn before all others, then positive priorities are drawn in
   order from lowest to highest
*/
enum priority : priority_t
{
  priority_tile = -100,
  priority_fixed = -10,
  priority_roads = -50,
  
  priority_prop = 50,
  
  priority_hover_symbol = 20,
  
  priority_behind_units = 100,
  priority_static = priority_behind_units,
  priority_units = 200,
  priority_front_units = 300,
  priority_projectiles = 350,
  
  priority_stone_wall_behind = priority_behind_units,
  priority_stone_wall_front = priority_front_units,
  
  priority_fire_wall_behind = priority_stone_wall_behind - 2,
  priority_fire_wall_front = priority_stone_wall_front + 2,
  
  priority_darkness_wall_behind = priority_stone_wall_behind - 1,
  priority_darkness_wall_front = priority_stone_wall_front + 1,
};

#pragma mark Tile Joins management

enum class MaskMode
{
  ALL,
  DIAGONAL,
  ORTHO
};

/* this utility functions computes the bitfield mask of neighbours according
   to a specified predicate, optionally computing it only on orthogonal or 
  diagonal directions
*/
DirJoin computeMask(MaskMode mode, const CombatTile* tile, predicate<CombatTile*> lambda)
{
  static const Dir neighbours[] = { Dir::NORTH, Dir::NORTH_EAST, Dir::EAST, Dir::SOUTH_EAST, Dir::SOUTH, Dir::SOUTH_WEST, Dir::WEST, Dir::NORTH_WEST };
  static const DirJoin flags[] = { DirJoin::N, DirJoin::NE, DirJoin::E, DirJoin::SE, DirJoin::S, DirJoin::SW, DirJoin::W, DirJoin::NW };
  
  size_t startIndex = mode == MaskMode::DIAGONAL ? 1 : 0;
  size_t increment = mode == MaskMode::ALL ? 1 : 2;
  
  DirJoin mask = DirJoin::NONE;
  for (size_t i = startIndex; i < 8; i += increment)
  {
    auto* ntile = tile->neighbour(neighbours[i]);
    if (lambda(ntile))
      mask |= flags[i];
  }
  
  return mask;
};

#pragma mark Environments

struct combat_env_hash
{
  size_t operator()(const CombatEnvironment& env) const
  {
    return std::hash<size_t>()(static_cast<size_t>(env.type) << 16 | env.plane);
  }
};

static const std::unordered_map<CombatEnvironment, LBXID, combat_env_hash> environments = {
  { { CombatEnvironment::Type::GRASS, Plane::ARCANUS }, LBXID::CMBGRASS },
  { { CombatEnvironment::Type::GRASS, Plane::MYRRAN }, LBXID::CMBGRASC },
  { { CombatEnvironment::Type::DESERT, Plane::ARCANUS }, LBXID::CMBDESRT },
  { { CombatEnvironment::Type::DESERT, Plane::MYRRAN }, LBXID::CMBDESRC },
  { { CombatEnvironment::Type::MOUNTAIN, Plane::ARCANUS }, LBXID::CMBMOUNT },
  { { CombatEnvironment::Type::MOUNTAIN, Plane::MYRRAN }, LBXID::CMBMOUNC },
  { { CombatEnvironment::Type::TUNDRA, Plane::ARCANUS }, LBXID::CMBTUNDR },
  { { CombatEnvironment::Type::TUNDRA, Plane::MYRRAN }, LBXID::CMBTUNDC },
  /* placeholders, not used */
  { { CombatEnvironment::Type::OCEAN, Plane::ARCANUS }, LBXID::CMBGRASS },
  { { CombatEnvironment::Type::OCEAN, Plane::MYRRAN }, LBXID::CMBGRASC }
};

#pragma mark Rough Terrain

class rough_terrain_map
{
  enum joinings
  {
    full = 4,
    corner_south = 8,
    horizontal_sw = 10,
    empty_corner_north = 12,
    corner_west = 14,
    empty_corner_east = 16,
    horizontal_nw = 18,
    corner_north = 20,
    empty_corner_south = 22,
    horizontal_ne = 24,
    corner_east = 26,
    empty_corner_west = 28,
    horizontal_se = 30,
  };
  
public:
  rough_terrain_map()
  {
    
  };
  
  SpriteInfo operator[](DirJoin mask)
  {
    DirJoin omask = mask & DirJoin::CROSS;
    SpriteInfo info;
    
    switch (omask)
    {
      case DirJoin::CORNER_SE_SW: info = LSI(CMBGRASS, corner_south); break;
      case DirJoin::CORNER_NE_SE: info = LSI(CMBGRASS, corner_east); break;
      case DirJoin::CORNER_NW_NE: info = LSI(CMBGRASS, corner_north); break;
      case DirJoin::CORNER_SW_NW: info = LSI(CMBGRASS, corner_west); break;
        
      case DirJoin::HALF_NE: info = LSI(CMBGRASS, horizontal_ne); break;
      case DirJoin::HALF_NW: info = LSI(CMBGRASS, horizontal_nw); break;
      case DirJoin::HALF_SE: info = LSI(CMBGRASS, horizontal_se); break;
      case DirJoin::HALF_SW: info = LSI(CMBGRASS, horizontal_sw); break;
        
      default:
      {
        if (mask == DirJoin::ALL)
          info = LSI(CMBGRASS, full);
        else
        {
          DirJoin nmask = ~mask;
          
          if ((nmask & DirJoin::S) == DirJoin::S) info = LSI(CMBGRASS, empty_corner_south);
          else if ((nmask & DirJoin::E) == DirJoin::E) info = LSI(CMBGRASS, empty_corner_east);
          else if ((nmask & DirJoin::W) == DirJoin::W) info = LSI(CMBGRASS, empty_corner_west);
          else if ((nmask & DirJoin::N) == DirJoin::N) info = LSI(CMBGRASS, empty_corner_north);
        }
       
        break;
      }
    }
    
    return info.index() == full ?
      info.relative(Util::randomIntUpTo(4)) :
      info.relative(Util::randomIntUpTo(1));
  }
} roughTerrainMap;

#pragma mark Hills

static const std::unordered_map<DirJoin, SpriteInfo, enum_hash> hillsMap = {
  { DirJoin::SW, LSI(CMBGRASS, 32) },
  { DirJoin::DIAGONAL_NE_SW, LSI(CMBGRASS, 33) },
  { DirJoin::SE, LSI(CMBGRASS, 34) },
  { DirJoin::DIAGONAL_NW_SE, LSI(CMBGRASS, 35) },
  { DirJoin::CROSS, LSI(CMBGRASS, 36) },
  { DirJoin::NE, LSI(CMBGRASS, 37) },
  { DirJoin::NW, LSI(CMBGRASS, 38) },
  { DirJoin::CORNER_SE_SW, LSI(CMBGRASS, 39) },
  { DirJoin::HALF_SW, LSI(CMBGRASS, 40) },
  { DirJoin::CORNER_SW_NW, LSI(CMBGRASS, 41) },
  { DirJoin::HALF_SE, LSI(CMBGRASS, 42) },
  { DirJoin::HALF_NW, LSI(CMBGRASS, 43) },
  { DirJoin::CORNER_NE_SE, LSI(CMBGRASS, 44) },
  { DirJoin::HALF_NE, LSI(CMBGRASS, 45) },
  { DirJoin::CORNER_NW_NE, LSI(CMBGRASS, 46) },
  { DirJoin::NONE, LSI(CMBGRASS, 47) }
};

#pragma mark Walls

struct StoneWallGfxSpec
{
  SpriteInfo sprite;
  priority_t priority;
  Point anchor;
  StoneWallGfxSpec() = default;
  //TODO: anchors must be fixed
  StoneWallGfxSpec(SpriteInfo sprite, priority_t priority) : sprite(sprite), priority(priority), anchor(2,9) { }
};

static const std::unordered_map<WallType, StoneWallGfxSpec, enum_hash> stoneWall = {
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
  Point anchor1, anchor2;
  
  NonSolidWallSpec() = default;
  //TODO: anchors must be fixed
  NonSolidWallSpec(SpriteInfo sprite1, bool behind1) : sprite1(sprite1), behind1(behind1), sprite2(LSI_PLACEHOLD), anchor1(2,11) { }
  NonSolidWallSpec(SpriteInfo sprite1, bool behind1, SpriteInfo sprite2, bool behind2) : sprite1(sprite1), behind1(behind1), sprite2(sprite2), behind2(behind2), anchor1(2,9), anchor2(2,11) { }
};

static const std::unordered_map<WallType, NonSolidWallSpec, enum_hash> nonSolidWall = {
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

#pragma mark Roads

struct RoadGfxSpec
{
  std::array<SpriteInfo, 2> normal;
  std::array<SpriteInfo, 2> enchanted;
};


static const std::unordered_map<DirJoin, RoadGfxSpec, enum_hash> roadGraphics = {
  { DirJoin::HORIZONTAL_NW_SE, { { LSI(CMBTCITY, 69), LSI(CMBTCITY, 76) }, { LSI(CMBTCITY, 83), LSI(CMBTCITY, 90) } } },
  { DirJoin::HORIZONTAL_NE_SW, { { LSI(CMBTCITY, 70), LSI(CMBTCITY, 77) }, { LSI(CMBTCITY, 84), LSI(CMBTCITY, 91) } } },
  { DirJoin::CORNER_NW_NE, { { LSI(CMBTCITY, 71), LSI(CMBTCITY, 78) }, { LSI(CMBTCITY, 85), LSI(CMBTCITY, 92) } } },
  { DirJoin::CORNER_NE_SE, { { LSI(CMBTCITY, 72), LSI(CMBTCITY, 79) }, { LSI(CMBTCITY, 86), LSI(CMBTCITY, 93) } } },
  { DirJoin::CORNER_SE_SW, { { LSI(CMBTCITY, 73), LSI(CMBTCITY, 80) }, { LSI(CMBTCITY, 87), LSI(CMBTCITY, 94) } } },
  { DirJoin::CORNER_SW_NW, { { LSI(CMBTCITY, 74), LSI(CMBTCITY, 81) }, { LSI(CMBTCITY, 88), LSI(CMBTCITY, 95) } } },
  { DirJoin::CROSS, { { LSI(CMBTCITY, 75), LSI(CMBTCITY, 82) }, { LSI(CMBTCITY, 89), LSI(CMBTCITY, 96) } } }
};

#pragma mark Buildings

struct BuildingGfxSpec
{
  SpriteInfo info;
  u32 count;
  Point offset;
};

static const std::unordered_map<TileBuilding, BuildingGfxSpec, enum_hash> buildingGraphics = {
  { TileBuilding::FORTRESS, { LSI(CMBTCITY, 17), 1, {-1, 14} } },
  { TileBuilding::TOWER_WIZARDRY, { LSI(CMBTCITY, 20), 1, {-1, 14} } },
  { TileBuilding::CAVE, { LSI(CMBTCITY, 19), 1, {-1, 14} } },
  { TileBuilding::RUINS, { LSI(CMBTCITY, 21), 1, {-1, 14} } },
  { TileBuilding::KEEP, { LSI(CMBTCITY, 22), 1, {-1, 14} } },
  { TileBuilding::TEMPLE, { LSI(CMBTCITY, 23), 1, {-1, 14} } },
  { TileBuilding::ANCIENT_RUINS, { LSI(CMBTCITY, 121), 1, {-1, 14} } },
  
  { TileBuilding::HOUSE, { LSI_PLACEHOLD, 5, {0, 8} } } /* no sprite info specified because it depends on race */
};

#pragma mark Rivers

using GfxEntry = CombatView::GfxEntry;

class ProjectileGfxEntry : public GfxEntry
{
  using coord_t = combat::CombatCoord;
  
private:
  enum Phase
  {
    MOVING,
    HITTING
  };
  
  u32 count;
  SpriteInfo effect;
  std::array<combat::CombatCoord, 2> coords;
  std::array<Point, 2> points;
  Point delta;
  Dir facing;
  Phase phase;
  float speed = 0.005f;
  
  mutable CombatCoord position;
  mutable float progress;
  
  //TODO: quite an hack since sprites are ordered starting by north in clockwise manner in LBX
  s32 spriteDeltaForFacing(Dir facing) const { return (s32)facing; }
  
public:
  ProjectileGfxEntry(CombatView* view, coord_t start, coord_t end, SpriteInfo effect) : CombatView::GfxEntry(view, priority_projectiles),
  coords({start, end}), points({CombatView::coordsForTile(start.x, start.y), CombatView::coordsForTile(end.x, end.y)}),
  delta(points[1] - points[0]), effect(effect), count(1), progress(0.0f), position(start), phase(Phase::MOVING)
  {
    facing = Dir::S;
  }
  
  u16 x() const override { return position.x; }
  u16 y() const override { return position.y; }
  
  void draw() const override
  {
    auto gfx = effect.frame(spriteDeltaForFacing(facing), (Gfx::fticks / 3) % 3);
    Point point = points[0] + delta*progress;
    
    auto offset = UnitDraw::offsetForFigures(true, 1);
    Gfx::draw(gfx,
              point.x + offset[0].x,
              point.y + offset[0].y - 17
              );
    
    progress += speed;
    
    Coord newPosition = CombatView::tileForCoords(point.x, point.y);
    
    if (newPosition != position)
    {
      position = newPosition;
      setDirty();
    }
    
  }
  
  bool destroyable() const override { return progress >= 1.0f; }
};

class UnitGfxEntry : public GfxEntry
{
private:
  CombatUnit* unit;
  
  mutable UnitDraw::CombatAction action;
  mutable CombatCoord position;
  mutable std::list<CombatPosition> goal;
  mutable float progress;
  
  static constexpr float PROGRESS_PER_TICK = 0.05f;
  
public:
  UnitGfxEntry(CombatView* view, CombatUnit* unit) : CombatView::GfxEntry(view, priority_units), unit(unit), action(UnitDraw::CombatAction::STAY), position(unit->position) { }
  ~UnitGfxEntry()
  {
    //TODO: combatView->unitsMap.erase(this);
  }
  
  bool isAnimating() const { return action != UnitDraw::CombatAction::STAY; }
  
  u16 x() const override { return position.x; }
  u16 y() const override { return position.y; }
  
  void draw() const override
  {
    Point coords = CombatView::coordsForTile(x(), y());
    
    if (action == UnitDraw::CombatAction::STAY)
    {
      if (unit->selected)
        Gfx::draw(SpriteInfo(TextureID::COMBAT_MISC_TILES, 0, 1), coords.x, coords.y);
    }
    else if (action == UnitDraw::CombatAction::MOVE)
    {
      /* if we ended current move animation */
      if (progress >= 1.0f)
      {
        /* remove goal position and assign it to position */
        position = goal.front();
        goal.pop_front();
        coords = CombatView::coordsForTile(x(), y());
        
        /* if there are other moves pending then reset and keep moving */
        action = !goal.empty() ? UnitDraw::CombatAction::MOVE : UnitDraw::CombatAction::STAY;
        
        if (action == UnitDraw::CombatAction::MOVE)
        {
          progress = 0.0f;
          unit->setFacing(goal.front().facing);
        }
      }
      else
      {
        Point destCoords = CombatView::coordsForTile(goal.front().x, goal.front().y);
        Point dx = destCoords - coords;
        coords += dx * progress;
        
        progress += PROGRESS_PER_TICK;
      }
    }
    else if (action == UnitDraw::CombatAction::ATTACK)
    {
      if (progress >= 1.0f)
      {
        progress = 0.0f;
        action = UnitDraw::CombatAction::STAY;
      }
      else
        progress += PROGRESS_PER_TICK;
    }
    
    // TODO: this -17 sounds like an hack, it's used also in CombatProjectile animation, maybewe should move it figure offsets
    UnitDraw::drawUnitIsoCombat(unit->getUnit(), coords.x, coords.y - 17, unit->facing(), action);
  }
  
  void attack(Dir direction)
  {
    progress = 0.0f;
    //TODO: should facing be a property just of UnitGfxEntry and not CombatUnit since it's irrelevant for the logic
    action = UnitDraw::CombatAction::ATTACK;
    unit->setFacing(direction);
  }
  
  void move(Dir direction)
  {
    if (action == UnitDraw::CombatAction::STAY)
    {
      progress = 0.0f;
      //TODO: should facing be a property just of UnitGfxEntry and not CombatUnit since it's irrelevant for the logic
      unit->setFacing(direction);
    }
    
    action = UnitDraw::CombatAction::MOVE;
    
    /* if unit wasn't moving then compute the destination position according to current position
       otherwise use last position in queue of moves */
    goal.push_back(CombatPosition((goal.empty() ? position : goal.back()).neighbour(direction), direction));
  }
};

class TileGfxEntry : public GfxEntry
{
private:
  SpriteInfo info;
  CombatCoord _position;
  u16 _animOffset;
  
public:
  TileGfxEntry(CombatView* view, SpriteInfo info, u16 x, u16 y, u16 animOffset = 0) : CombatView::GfxEntry(view, priority_tile), info(info), _position(x,y), _animOffset(animOffset) { }
  
  void draw() const override
  {
    Point coords = CombatView::coordsForTile(_position.x, _position.y);
    Gfx::drawAnimated(info, coords, _animOffset);
    Gfx::draw(TSI(COMBAT_MISC_TILES, 0, 0), coords.x, coords.y);
    
    /* these special tile hover things should have their own priority and GfxEntry */
    if (_view->getReachableTiles().contains(_position))
      Gfx::draw(TSI(COMBAT_MISC_TILES, 0, 1), coords.x, coords.y);
      
    const CombatUnit* selectedUnit = _view->getSelectedUnit();
    if (selectedUnit && selectedUnit->position == _position)
      Gfx::drawAnimated(selected_tile, coords.x, coords.y);
  }
  
  u16 x() const override { return _position.x; }
  u16 y() const override { return _position.y; }
};

class PropGfxEntry : public GfxEntry
{
private:
  static constexpr size_t MAX_SPRITES = 4;
  static constexpr s32 MAX_OFFSET_X = 5;
  static constexpr s32 MAX_OFFSET_Y = 4;
  static constexpr size_t VARIANTS_COUNT = 5;
  
  struct PropGfx
  {
    SpriteInfo gfx;
    Point offset;
  };
  
  TileProp _type;
  std::array<PropGfx, MAX_SPRITES> sprites;
  u16 _x, _y;
  
public:
  PropGfxEntry(CombatView* view, u16 x, u16 y, TileProp type, SpriteInfo info, size_t count)
  : GfxEntry(view, priority_prop), _x(x), _y(y), _type(type)
  {
    for (size_t i = 0; i < MAX_SPRITES; ++i)
      if (i < count)
      {
        sprites[i] = { info.relative(Util::randomIntUpTo(VARIANTS_COUNT)),
          Point(Util::randomIntInclusive(-MAX_OFFSET_X, MAX_OFFSET_X), Util::randomIntInclusive(-MAX_OFFSET_Y, MAX_OFFSET_Y)) };
      }
      else
        sprites[i] = { LSI_PLACEHOLD };
    
    std::sort(begin(sprites), end(sprites), [](const PropGfx& p1, const PropGfx& p2) {
      if (p1.gfx == LSI_PLACEHOLD) return false;
      else if (p1.gfx == LSI_PLACEHOLD) return true;
      else if (p1.offset.y < p2.offset.y) return true;
      else if (p1.offset.x < p2.offset.x) return true;
      else return false;
    });
  }
  
  void draw() const override
  {
    static const Point TREE_OFFSET = Point(-6, -13);
    static const Point ROCK_OFFSET = Point(-8, -11);
    
    Point coords = CombatView::coordsForTile(_x, _y) + Point(CombatView::TILE_WIDTH/2, CombatView::TILE_HEIGHT/2);

    for (size_t i = 0; i < MAX_SPRITES; ++i)
    {
      if (sprites[i].gfx != LSI_PLACEHOLD)
        Gfx::draw(sprites[i].gfx, coords + (_type == TileProp::ROCK ? ROCK_OFFSET : TREE_OFFSET) + sprites[i].offset);
    }
  }
  
  u16 x() const override { return _x; }
  u16 y() const override { return _y; }
};

class StaticGfxEntry : public GfxEntry
{
protected:
  bool animated;
  SpriteInfo info;
  u16 _x, _y;
  Point _anchor;

  void draw(SpriteInfo info) const
  {
    Point coords = CombatView::coordsForTile(_x, _y);
    
    if (animated)
      Gfx::drawAnimated(info, coords + _anchor);
    else
      Gfx::draw(info, coords + _anchor);
  }
  
public:
  StaticGfxEntry(CombatView* view, priority_t priority, SpriteInfo info, u16 x, u16 y, u16 ax, u16 ay, bool animated = false) :
  CombatView::GfxEntry(view, priority), info(info), _x(x), _y(y), _anchor(-ax, CombatView::TILE_HEIGHT/2 - info.sh() + ay), animated(animated) { }
  
  StaticGfxEntry(CombatView* view, SpriteInfo info, u16 x, u16 y, u16 ax, u16 ay, bool animated = false) : StaticGfxEntry(view, priority_static, info, x, y, ax, ay, animated) { }
  
  u16 x() const override { return _x; }
  u16 y() const override { return _y; }

  void draw() const override { draw(info); }
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
  StoneWallGfxEntry(CombatView* view, const CombatTile* tile, HouseType houseType) : StoneWallGfxEntry(view, stoneWall.find(tile->stoneWall)->second, tile, houseType) { }

  StoneWallGfxEntry(CombatView* view, const StoneWallGfxSpec& spec, const CombatTile* tile, HouseType houseType) :
  StaticGfxEntry(view, spec.priority, spec.sprite.relative(offsetForHouseType(houseType)), tile->x(), tile->y(), spec.anchor.x, spec.anchor.y), tile(tile) { }

  using StaticGfxEntry::draw;
  void draw() const override {
    draw(tile->isStoneWallDestroyed ? info.frame(1) : info);
  }
};

class NonSolidWallGfxEntry : public StaticGfxEntry
{
  const CombatTile* const tile;
  
public:
  NonSolidWallGfxEntry(CombatView* view, const CombatTile* tile, SpriteInfo info, priority_t priority, Point anchor) :
  StaticGfxEntry(view, priority, info, tile->x(), tile->y(), anchor.x, anchor.y, true), tile(tile) { }
};

class FixedGfxEntry : public GfxEntry
{
private:
  SpriteInfo info;
  u16 _x, _y;
  
public:
  FixedGfxEntry(CombatView* view, SpriteInfo info, u16 x, u16 y) : CombatView::GfxEntry(view, priority_fixed), info(info), _x(x), _y(y) { }
  
  u16 x() const override { return 0; }
  u16 y() const override { return 0; }
  
  void draw() const override
  {
    Gfx::draw(info, _x, _y);
  }
};


CombatView::CombatView(ViewManager* gvm) : View(gvm), hover(Coord(-1,-1))
{
  buttons.resize(bt_count);
  
  buttons[bt_spell] = Button::buildTristate("spell", 144, 168, LSI(COMPIX, 1), LSI(COMPIX, 23));
  buttons[bt_wait] = Button::buildTristate("wait", 144+26, 168, LSI(COMPIX, 2), LSI(COMPIX, 24));
  buttons[bt_info] = Button::buildTristate("info", 144, 168+10, LSI(COMPIX, 20), LSI(COMPIX, 25));
  buttons[bt_auto] = Button::buildTristate("auto", 144+26, 168+10, LSI(COMPIX, 4), LSI(COMPIX, 26));
  buttons[bt_flee] = Button::buildTristate("flee", 144, 168+20, LSI(COMPIX, 21), LSI(COMPIX, 27));
  buttons[bt_done] = Button::buildTristate("done", 144+26, 168+20, LSI(COMPIX, 3), LSI(COMPIX, 28));

  buttons[bt_wait]->setAction([this](){
    assert(player == combat->currentPlayer());
    selectedUnit = findNextUsableUnit();
    reachableTiles = g->combatMechanics.reachableTiles(combat, selectedUnit, selectedUnit->getProperty(Property::AVAILABLE_MOVEMENT));
  });
  
  buttons[bt_done]->setAction([this](){
    auto units = combat->friendlyUnits(player);
    std::for_each(units.begin(), units.end(), [](CombatUnit* unit) { unit->resetMoves(); });
  });
}

void CombatView::prepareGraphics()
{
  auto mp = g->combatMechanics.defaultPositionForObject(CombatObject::MAIN_BUILDING);
  this->combat->map()->placeBuilding(mp.x, mp.y, TileBuilding::FORTRESS);
  
  /*combat->map()->tileAt(6, 7)->type = 33;
  combat->map()->tileAt(6, 8)->type = 38;*/
  
  /*for (size_t i = 0; i < 16; ++i)
    combat->map()->tileAt(5, i)->type = 8+i;
  
  for (size_t i = 0; i < 8; ++i)
    combat->map()->tileAt(7, i)->type = 16+8+i;*/
  
  //this->combat->map()->placeSegment(4, 4, Dir::SOUTH_EAST, 3, combat::TileType::HILLS);
  //this->combat->map()->placeSegment(5, 6, Dir::SOUTH_WEST, 4, combat::TileType::HILLS);
  //this->combat->map()->placeSegment(3, 6, Dir::SOUTH_EAST, 3, combat::TileType::HILLS);
  

  //this->combat->map()->placeRect(6, 4, 3, 7, combat::TileType::ROUGH);
  
  setEnvironment({CombatEnvironment::Type::GRASS, Plane::ARCANUS});
  
  //this->combat->map()->placeFireWall(2, 4);
  
  this->combat->map()->placeCityRoadExit(Dir::SOUTH_EAST);
  /*this->combat->map()->placeCityRoadExit(Dir::SOUTH_WEST);
  this->combat->map()->placeCityRoadExit(Dir::NORTH_EAST);*/
  
  //this->combat->map()->placeDarknessWall(3, 6);
  //this->combat->map()->placeStoneWall(3, 6);
 
  //addGfxEntry(dummyUnit(3, 7));
  
  this->combat->deployUnits();

  for (CombatUnit* unit : combat->getUnits())
  {
    auto* gfx = new UnitGfxEntry(this, unit);
    unitsMap[unit] = gfx;
    entries.add(gfx);
  }

  //addRoads();

  
  const HouseType houseType = HouseType::TREE;
  for (u16 y = 0; y < Combat::H; ++y)
    for (u16 x = 0; x < Combat::W; ++x)
    {
      //if (x != 9 || y%2 == 0)
      {
        const CombatTile* tile = combat->tileAt(x, y);

        /* if there is a stone wall generate graphics */
        if (tile->stoneWall != WallType::NO_WALL)
        {
          //TODO: determine real house type
          entries.add(new StoneWallGfxEntry(this, tile, houseType));
        }
        
        /* if there is fire wall generate graphics */
        if (tile->fireWall != WallType::NO_WALL)
        {
          const auto& spec = nonSolidWall.find(tile->fireWall)->second;
          entries.add(new NonSolidWallGfxEntry(this, tile, spec.sprite1, spec.behind1 ? priority_fire_wall_behind : priority_fire_wall_front, spec.anchor1));

          if (spec.sprite2.isValid())
          {
            entries.add(new NonSolidWallGfxEntry(this, tile, spec.sprite2, spec.behind2 ? priority_fire_wall_behind : priority_fire_wall_front, spec.anchor2));
          }
        }
        
        /* if there is darkness wall generate graphic */
        if (tile->darknessWall != WallType::NO_WALL)
        {
          const auto& spec = nonSolidWall.find(tile->darknessWall)->second;
          entries.add(new NonSolidWallGfxEntry(this, tile, spec.sprite1.relative(14), spec.behind1 ? priority_darkness_wall_behind : priority_darkness_wall_front, spec.anchor1));

          if (spec.sprite2.isValid())
          {
            entries.add(new NonSolidWallGfxEntry(this, tile, spec.sprite2.relative(14), spec.behind2 ? priority_darkness_wall_behind : priority_darkness_wall_front, spec.anchor2));
          }
        }
        
        /* if there is road calculate tile and generate graphics */
        if (tile->road != RoadType::NONE)
        {
          /* mask is computed only on diagonal directions as roads can't go orthogonally */
          DirJoin mask = computeMask(MaskMode::DIAGONAL, tile, [](const CombatTile* tile) {return tile && tile->road != RoadType::NONE; });
          
          /* since there is no graphics for road deadends we adjust mask to be horizontal */
          if (mask == DirJoin::NW || mask == DirJoin::SE)
            mask = DirJoin::HORIZONTAL_NW_SE;
          else if (mask == DirJoin::NE || mask == DirJoin::SW)
            mask = DirJoin::HORIZONTAL_NE_SW;
          
          auto it = roadGraphics.find(mask);
          assert(it != roadGraphics.end());
          
          SpriteInfo info = (tile->road == RoadType::NORMAL ? it->second.normal : it->second.enchanted)[Util::oneOfTwoChance()];
          entries.add(new StaticGfxEntry(this, priority_roads, info, tile->x(), tile->y(), 0, TILE_HEIGHT/2, true));
        }
        
        if (tile->prop != TileProp::NONE)
        {
          size_t count = tile->prop == TileProp::TREES ? Util::randomIntInclusive(2, 3) : 1;
          SpriteInfo info = tile->prop == TileProp::ROCK ? rock : tree;
          entries.add(new PropGfxEntry(this, tile->x(), tile->y(), tile->prop, info.lbx(environmentLBX) ,count));
        }
        
        if (tile->building != TileBuilding::NONE)
        {
          const auto& spec = buildingGraphics.find(tile->building)->second;
          SpriteInfo gfx = tile->building == TileBuilding::HOUSE ? GfxData::raceHouseGfxSpec(houseType).combatHouse : spec.info;
          entries.add(new StaticGfxEntry(this, gfx.relative(Util::randomIntUpTo(spec.count)), tile->x(), tile->y(), spec.offset.x, spec.offset.y));
        }
        
        /* manage base tile graphics */
        if (tile->type == combat::TileType::ROUGH)
        {
          /* rough terrain graphics is computed through mask with the neighbours */
          DirJoin mask = computeMask(MaskMode::ALL, tile, [](const CombatTile* tile) { return !tile || tile->type == combat::TileType::ROUGH; });
          SpriteInfo info = roughTerrainMap[mask];
          entries.add(new TileGfxEntry(this, info.lbx(environmentLBX), x, y));
        }
        else if (tile->type == combat::TileType::HILLS)
        {
          /* hills terrain graphics is computed through mask with the neighbours */
          DirJoin mask = computeMask(MaskMode::DIAGONAL, tile, [](const CombatTile* tile) { return !tile || tile->type == combat::TileType::HILLS; });
          SpriteInfo info = hillsMap.find(mask)->second;
          entries.add(new TileGfxEntry(this, info.lbx(environmentLBX), x, y));
        }
        else
        {
          /* standard environment has 4 possible tiles for normal ground */
          if (environment.type != CombatEnvironment::Type::OCEAN)
            entries.add(new TileGfxEntry(this, SpriteInfo(environmentLBX, Util::randomIntUpTo(4)), x, y));
          else
          /* ocean environment graphics is special as it has just these 4 tiles */
            entries.add(new TileGfxEntry(this, SpriteInfo(environment.plane == Plane::ARCANUS ? water_tile_arcanus : water_tile_myrran).relative(Util::randomIntUpTo(4)), x, y, Util::randomIntUpTo(4)));

        }
        

      }
    }
}

void CombatView::setEnvironment(combat::CombatEnvironment env)
{
  this->environment = env;
  this->environmentLBX = environments.find(env)->second;
}

void CombatView::activate()
{
  Player* p1 = *g->getPlayers().begin();
  Player* p2 = *std::next(g->getPlayers().begin());
  
  this->combat = new Combat(*p2->getArmies().begin(), *p1->getArmies().begin(), &g->combatMechanics);
  
  selectedUnit = nullptr;
  
  prepareGraphics();
  entries.setDirty();
  
  //CursorManager::hideCursor();
}

void CombatView::deactivate()
{
  entries.clear();
  selectedUnit = nullptr;
  reachableTiles.clear();
}

void CombatView::addRoads() { entries.add(new FixedGfxEntry(this, roads_static, 16, 40)); }
void CombatView::addFlyingFortress() { entries.add(new FixedGfxEntry(this, flying_fortess, 16, 40)); }

void CombatView::addHouse(SpriteInfo info, int x, int y) { entries.add(new StaticGfxEntry(this, info, x, y, 0, 8)); }


void CombatView::draw()
{
  entries.sortIfNeeded();
  
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
  
  entries.draw();
  
  auto nend = std::remove_if(entries.begin(), entries.end(), [] (const std::unique_ptr<GfxEntry>& entry) { return entry->destroyable(); });
  entries.erase(nend, entries.end());
  
  if (hover.x != -1)
  {
    Point hoverCoords = coordsForTile(hover.x, hover.y);
    //TODO: working?
    Gfx::drawAnimated(hover_tile, hoverCoords, 0);
    
    Fonts::drawString(Fonts::format("%u,%u", hover.x, hover.y), FontFaces::Small::WHITE, 5, 5, ALIGN_LEFT);
  }

  SpriteInfo lowerBg = lower_backdrop;
  Gfx::draw(lowerBg, 0, HEIGHT - lowerBg.sh());
  
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
  
  if (hover.isValid())
  {
    const CombatUnit* unit = combat->unitAtTile(hover);
    if (unit)
      drawUnitProps(unit, false);
  }
  
  if (selectedUnit)
    drawSelectedUnitProps(selectedUnit);
}

UnitGfxEntry* CombatView::dummyUnit(s16 x, s16 y)
{
  Unit* unit = new RaceUnit(Data::unit("barbarian_spearmen")->as<RaceUnitSpec>());
  Army* army = new Army(player);
  unit->setArmy(army);
  CombatUnit* cunit = new CombatUnit(combat, Side::ATTACKER, unit);
  cunit->setPosition(x, y);
  return new UnitGfxEntry(this, cunit);
}

CombatUnit* CombatView::findNextUsableUnit()
{
  const auto& units = combat->friendlyUnits(player);
  
  if (!selectedUnit)
    return units.front();
  else
  {
    auto it = std::find(units.begin(), units.end(), selectedUnit);
    ++it;

    do
    {
      if (it == units.end())
        it = units.begin();
      
      if ((*it)->getProperty(Property::AVAILABLE_MOVEMENT) > 0)
        return *it;
      else
        ++it;
      
    } while (*it != selectedUnit);
  }
  
  return nullptr;
}

void CombatView::drawUnitProps(const CombatUnit* cunit, bool onTheLeft)
{
  const int DW = 69, DH = 40;
  const int RX = WIDTH - DW - 1;
  const int RY = 5;
  
  const Color darkBorder = Color(62,82,94);
  const Color brightBorder = Color(88,114,140);
  const Color bgColor = Color(0,0,0,80);
  
  /* draw props panel */
  Gfx::fillRect(RX+1, RY+1, DW-2, DH-2, bgColor);
  Gfx::drawLine(brightBorder, RX, RY, RX, RY+DH);
  Gfx::drawLine(brightBorder, RX, RY, RX+DW-1, RY);
  Gfx::drawLine(darkBorder, RX+DW-1, RY, RX+DW-1, RY+DH);
  Gfx::drawLine(darkBorder, RX+1, RY+DH-1, RX+DW-1, RY+DH-1);

  const CombatUnit* unit = cunit;
  const std::string unitName = unit->getUnit()->name();
  float health = unit->getUnit()->health()->percentHealth();
  MeleeInfo melee = unit->getMeleeInfo();
  RangedInfo ranged = unit->getRangedInfo(); // TODO: should use actual ranged info to retrieve real ammo
  MovementInfo movement = unit->getActualMovementInfo();
  s16 defense = unit->getProperty(Property::SHIELDS);
  s16 resistance = unit->getProperty(Property::RESIST);
  const Level* level = unit->getUnit()->getExperienceLevel();


  // TODO: on Phantom Warriors example it's aligned by 1 pixel to the left, maybe it's an original rounding issue
  Fonts::drawString(unitName, FontFaces::Tiny::GOLD_COMBAT, RX + DW/2 - 1, RY+5 - 4, ALIGN_CENTER);
  
  /* draw hits */
  const int BAR_LENGTH = 20;
  Fonts::drawString("Hits", FontFaces::Tiny::GOLD_COMBAT, RX + 2, RY+DH - 9, ALIGN_LEFT);
  UnitDraw::drawHealthBar(RX + 19, RY + DH - 6, BAR_LENGTH, health);
  Gfx::drawLine({0,0,0}, RX + 19, RY + DH - 5, RX + 19 + BAR_LENGTH , RY + DH - 5);
  
  const int propy[3] = {RY+9, RY+16, RY+23};
  
  const auto& propGfx = GfxData::propGfx();

  /* draw props */
  /* melee */
  Fonts::drawString(Fonts::format("%d", melee.strength), FontFaces::Tiny::GOLD_COMBAT, RX + 10, propy[0], ALIGN_RIGHT);
  Gfx::draw(propGfx[melee.type].blackShadow, RX + 11, propy[0]-1);
  
  /* ranged */
  if (ranged.isPresent())
  {
    Fonts::drawString(Fonts::format("%d", ranged.strength), FontFaces::Tiny::GOLD_COMBAT, RX + 10, propy[1], ALIGN_RIGHT);
    Gfx::draw(propGfx[ranged.type].blackShadow, RX + 11, propy[1]-1);
    
    /* TODO: if unit is caster it should show MP */
    if (ranged.ammo != 0)
    {
      Fonts::drawString(Fonts::format("%d", ranged.ammo), FontFaces::Tiny::GOLD_COMBAT, RX + 49, propy[2], ALIGN_RIGHT);
      Fonts::drawString("ammo", FontFaces::Tiny::GOLD_COMBAT, RX + 50, propy[2], ALIGN_LEFT);
    }
  }

  /* movement */
  Fonts::drawString(UnitDraw::stringForDoubleMovement(movement.moves, true), FontFaces::Tiny::GOLD_COMBAT, RX + 10, propy[2], ALIGN_RIGHT);
  Gfx::draw(propGfx[movement.type].blackShadow, RX + 11, propy[2]-1);
  
  /* defense */
  Fonts::drawString(Fonts::format("%d", defense), FontFaces::Tiny::GOLD_COMBAT, RX + 49, propy[0], ALIGN_RIGHT);
  Gfx::draw(propGfx[Property::SHIELDS].blackShadow, RX + 50, propy[0]-1);
  
  /* resistance */
  /* defense */
  Fonts::drawString(Fonts::format("%d", resistance), FontFaces::Tiny::GOLD_COMBAT, RX + 49, propy[1], ALIGN_RIGHT);
  Gfx::draw(propGfx[Property::RESIST].blackShadow, RX + 50, propy[1]-1);
  
  
  //TODO: check spacing
  UnitDraw::drawUnitLevel(level, RX + 48, RY + DH - 7, 5, true);
}

void CombatView::drawSelectedUnitProps(const combat::CombatUnit* unit)
{
  const auto& propGfx = GfxData::propGfx();
  
  MeleeInfo melee = unit->getMeleeInfo();
  RangedInfo ranged = unit->getRangedInfo();
  MovementInfo movement = unit->getActualMovementInfo();
  float health = unit->getUnit()->health()->percentHealth();
  
  /* melee */
  Gfx::draw(propGfx[melee.type].blueShadow, 128, 172);
  Fonts::drawString(Fonts::format("%d", melee.strength), FontFaces::Tiny::BLACK_COMBAT, 126+1, 172+1, ALIGN_RIGHT);
  
  /* ranged */
  if (ranged.isPresent())
  {
    Gfx::draw(propGfx[ranged.type].blueShadow, 128, 172+7);
    Fonts::drawString(Fonts::format("%d", ranged.strength), FontFaces::Tiny::BLACK_COMBAT, 126+1, 172+7+1, ALIGN_RIGHT);
  }

  /* movement */
  Gfx::draw(propGfx[movement.type].blueShadow, 128, 172+7*2);
  Fonts::drawString(UnitDraw::stringForDoubleMovement(movement.moves, true), FontFaces::Tiny::BLACK_COMBAT, 126+1, 172+7+7+1, ALIGN_RIGHT);

  /* health */
  Gfx::draw(hitpoints_background, 117, 194);
  UnitDraw::drawHealthBar(118, 195, 20, health);

  /* figure */
  // TODO: vertical position is not correct, probably it varies according to figure height
  Gfx::draw(GfxData::unitGfx(unit->getUnit()->spec).fullFigure.frame(2, 0), 86, 167);
}

bool CombatView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  entries.add(new ProjectileGfxEntry(this, {3,5}, {3,17}, LBXI(CMBMAGIC, 8)));
  
  //player->push(new anims::CombatProjectile({2,4}, {5,11}, LBXI(CMBMAGIC, 8), 1));
  return true;
  //player->push(new anims::SpellEffect(LSI(CMBTFX, 22), CombatCoord(hover.x,hover.y)));
  
  if (std::any_of(unitsMap.begin(), unitsMap.end(), [](const decltype(unitsMap)::value_type& entry) { return entry.second->isAnimating(); }))
    return true;
  
  CombatUnit* unit = combat->unitAtTile(hover);
  
  if (b == MouseButton::BUTTON_LEFT /* && player->isCurrentlyPlaying()*/)
  {
    Target target = player->getSpellTarget();
    
    if (unit && target == Target::FRIENDLY_UNIT && unit->getOwner() == player) // TODO: this check should be fixed by overloading operator==
      g->castSpell(unit->getUnit(), player, true);
    else if (unit && target == Target::ENEMY_UNIT && unit->getOwner() != player)
      g->castSpell(unit->getUnit(), player, true);
    else if (unit && unit->hasMoves() && unit->getOwner() == player)
    {
      if (selectedUnit != unit)
      {
        selectedUnit = unit;
        reachableTiles = g->combatMechanics.reachableTiles(combat, unit, unit->getProperty(Property::AVAILABLE_MOVEMENT));
      }
    }
    else if (selectedUnit && reachableTiles.contains(hover))
    {
      LOGD("[combat][movement] Moving unit from (%d,%d) to (%d,%d)", selectedUnit->x(), selectedUnit->y(), hover.x, hover.y);
      
      combat_moves_list path = reachableTiles.buildRoute(hover);
      
      combat->moveUnit(selectedUnit, path);
      for (auto it = path.begin(); it != path.end()-1; ++it)
        unitsMap[selectedUnit]->move(it->facing);
      
      if (selectedUnit->getAvailableMoves() == 0)
        selectedUnit = findNextUsableUnit();
        
      if (selectedUnit && selectedUnit->getAvailableMoves() > 0)
        reachableTiles = g->combatMechanics.reachableTiles(combat, selectedUnit, selectedUnit->getAvailableMoves());
      
    }
    else if (selectedUnit && unit && unit->getOwner() != selectedUnit->getOwner())
    {
      Dir relativeFacing = combat->map()->adjacentDirection(selectedUnit->tile(), unit->tile());
      
      if (relativeFacing != Dir::INVALID && g->combatMechanics.canMeleeAttack(combat, selectedUnit, unit))
      {
        LOGD("[combat][attack] Melee attack from %s at (%d,%d) to %s at (%d,%d)", selectedUnit->getUnit()->name().c_str(), selectedUnit->x(), selectedUnit->y(), unit->getUnit()->name().c_str(), unit->x(), unit->y());
        
        selectedUnit->consumeHalfMoves();
        unitsMap[selectedUnit]->attack(relativeFacing);
      }
    }

  }
  else if (b == MouseButton::BUTTON_RIGHT)
  {
    if (unit)
    {
      SDL::gvm->unitDetailView()->setUnit(unit->getUnit(), false);
      SDL::gvm->switchOverview(VIEW_UNIT);
    }
  }
  
  /*
   if (combat.selectedUnit == null)
   reachable = null;
   */
  
  return true;
}

constexpr int OX = -CombatView::TILE_WIDTH;// 0;
constexpr int OY = -CombatView::TILE_HEIGHT/2;//8;

Point CombatView::coordsForTile(u16 x, u16 y) { return Point(32*x + OX + (y % 2 == 0 ? 0 : 16), 8*y + OY); }

Coord CombatView::tileForCoords(s16 x, s16 y)
{
  //const int TH = CombatView::TILE_HEIGHT;
  //const int TW = CombatView::TILE_WIDTH;
  const int H = Combat::H;
  const int W = Combat::W;
  
  int th = 16;
  int tw = 32;
  
  Coord hover = Coord(-1,-1);
  
  if (x >= OX && x < tw*W + OX && y > OY && y < th*H + OY)
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
    
    if (hover.x < 0 || hover.y < 0 || hover.y > H-1 || hover.x > W-1 || !Combat::isValidTile(hover.x, hover.y))
      hover.x = -1;
  }
  
  return hover;
}


bool CombatView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  if (y < HEIGHT - SpriteInfo(lower_backdrop).sh())
    hover = tileForCoords(x, y);
  else
    hover.x = -1;
  
  return true;
}
