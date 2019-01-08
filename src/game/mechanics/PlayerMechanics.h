#ifndef _PLAYER_MECHANICS_
#define _PLAYER_MECHANICS_

#include "common/Common.h"

class Game;
class Player;
class Spell;

class PlayerMechanics
{
private:
  Game* const g;
  
public:
  PlayerMechanics(Game* game) : g(game) { }
  
  Upkeep computeGain(const Player* player);
  Upkeep computeUpkeep(const Player* player);
  
  value_t computeResearchGain(const Player* player);

  
  value_t computeBaseCastingSkill(const Player* player);
  value_t computeBonusCastingSkill(const Player* player);
  void updateBonusCastingSkill(Player* player);
  
  value_t computeManaFromNodes(const Player* player);
  
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
