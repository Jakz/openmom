#include "PlayerMechanics.h"

#include "Player.h"
#include "City.h"
#include "Army.h"
#include "Unit.h"
#include "Places.h"
#include "SpellBook.h"
#include "World.h"
#include "Game.h"
#include "Spells.h"
#include "Tile.h"
#include "Animations.h"

void PlayerMechanics::updateGlobalGains(Player* player)
{
  Upkeep upkeep = computeUpkeep(player);
  Upkeep gain = computeGain(player);
  player->goldUpkeep = upkeep.gold;
  player->foodUpkeep = upkeep.food;
  player->manaUpkeep = upkeep.mana;

  player->goldGain = gain.gold;
  player->foodGain = gain.food;
  player->manaGain = gain.mana + computeManaFromNodes(player);
  
  player->researchGain = computeResearchGain(player);
}

void PlayerMechanics::setInitialManaRatios(Player *player)
{
  s32 total = player->manaGain;
  s32 third = total/3;
  
  s32 ratios[3] = {third, third, third};
  
  if (total % 3 == 1)
    ++ratios[0];
  else if (total % 3 == 2)
  {
    ++ratios[0];
    ++ratios[1];
  }
  
  player->setManaRatios(ratios[0], ratios[1], ratios[2]);
}

void PlayerMechanics::resetAvailableMana(Player *player)
{
  player->availableMana = player->castingSkill();
}

void PlayerMechanics::updatePools(Player *player)
{
  player->goldPool += player->goldDelta();
  player->manaPool += player->manaDelta();
}

Upkeep PlayerMechanics::computeUpkeep(const Player *player)
{
  Upkeep up;
  
  for (auto c : player->cities)
  {
    if (!c->isOutpost())
      up.add(c->getUpkeep());
  }
  
  for (auto a : player->armies)
  {
    for (auto u : *a)
    {
      up.add(u->upkeep());
    }
  }
  
  // TODO: remove mana of spells on enemy units (if there are)
  
  // TODO: remove mana from spells on cities (even enemy's)
  
  for (auto cast : player->spells)
    up.mana += cast.spell->mana.upkeep;
  
  return up;
}

Upkeep PlayerMechanics::computeGain(const Player *player)
{
  Upkeep up;
  
  for (auto c : player->cities)
  {
    up.add(c->getProduction());
  }
  
  // TODO: noble
  
  return up;
}

s32 PlayerMechanics::computeManaFromNodes(const Player *player)
{
  s32 mana = 0;
  
  for (auto node : player->nodes)
  {
    s32 nmana = node->mana;
    
    if (player->hasMastery(node->school))
      nmana *= 2;
    
    if (player->hasTrait(TraitID::NODE_MASTERY))
      nmana *= 2;
  }
  
  return mana;
}

s32 PlayerMechanics::computeResearchGain(const Player* player)
{
  s32 knowledge = 0;
  for (auto c : player->cities)
    knowledge += c->getKnowledge();
  return knowledge;
}

void PlayerMechanics::castCombatSpell(Player* player, const Spell* spell)
{
  if (spell->type == SpellType::COMBAT_ENCHANT)
  {
    //TODO player->combat->castEnchantment(SpellCast(player,spell)) TODO
    player->push(new anims::Blink(spell->school));
    player->spellBook.cancelCast();
  }
  else if (spell->target != Target::NONE)
    player->setSpellTarget(spell->target);
  
  // TODO: think about other cast types
}

void PlayerMechanics::castSpell(Player* player, const Spell* spell)
{
  if (spell->type == SpellType::SUMMON)
  {
    City* where = player->cityWithSummoningCircle();
    Tile* tile = g.world->get(where->getPosition());
    
    // add unit to city army or as new army
    // TODO: move units from full army to make room for summon
    if (tile->army && tile->army->size() < 9)
      tile->army->add(new FantasticUnit(*static_cast<const SummonSpell*>(spell)->spec));
    else if (!tile->army)
    {
      tile->placeArmy(new Army(player, {new FantasticUnit(*static_cast<const SummonSpell*>(spell)->spec)}));
    }
    
    // TODO: push summoning effect
  }
  else if (spell->type == SpellType::GLOBAL || spell->type == SpellType::GLOBAL_SKILL)
  {
    g.castSpell(static_cast<const GlobalSpell*>(spell), player);
    updateGlobalGains(player);
  }
  else if (spell->target != Target::NONE)
    player->setSpellTarget(spell->target);
  
  // TODO: manage other cases?
}

void PlayerMechanics::updateSpellCast(Player *player)
{
  const Spell* spell = player->spellBook.getCurrentCast();
  
  /* mana that the player can spend is the minimum between the mana cost of the spell, the available mana from casting skill
   * and the mana pool available
   */
  
  if (spell)
  {
    s32 manaAvailable = std::min(player->availableMana, std::min(g.spellMechanics.actualManaCost(player, spell, false), player->manaPool));

    if (player->spellBook.spendManaForCast(manaAvailable))
    {
      castSpell(player, spell);
      player->spellBook.cancelCast();
    }
    
    // remove mana spent during this turn for the spell cast
    player->manaPool -= manaAvailable;
    player->availableMana -= manaAvailable;
  }
}

void PlayerMechanics::updateSpellResearch(Player *player)
{
  player->spellBook.advanceResearch();
}

s32 PlayerMechanics::computeBaseCastingSkill(const Player *player)
{
  s32 base = player->spellBook.totalBooks()*2;
  
  if (player->hasTrait(TraitID::ARCHMAGE))
    base += 10;
  
  return base;
}

s32 PlayerMechanics::computeBonusCastingSkill(const Player *player)
{
  s32 max = computeBaseCastingSkill(player) + player->castingSkillGained()*2;
  s32 bonus = 0;
  
  City* city = player->cityWithFortress();
  if (city)
  {
    // for each army in player city with Mage Fortress
    // TODO: finish
    for (auto a : player->armies)
    {
      if (a->getPosition().same(city->getPosition()))
      {
        for (auto u : *a)
        {
          // if u is hero and has caster
          //bonus += caster mana / 2;
        }
      }
    }
  }
  
  return std::min(bonus, max);
}

void PlayerMechanics::updateBonusCastingSkill(Player *player)
{
  s32 totalBonus = player->castingSkillCounter + player->manaRatio(2);
  
  // ARCHMAGE has a 50% bonus on mana diverted on spell cast gain
  // TODO: check if correct (rounded down and so on)
  if (player->hasTrait(TraitID::ARCHMAGE))
    totalBonus += std::floor(player->manaRatio(2)/2.0f);
  
  while (totalBonus > 2*player->castingSkillBase())
  {
    totalBonus -= 2*player->castingSkillBase();
    ++player->castingSkillGained_;
  }

  player->castingSkillCounter = totalBonus;
}

