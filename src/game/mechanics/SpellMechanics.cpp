#include "SpellMechanics.h"

#include "Spells.h"
#include "SpellBook.h"
#include "Player.h"
#include "Tile.h"
#include "Unit.h"
#include "Game.h"
#include "World.h"

#include <list>

using namespace std;

bool SpellMechanics::isGlobalAllowed(const Player *player, const Spell *spell)
{
  auto spells = player->getSpells();
  auto it = find_if(spells.begin(), spells.end(), [&](const SpellCast& cast) { return cast.spell == spell; });
  return it == spells.end();
}

bool SpellMechanics::isCityAllowed(const Player *player, const Spell *spell, const City *city)
{
  // TODO
  return true;
}

bool SpellMechanics::isTileAllowed(const Player *player, const Spell *spell, const Tile *tile)
{
  if (!player->fog()->get(tile->position))
    return false;
  
  if (spell == Spells::CORRUPTION)
  {
    if (tile->type == TileType::OCEAN || tile->node())
      return false;
  }
  else if (spell == Spells::CHANGE_TERRAIN)
  {
    if (tile->node()) return false;
    
    switch (tile->type) {
      case TileType::DESERT:
      case TileType::FOREST:
      case TileType::HILL:
      case TileType::SWAMP:
      case TileType::GRASS:
      case TileType::VOLCANO:
      case TileType::MOUNTAIN:
        return true;
      default:
        return false;
    }
  }
  else if (spell == Spells::RAISE_VOLCANO)
  {
    return !tile->node() && tile->type != TileType::HILL && tile->type != TileType::MOUNTAIN && tile->type != TileType::RIVER;
  }
  
  return true;
}

bool SpellMechanics::isUnitAllowed(const Player *player, const UnitSpell *spell, const Unit *unit)
{
  return !unit->skills()->hasSpell(spell);
}

bool SpellMechanics::applyTileSpell(const SpellCast& cast, Tile *tile)
{
  if (cast.spell == Spells::CHANGE_TERRAIN)
  {
    TileType newType;
    
    switch (tile->type) {
      case TileType::DESERT:
      case TileType::FOREST:
      case TileType::HILL:
      case TileType::SWAMP:
        newType = TileType::GRASS; break;
      case TileType::GRASS:
        newType = TileType::FOREST; break;
      case TileType::VOLCANO:
        newType = TileType::MOUNTAIN; break;
      case TileType::MOUNTAIN:
        newType = TileType::HILL; break;
      default:
        newType = TileType::GRASS;
    }
    
    tile->type = newType;
    
    g->world->calcSubTile(tile->position.x, tile->position.y, tile->position.plane);
  }
  else if (cast.spell == Spells::RAISE_VOLCANO)
  {
    tile->resource = Resource::NONE;
    tile->type = TileType::VOLCANO;
  }
  else
  {
    tile->addSpell(cast);
  }
  
  return true;
}

s32 SpellMechanics::actualManaCost(Player *player, const Spell *spell, bool combat)
{
  return combat ? spell->mana.combatManaCost : spell->mana.manaCost;
}

s32 SpellMechanics::actualResearchGain(const Player *player, const Spell *spell)
{
  s32 research = player->baseResearchPoints();
  
  //TODO: check how bonus is calculated, ceil floor?
  // 20% bonus by Sage Master
  if (player->hasTrait(TraitID::SAGE_MASTER))
    research += std::floor(research * 0.20f);
  
  if (spell)
  {
    // 25% bonus by Conjurer if Summoning spell
    if (player->hasTrait(TraitID::CONJURER) && spell->kind == SpellKind::SUMMONING)
      research += std::floor(research * 0.25f);
    
    // 15% for misc School mastery if spell of same school
    if (player->hasMastery(spell->school))
      research += std::floor(research * 0.15f);
    
    // 10% of discount for each book starting from 8 books
    int bookCount = player->book()->booksForSchool(spell->school);
    if (bookCount >= 8)
      research += std::floor(research * (0.10f * bookCount - 7));
  }
  
  return research;
}

bool SpellMechanics::willDispel(const SpellCast &cast, const SpellCast &dispelCast)
{
  s32 dispelMana = dispelCast.spell->mana.combatManaCost + dispelCast.extraMana;
  
  // TODO: only combat mana cost of spell to dispel is considered, should we distinguish
  // ever lasting
  float chance = dispelMana / (dispelMana + cast.spell->mana.combatManaCost);
  
  // TODO: are traits like ARCHMAGE + NATURE MASTERY cumulative?
  if (cast.player->hasTrait(TraitID::ARCHMAGE) || cast.player->hasTrait(TraitID::RUNEMASTER))
    chance /= 2.0f;
  
  if (cast.player->hasMastery(cast.spell->school))
    chance /= 2.0f;
  
  return Util::chance(chance);
}



