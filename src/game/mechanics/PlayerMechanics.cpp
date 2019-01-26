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
#include "LocalPlayer.h"

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
      up += c->getUpkeep();
  }
  
  for (auto a : player->armies)
  {
    for (auto u : *a)
    {
      up += u->upkeep();
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
    up += c->getProduction();
  
  // TODO: noble
  
  return up;
}

value_t PlayerMechanics::computeManaFromNodes(const Player *player)
{
  value_t mana = 0;
  
  for (auto node : player->nodes)
  {
    /* if node is warped then no mana is generated but instead a malus is subtracted */
    if (node->isWarped())
    {
      mana -= g->values.get<s32>(Value::WARPED_NODE_POWER_MALUS);
    }
    else
    {
      s32 nmana = node->getMana();
      
      if (player->hasMastery(node->school))
        nmana *= g->values.get<float>(Value::SCHOOL_MASTERY_MANA_NODE_MULTIPLIER);
      
      if (player->hasRetort("node_mastery"))
        nmana *= g->values.get<float>(Value::NODE_MASTERY_MANA_MULTIPLIER);
      
      nmana *= g->values.get<float>(Value::DIFFICULTY_MANA_NODE_MULTIPLIER);
    }
  }
  
  return mana;
}

value_t PlayerMechanics::computeResearchGain(const Player* player)
{
  value_t knowledge = 0;

  for (auto c : player->cities)
    knowledge += c->getKnowledge();

  for (const auto* army : player->armies)
    for (const auto* unit : army->getUnits())
      knowledge += unit->skills()->bonusForPlayerAttribute(WizardAttribute::RESEARCH);

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
    Tile* tile = g->world->get(where->getPosition());
    
    const SummonSpec* spec = spell->as<SummonSpell>()->unit;
    
    // add unit to city army or as new army
    // TODO: move units from full army to make room for summon
    if (tile->army && tile->army->size() < 9)
      tile->army->add(new FantasticUnit(spec));
    else if (!tile->army)
    {
      tile->placeArmy(new Army(player, { new FantasticUnit(spec) }));
    }
    
    // TODO: manage awareness and such things
    g->localGame->currentPlayer()->push(new anims::SummonAnimation(player->wizard, spec));
  }
  else if (spell->type == SpellType::GLOBAL || spell->type == SpellType::GLOBAL_SKILL)
  {
    g->castSpell(static_cast<const GlobalSpell*>(spell), player);
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
    value_t maxAvailableMana = player->availableMana;
    value_t actualManaCost = g->spellMechanics.actualManaCost(player, spell, false);
    
    LOGG("spell-cast","updating spell cast for '%s', castingSkill: %d, spellCost: %d, manaPool: %d", i18n::c(spell->name), maxAvailableMana, actualManaCost, player->manaPool);
    
    value_t manaAvailable = std::min(maxAvailableMana, std::min(actualManaCost, player->manaPool));

    if (player->spellBook.spendManaForCast(manaAvailable))
    {
      LOGG("spell-cast","casting spell '%s'", i18n::c(spell->name));
      castSpell(player, spell);
      //player->spellBook.cancelCast(); // TODO: if you cancel it then since it is required by mechanics (eg. unit spell) it crashes, if you don't it keeps casting it
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

value_t PlayerMechanics::computeBaseCastingSkill(const Player *player)
{
  //TODO: does additional book found after game start contribute?
  s32 base = player->spellBook.totalBooks()*2;
  
  if (player->hasRetort("archmage"))
    base += 10;
  
  return base;
}

value_t PlayerMechanics::computeBonusCastingSkill(const Player *player)
{
  value_t max = computeBaseCastingSkill(player) + player->castingSkillGained()*2;
  value_t bonus = 0;
  
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
  value_t totalBonus = player->castingSkillCounter + player->manaRatio(2);
  
  // ARCHMAGE has a 50% bonus on mana diverted on spell cast gain
  // TODO: check if correct (rounded down and so on)
  if (player->hasRetort("archmage"))
    totalBonus += std::floor(player->manaRatio(2)/2.0f);
 
  LOGG("spell-skill", "updating spell skill: updating counter from %d to %d, next increase at %d, total bonus skill: %d", player->castingSkillCounter, totalBonus, 2*player->castingSkillBase(), player->castingSkillGained_);
  
  while (totalBonus > 2*player->castingSkillBase())
  {
    totalBonus -= 2*player->castingSkillBase();
    ++player->castingSkillGained_;
  }

  player->castingSkillCounter = totalBonus;
}

