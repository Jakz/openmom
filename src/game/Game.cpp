//
//  Game.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Game.h"

#include "World.h"
#include "LocalPlayer.h"

#include "Army.h"
#include "Unit.h"

#include "Messages.h"

#include "Animations.h"
#include "GfxData.h"

using namespace std;

Game::Game() : world(new World(this, 60,40)), values(this), mapMechanics(this), lairMechanics(this), spellMechanics(this), eventMechanics(this), cityMechanics(this), playerMechanics(this), combatMechanics(this), firstTurn(true), turnCounter(0)
{
  
}

void Game::init()
{
  for (auto* c : cities)
    cityMechanics.updateValues(c);
  
  for (auto* p : players)
  {
    playerMechanics.updateGlobalGains(p);
    playerMechanics.setInitialManaRatios(p);
    playerMechanics.resetAvailableMana(p);
  }
  
  // Relations.initRelations blabla
}

void Game::dummyInit()
{
  LocalPlayer *player = new LocalPlayer(this, "Kali", Data::wizard(KALI), GREEN, Race::race(RaceID::BARBARIANS), 60, 40);
  LocalPlayer *player2 = new LocalPlayer(this, "Antani", Data::wizard(MERLIN), BLUE, Race::race(RaceID::BARBARIANS), 60, 40);

  
  Army* army2 = new Army(player2, {
		new FantasticUnit(UnitSpec::summonSpec(UnitID::MAGIC_SPIRIT)),
    new RaceUnit(UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BARBARIANS)),
  });
  
  player2->add(army2);
  placeArmy(army2, Position(2, 2, ARCANUS));
  
  players.push_back(player);
  players.push_back(player2);
  current = players.begin();
  
  world->fill(TILE_WATER, ARCANUS);
  world->fill(TILE_WATER, MYRRAN);
  world->rect(TILE_GRASS, 3, 3, 10, 10, ARCANUS);
  world->rect(TILE_MOUNTAIN, 4, 4, 2, 2, ARCANUS);
  world->line(TILE_HILL, 4, 8, 11, true, ARCANUS);
  world->rect(TILE_GRASS, 1, 1, 6, 6, MYRRAN);
  world->line(TILE_MOUNTAIN, 6, 8, 5, false, MYRRAN);
  world->line(TILE_HILL, 5, 8, 6, true, MYRRAN);
  world->line(TILE_WATER, 2, 6, 6, true, ARCANUS);
  world->set(TILE_WATER, 4, 7, ARCANUS);
  world->set(TILE_HILL, 4,4, ARCANUS);
  world->set(TILE_DESERT, 8,8, ARCANUS);
  world->set(TILE_DESERT, 7,8, ARCANUS);
  world->set(TILE_DESERT, 9,8, ARCANUS);
  world->set(TILE_DESERT, 8,7, ARCANUS);
  world->set(TILE_DESERT, 8,9, ARCANUS);
  world->set(TILE_DESERT, 7,9, ARCANUS);
  world->set(TILE_DESERT, 9,9, ARCANUS);
  world->set(TILE_DESERT, 10,9, ARCANUS);
  
  world->get(7, 8, ARCANUS)->placeManaNode(new ManaNode(School::SORCERY, 0));
    
  //WorldGenerator.generate(world, ARCANUS);
  //WorldGenerator.generate(world, MYRRAN);

  Army* a = new Army(player, {
		new FantasticUnit(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)),
    new RaceUnit(UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BARBARIANS)),
		new RaceUnit(UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS)),
		new RaceUnit(UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BARBARIANS)),
		new RaceUnit(UnitSpec::raceSpec(UnitID::CAVALRY, RaceID::BARBARIANS)),
    new FantasticUnit(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)),
		new RaceUnit(UnitSpec::raceSpec(UnitID::BERSERKERS, RaceID::BARBARIANS)),
  });
  
  SpellCast cast = SpellCast(player, Spells::BLESS);
  a->get(0)->skills()->add(cast);
  
  //map.get(5, 5, ARCANUS).placeManaNode(mapMechanics.generateManaNode(world, 5, 5, ARCANUS));
  //this.meldNode(a, 5, 5, ARCANUS);
  //map.get(5, 6, ARCANUS).placeManaNode(new ManaNode());
  //map.get(6, 5, ARCANUS).placeManaNode(new ManaNode());
  //map.get(6, 6, ARCANUS).placeManaNode(new ManaNode());
  world->get(4, 3, ARCANUS)->placeResource(Resource::COAL);
  world->get(3, 4, ARCANUS)->placeResource(Resource::WILD_GAME);
  //map.rect(TILE_GRASS, 2, 2, 6, 3);
  //map.rect(TILE_GRASS, 8, 4, 1, 1);
  world->get(3,4,ARCANUS)->placeRoad(true);
  world->get(3,5,ARCANUS)->placeRoad(true);
  world->get(4,5,ARCANUS)->placeRoad(true);
  world->get(5,5,ARCANUS)->placeRoad(true);

  world->set(TILE_VOLCANO, 0, 0, ARCANUS);
  world->set(TILE_VOLCANO, 59, 0, ARCANUS);
  world->set(TILE_VOLCANO, 0, 39, ARCANUS);
  world->set(TILE_VOLCANO, 59, 39, ARCANUS);
  world->set(TILE_HILL, 1, 2, MYRRAN);
  
  player->fog()->setRect(0, 0, 60, 40, ARCANUS);
  player->fog()->setRect(0, 0, 60, 40, MYRRAN);
  player->setViewport(5, 5);
  
  player->book()->discoverSpell(Spells::BLESS);
  player->book()->discoverSpell(Spells::CORRUPTION);
  player->book()->discoverSpell(Spells::CHANGE_TERRAIN);
  player->book()->discoverSpell(Spells::RAISE_VOLCANO);
  player->book()->discoverSpell(Spells::GUARDIAN_SPIRIT);
  
  new LocalGame(this);
  
  placeArmy(a, Position(5, 6, ARCANUS));
  placeArmy(new Army(player, {new FantasticUnit(UnitSpec::summonSpec(UnitID::MAGIC_SPIRIT)),new FantasticUnit(UnitSpec::summonSpec(UnitID::GREAT_DRAKE))}), Position(4, 3, ARCANUS));
  placeArmy(new Army(player, {new FantasticUnit(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)), new Hero(UnitSpec::heroSpec(UnitID::HERO_DWARF))}), Position(4, 8, ARCANUS));
  
  City* florence = new City(player, "Florence", 4000, Position(3, 3, ARCANUS));
  florence->addBuilding(Building::MAGE_FORTRESS);
  florence->addBuilding(Building::SMITHY);
  florence->addBuilding(Building::BUILDERS_HALL);
  florence->addBuilding(Building::BARRACKS);
  florence->addBuilding(Building::FIGHTERS_GUILD);
  florence->addBuilding(Building::SUMMONING_CIRCLE);
  florence->addBuilding(Building::ALCHEMISTS_GUILD);
  settleCity(florence);
  
  world->get(7,6,ARCANUS)->placePlace(new Place(PLACE_TOWER_OF_WIZARDRY, false));
  world->get(8,6,ARCANUS)->placePlace(new Place(PLACE_CAVE, false));
  
  
  world->calcSubTiles();
  world->updateRoads();
  
  init();
}

