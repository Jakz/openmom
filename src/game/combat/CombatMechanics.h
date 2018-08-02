//
//  CombatMechanics.h
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMBAT_MECHANICS_H_
#define _COMBAT_MECHANICS_H_

#include "common/Common.h"
#include "Unit.h"
#include "CombatUtils.h"

enum class CombatModifier : u8;

class Game;

#include <numeric>
class unit_figure_value : public std::vector<s32>
{
public:
  unit_figure_value(size_t size) : std::vector<value_type>(size, 0) { }
  unit_figure_value(size_t size, std::function<value_type(value_type)> generator) : unit_figure_value(size)
  {
    for (s32 i = 0; i < size; ++i)
      (*this)[i] = generator(i);
  }
  
  value_type sum() { return std::accumulate(begin(), end(), 0); }
  
  void forEach(std::function<void(value_type&)> lambda) { std::for_each(begin(), end(), lambda); }

  void makeAtLeastZero() { forEach([] (value_type& v) { v = std::max(v, 0); }); }
  
  unit_figure_value operator-(const unit_figure_value& other) const
  {
    assert(size() == other.size());
    
    unit_figure_value result(*this);
    for (size_t i = 0; i < size(); ++i)
      result[i] -= other[i];
    return result;
  };
  
  unit_figure_value& operator-=(const unit_figure_value& other)
  {
    assert(size() == other.size());
    
    for (size_t i = 0; i < size(); ++i)
      (*this)[i] = other[i];
    
    return *this;
  };
  
  void generate(combat::CombatUnit* unit, std::function<value_type(combat::CombatUnit*, s32)> generator)
  {
    for (u32 i = 0; i < size(); ++i)
      (*this)[i] = generator(unit, i);
  }
};

namespace combat
{
  struct CombatTile;
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
    
    /* spells related functions */
    void castCombatInstant(const SpellCast& cast, const CombatUnit* unit);
    
    s32 computeAreaDamage(CombatUnit* target, s32 strength, School school, s32 toHit);
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
    
    AREA_DAMAGE,
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
