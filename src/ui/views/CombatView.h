//
//  CombatView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMBAT_VIEW_H_
#define _COMBAT_VIEW_H_

#include "View.h"
#include "CombatMap.h"
#include "DrawQueue.h"

#include <vector>
#include <memory>
#include <cmath>

class ViewManager;

//TODO: required for dummyUnit()
class UnitGfxEntry;

using priority_t = s16;

class CombatView : public View
{
public:
  class GfxEntry;

private:
  struct entry_comparator
  {
    bool operator()(const std::unique_ptr<CombatView::GfxEntry>& e1, const std::unique_ptr<CombatView::GfxEntry>& e2) const
    {
      if (e1->priority() < 0 && e2->priority() >= 0)
        return true;
      else if (e1->priority() >= 0 && e2->priority() < 0)
        return false;
      else if (e1->priority() < 0 && e2->priority() < 0)
        return e1->priority() < e2->priority();
      else if (e1 == e2)
        return false;
      
      if (e1->x() != e2->x())
      {
        /* this special condition is needed because of how coordinates are used
         in the isometric map
         */
        if (abs(e1->x() - e2->x()) == 1)
          return e1->y() < e2->y();
        return e1->x() < e2->x();
      }
      else if (e1->y() != e2->y())
        return e1->y() < e2->y();
      else
      {
        assert(e1->priority() != e2->priority());
        return e1->priority() < e2->priority();
      }
    }
  };
  
public:
  class GfxEntry : DrawElement<GfxEntry, entry_comparator>
  {
    priority_t _priority;
  protected:
    CombatView* const _view;
    GfxEntry(CombatView* view, priority_t priority) : _view(view), _priority(priority) { }
  public:
    priority_t priority() const { return _priority; }
    void setPriority(priority_t priority) { _priority = priority; }
    virtual u16 x() const = 0;
    virtual u16 y() const = 0;

    virtual void draw() const = 0;
  };

private:
  DrawQueue<GfxEntry, entry_comparator> entries;
  std::unordered_map<const combat::CombatUnit*, UnitGfxEntry*> unitsMap;
  
  enum
  {
    bt_spell,
    bt_wait,
    bt_info,
    bt_auto,
    bt_flee,
    bt_done,
    
    bt_count
  };
  
  enum class SubState
  {
    COMBAT,
    SPELL_CAST
  };
  
  void draw() override;
  void drawPost() override { }
  
  Coord hover;
  
  combat::Combat* combat;
  
  void drawUnitProps(const combat::CombatUnit* unit, bool onTheLeft);
  void drawSelectedUnitProps(const combat::CombatUnit* unit);
  
  void addRoads();
  void addFlyingFortress();
  void addHouse(SpriteInfo info, int x, int y);
  void prepareGraphics();
  
  UnitGfxEntry* dummyUnit(s16 x, s16 y);
  
  combat::CombatEnvironment environment;
  LBXID environmentLBX;
  
  combat::combat_pathfind_info reachableTiles;
  combat::CombatUnit* selectedUnit;
  
  combat::CombatUnit* findNextUsableUnit();
    
public:
  CombatView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override;
  
  void setEnvironment(combat::CombatEnvironment env);
  
  //Point coordsForTile(u16 x, u16 y);

  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  void mouseMoved(u16 x, u16 y, MouseButton b) override;
  
  
  static Point coordsForTile(u16 x, u16 y);
  static Coord tileForCoords(s16 x, s16 y);

  
  constexpr static int TILE_WIDTH = 32;
  constexpr static int TILE_HEIGHT = 16;
  
  const decltype(reachableTiles)& getReachableTiles() { return reachableTiles; }
  const combat::CombatUnit* getSelectedUnit() { return selectedUnit; }

};

#endif 