void Game::placeArmy(Army* army, const Position& position)
{
  world->get(position)->placeArmy(army);
  army->getOwner()->fog()->setRange(position, army->sightRange());
}

void Game::computeRoute(Army *army, const Position goal)
{
  world->pathfinder.computeRoute(world, army, goal);
}

bool Game::settleCity(Army* army, const std::string name)
{
  for (auto u : *army)
  {
    if (u->skills()->hasSimpleEffect(SimpleEffect::Type::CREATE_OUTPOST))
    {
      army->remove(u);
      break;
    }
  }
  
  settleCity(army->getOwner(), name, Position(army->getPosition()));
  return true;
}

void Game::settleCity(Player* player, const std::string name, const Position& position)
{
  settleCity(player, name, cityMechanics.computeInitialPopulation(player,position), position);
}

void Game::settleCity(Player* player, const std::string name, u16 population, const Position& position)
{
  settleCity(new City(player, name, population, position));
}

void Game::settleCity(City* city)
{
  world->get(city->getPosition())->settleCity(city);
  cities.push_back(city);
  city->getOwner()->fog()->setRange(city->getPosition(), city->sightRange());
}

bool Game::meldNode(const Army* army, const Position& position)
{
  Tile* t = world->get(position);
  t->node->owner = army->getOwner();
  army->getOwner()->add(t->node);
  return true;
}

bool Game::startCast(Player* player, const Spell* spell)
{
  player->book()->startCast(spell);
  
  s32 manaCost = spellMechanics.actualManaCost(player, spell, false);
  if (player->getAvailableMana() >= manaCost)
    playerMechanics.updateSpellCast(player);
  
  return true;
}

void Game::castSpell(Unit* unit, const Spell* spell, const Player* player)
{
  unit->skills()->add(SpellCast(player, spell));
}


bool Game::castSpell(Unit* unit, Player* player, bool combat)
{
  SpellCast cast = SpellCast(player, player->book()->getCurrentCast());
  
  if (spellMechanics.isUnitAllowed(player, static_cast<const UnitSpell*>(cast.spell), unit))
  {
    unit->skills()->add(cast);
    
    if (!combat)
    {
      //TODO player->push(new SpellEffect(SpellEffect.schoolsEffects[cast.spell.props.school.ordinal()],new Position(unit.army.x,unit.army.y)));
      cancelCast(player);
      playerMechanics.updateGlobalGains(player);
    }
    else
    {
      /*TODO int y = caster.combat.unitPosition(unit);
      int x = y >> 16;
      y &= 0x0000FFFF;
      caster.push(new SpellEffect(SpellEffect.schoolsEffects[cast.spell.props.school.ordinal()],CombatView.tileX(x,y),CombatView.tileY(x,y))); */
      cancelCast(player);
    }
    
    return true;
  }
  else
  {
    player->send(new msgs::Error("You may not cast this spell on the specified unit.")); // TODO localize and check text
    return false;
  }
}


