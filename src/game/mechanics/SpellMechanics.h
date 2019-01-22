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
enum class SpellRarity : u32;

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
  
  value_t actualManaCost(Player* player, const Spell* spell, bool combat);
  value_t actualResearchGain(const Player* player, const Spell* spell);
  
  float computeDispelChance(const SpellCast& cast, const SpellCast& dispelCast, float dispelMultiplier);
  
  count_t guaranteedSpellAmountForRarity(SpellRarity rarity, School school, count_t books);
  spell_rarity_map<s32> guaranteedSpells(School school, u32 books);
  
  count_t researchableSpellAmountForRarity(SpellRarity rarity, School school, count_t books);
};

#endif
