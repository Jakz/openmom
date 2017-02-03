//
//  CombatMechanics.h
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMBAT_MECHANICS_H_
#define _COMBAT_MECHANICS_H_

#include "Common.h"
#include "Unit.h"
#include "CombatUtils.h"

enum class CombatModifier : u8;

class Game;

namespace combat
{
  class CombatTile;
  class CombatMap;
  struct CombatEnvironment;
  enum class CombatObject;
  
  class CombatMechanics
  {
  private:
    Game* const game;
  public:
    CombatMechanics(Game* game) : game(game) { }
    
    /* attack related functions */
    bool canMeleeAttack(const Combat* combat, const CombatUnit* attacker, const CombatUnit* defender) { return true; }
    bool canRangedAttack(const Combat* combat, const CombatUnit* attacker, const CombatUnit* defender) { return true; }
    
    /* map related functions */
    u16 movementCostForTile(const CombatUnit* unit, const CombatTile* tile, Dir from);
    CombatPosition positionForDeployedUnit(CombatMap* map, const CombatUnit* unit, u16 index);
    CombatCoord defaultPositionForObject(CombatObject object);
    bool isTileBlocked(const CombatTile* tile, const CombatUnit* unit);
    combat_pathfind_info reachableTiles(const Combat* combat, const CombatUnit* unit, s16 movement);
    
    
  };

  
  class Damage
  {
  public:
    const enum class Type {
      SINGLE,
      EACH_SAME,
      EACH_DIFFERENT,
      EACH_FATAL
    } type;
    Damage(Type type) : type(type) { }
    virtual void apply(Unit* unit) const = 0;
  };
  
  class DamageSingle : public Damage
  {
    const s16 amount;
  public:
    DamageSingle(s16 amount) : Damage(Type::SINGLE), amount(amount) { }
    void apply(Unit* unit) const override
    {
      unit->health()->applyDamage(amount);
    }
  };
  
  class DamageEachSame : public Damage
  {
    const s16 amount;
  public:
    DamageEachSame(s16 amount) : Damage(Type::EACH_SAME), amount(amount) { }
    void apply(Unit* unit) const override
    {
      unit->health()->applySameDamageToEachFigure(amount);
    }
  };
  
  class DamageEachDifferent : public Damage
  {
    const hit_points amounts;
  public:
    DamageEachDifferent(hit_points&& amounts) : Damage(Type::EACH_SAME), amounts(amounts) { }
    void apply(Unit* unit) const override
    {
      unit->health()->applyDifferentDamageToEachFigure(amounts);
    }
  };
  
  class DamageEachFatal : public Damage
  {
    const unit_figure_flag amounts;
  public:
    DamageEachFatal(unit_figure_flag&& amounts) : Damage(Type::EACH_FATAL), amounts(amounts) { }
    void apply(Unit* unit) const override
    {
      unit->health()->killFigures(amounts);
    }
  };

  
  
  enum class DamageType : u8
  {
    RANGED_MAGICAL_CHAOS,
    RANGED_MAGICAL_NATURE,
    RANGED_MAGICAL_LIFE,
    RANGED_MAGICAL_SORCERY,
    RANGED_MAGICAL_DEATH,
    
    RANGED_BOULDER,
    RANGED_NORMAL,
    
    THROWN,
    
    PHYSICAL_MELEE_ATTACK,
    PHYSICAL_MAGICAL_ATTACK,
    
    AREA_IMMOLATION,
    AREA_BLIZZARD
  };
  
  enum class AttackType
  {
    THROWN
  };
  
  enum class AttackPriority : s8
  {
    GAZE_DEFENDER = 0,
    THROWN_ATTACK = 0,
    BREATH_ATTACK = 0,
    
    GAZE_ATTACKER = 1,
    
    FIRST_STRIKE_ATTACK = 3,
    
    MELEE_ATTACK = 4,
    TOUCH_ATTACK = 4,
    
    NOT_AVAILABLE = -1
  };
  
  struct Attack
  {
    const CombatUnit* attacker;
    const CombatUnit* defender;
    
    bool isCounter;
    
    AttackType type;
    s16 strength;
    
    virtual void resolve();
  };
}

#endif