void Game::castSpell(City* city, const Spell* spell, const Player* player)
{
  city->addSpell(SpellCast(player, spell));
}

void Game::castCombatspell(Player* player, const Spell* spell)
{
  player->combatCast(spell);
}

bool Game::castSpell(City* city, Player* player)
{
  SpellCast cast = SpellCast(player, player->book()->getCurrentCast());
  
  if (spellMechanics.isCityAllowed(player, cast.spell, city))
  {
    city->addSpell(cast);
    cancelCast(player);
    // TODO	caster.push(new SpellEffect(SpellEffect.schoolsEffects[cast.spell.props.school.ordinal()],new Position(city.x,city.y)));
    playerMechanics.updateGlobalGains(player);
    
    return true;
  }
  else
  {
    player->send(new msgs::Message("You may not cast this spell on the specified tile.")); // TODO localize and check text
    return false;
  }
}

bool Game::castSpell(const GlobalSpell* spell, Player* player)
{
  if (!player->hasSpell(spell))
  {
    player->add(SpellCast(player,spell));
    cancelCast(player);
  }
  
  return true;
}

bool Game::castSpell(Tile* tile, Player* player)
{
  SpellCast cast = SpellCast(player, player->book()->getCurrentCast());
  
  if (spellMechanics.isTileAllowed(player, cast.spell, tile))
  {
    spellMechanics.applyTileSpell(cast, tile);
    cancelCast(player);
    //TODO: we'll need to find a way to manage everything seamlessly for local players and other kind of players such that all players which have a viewport view the animaton
    localGame->currentPlayer()->push(new anims::SpellEffect(localGame->currentPlayer(), GfxData::specialSpellGfxEffect(cast.spell), tile->position));
    playerMechanics.updateGlobalGains(player);
    
    return true;
  }
  else
  {
    player->send(new msgs::Message("You may not cast this spell on the specified tile.")); // TODO localize and check text
    return false;
  }
}

void Game::dispelCast(City* city, const SpellCast& cast)
{
  // TODO: update layout city if spell added building
  city->removeSpell(cast);
  playerMechanics.updateGlobalGains(city->getOwner());
}

void Game::cancelCast(Player* player)
{
  player->book()->cancelCast();
  player->setSpellTarget(Target::NONE);
}

void Game::addSkill(Unit* unit, const Skill* skill)
{
  unit->skills()->add(skill);
}

bool Game::consumeMovement(Army* army)
{
  army->getRoute()->consumeMovement(world);
  
  if (army->getRoute()->completed())
    army->setRoute(nullptr);
  
  return true;
}

bool Game::revalidateRoute(Army* army)
{
  if (!army->getRoute()) return true;
  else return army->getRoute()->stillValid(world);
}

const vector<const Productable*> Game::availableUnits(const City* city)
{
  list<const RaceUnitSpec*> units = cityMechanics.availableUnits(city);
  vector<const Productable*> runits;
  for (auto r : units) runits.push_back(r);
  
  sort(runits.begin(), runits.end(), Building::ComparatorByName());

  return runits;
}

bool Game::alchemy(Player* player, s32 amount, bool fromGold)
{
  s32 newAmount = amount;
  
  if (!player->hasTrait(TraitID::ALCHEMY))
    newAmount /= 2;
  
  if (fromGold)
    player->alchemy(-amount, newAmount);
  else
    player->alchemy(newAmount, -amount);
  
  return true;
}

void Game::turnBeginning()
{
  // TODO: should be client side, not here!
  // TODO if (currentPlayer()->book()->getCurrentResearch() == null)
  //  GameViewManager.instance.switchView(ViewType.RESEARCH);
  
  // TODO: if values change, these are not reflected in manaRatios of player
  Player* current = currentPlayer();
  
  LOGG("turn", "turn %zu start for ", turnCounter, current->name.c_str())
  
  current->updateCities();
  
  playerMechanics.updatePools(current);
  playerMechanics.updateGlobalGains(current);
  
  playerMechanics.updateBonusCastingSkill(current);
  playerMechanics.resetAvailableMana(current);
  
  playerMechanics.updateSpellCast(current);
  playerMechanics.updateSpellResearch(current);
  
  current->turnBeginArmies();
  // TODO: probably this following method can be merged with previous one
  current->refreshArmies();
  
  current->growCities();
}

void Game::nextTurn()
{
  ++current;
  
  if (current == players.end())
  {
    firstTurn = false;
    ++turnCounter;
    current = players.begin();
  }
  
  if (!firstTurn)
    turnBeginning();
  
  localGame->switchToPlayer(*current);
}


#include "ViewManager.h"
#include "SDLHelper.h"

LocalGame* LocalGame::i = nullptr;

LocalGame::LocalGame(Game* game) : game(game)
{
  game->localGame = this;
  for (Player* p : game->players)
    this->players.push_back(static_cast<LocalPlayer*>(p));
  
  current = players.begin();
  LocalGame::i = this;
}

void LocalGame::switchToPlayer(Player* player)
{
  current = std::find(players.begin(), players.end(), player);
  SDL::gvm->setPlayer(*current);
}
