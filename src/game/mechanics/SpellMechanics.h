#ifndef _SPELL_MECHANICS_H_
#define _SPELL_MECHANICS_H_

#include "common/Common.h"

class Player;
class Spell;
class Tile;
class Unit;
class City;
class SpellCast;
class VariableSpellCast;
class Game;
class UnitSpell;

class SpellMechanics
{
private:
  Game* const g;
  
public:
  SpellMechanics(Game* game) : g(game) { }
  
  bool isGlobalAllowed(const Player* player, const Spell* spell);
  bool isTileAllowed(const Player* player, const Spell* spell, const Tile* tile);
  bool isCityAllowed(const Player* player, const Spell* spell, const City* city);
  bool isUnitAllowed(const Player* player, const UnitSpell* spell, const Unit* unit);
  
  bool applyTileSpell(const SpellCast& cast, Tile* tile);
  
  s32 actualManaCost(Player* player, const Spell* spell, bool combat);
  s32 actualResearchGain(const Player* player, const Spell* spell);
  
  bool willDispel(const SpellCast& cast, const SpellCast& dispelCast);
};

#endif
