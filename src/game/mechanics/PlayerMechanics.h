#ifndef _PLAYER_MECHANICS_
#define _PLAYER_MECHANICS_

#include "Common.h"

class Game;
class Player;
class Spell;

class PlayerMechanics
{
private:
  Game &g;
  
public:
  PlayerMechanics(Game& game) : g(game) { }
  
  Upkeep computeGain(const Player* player);
  Upkeep computeUpkeep(const Player* player);
  
  s32 computeResearchGain(const Player* player);

  
  s32 computeBaseCastingSkill(const Player* player);
  s32 computeBonusCastingSkill(const Player* player);
  void updateBonusCastingSkill(Player* player);
  
  s32 computeManaFromNodes(const Player* player);
  
  void castSpell(Player* player, const Spell* spell);
  void castCombatSpell(Player* player, const Spell* spell);
  
  void updateSpellCast(Player* player);
  void updateSpellResearch(Player* player);


  
  void updateGlobalGains(Player* player);
  
  void setInitialManaRatios(Player* player);
  void resetAvailableMana(Player* player);
  void updatePools(Player* player);
};

#endif