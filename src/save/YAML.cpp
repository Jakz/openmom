#include "YAML.h"

#include <iostream>

#include "yaml-cpp/yaml.h"
#include "Platform.h"

#include "Data.h"
#include "GfxData.h"
#include "Localization.h"

#include "Level.h"
#include "UnitSpec.h"
#include "Spells.h"
#include "Skill.h"
#include "Effects.h"
#include "Items.h"

#define PARSE_ERROR(x, ...) do { LOGD("[yaml] parse error: " x, __VA_ARGS__); } while (false)

#define FETCH_OR_FAIL(name, map, n) do { \
  auto it = map.find(n.asString()); \
  if (it != map.end()) return it->second; \
  PARSE_ERROR("mapped value '%s' not found for %s", n.asString().c_str(), name); \
  assert(false); \
  } while(false)

#define YAML_ASSERT_OR_RETURN_NULL(c, x, ...) do { if (!(c)) { PARSE_ERROR(x, __VA_ARGS__); return nullptr; } } while (false)

#define YAML_ASSERT(c, x, ...) do { if (!c) { PARSE_ERROR(x, __VA_ARGS__); } } while (false)

using namespace YAML;
using N = yaml::N;

static std::string currentFile = "";
static std::string currentEntry = "";

class yaml::Node : public YAML::Node
{
  
public:
  Node(const YAML::Node& node) : YAML::Node(node) { }
  
  template<typename T> void keyNotFoundError(const T& key) const;
  
  template<typename T> bool hasChild(const T& key) const { return YAML::Node::operator[](key).IsDefined(); }
  
  template<typename T> const YAML::Node getWithoutCheck(const T& key) const
  {
    return YAML::Node::operator[](key);
  }
  
  template<typename T> const
  YAML::Node operator[](const T& key) const
  {
    auto node = YAML::Node::operator[](key);
    
    if (node.IsDefined())
      return node;
    else
    {
      keyNotFoundError(key);
      assert(false);
    }
  }
  
  bool isIntegral() const
  {
    std::string v = asString();
    return std::all_of(v.begin(), v.end(), isdigit);
  }
};

template<> void yaml::Node::keyNotFoundError(const int& key) const
{
  PARSE_ERROR("index '%d' not found while parsing %s from %s", key, currentEntry.c_str(), currentFile.c_str());
}

template<typename T> void yaml::Node::keyNotFoundError(const T& key) const
{
  PARSE_ERROR("key '%s' not found while parsing %s from %s", key, currentEntry.c_str(), currentFile.c_str());
}

using NNN = YAML::Node;

bool operator==(const N& n, const std::string& string) { return n.Scalar() == string; }

N yaml::parse(const std::string& fileName)
{
  currentFile = fileName;
  return N(YAML::LoadFile(yamlPath(fileName)));
}

Path yaml::yamlPath(const std::string& fileName)
{
  Path path = Platform::instance()->getResourcePath() + "/data/yaml/" + fileName;
  
  if (!path.exists())
  {
    LOGD("lbx file at %s not found", path.c_str());
    assert(false);
  }

  return path;
}

template<> s32 yaml::parse(const N& node) { return node.as<s32>(); }
template<> s16 yaml::parse(const N& node) { return node.operator s16(); }
template<> u16 yaml::parse(const N& node) { return node.operator u16(); }
template<> float yaml::parse(const N& node) { return node.as<float>(); }
template<> bool yaml::parse(const N& node) { return node.as<bool>(); }
template<> std::string yaml::parse(const N& node) { return node.as<std::string>(); }


template<typename T, typename std::enable_if<!std::is_pointer<T>::value, int>::type> T yaml::parse(const N& node)
{
  T value;
  parse(node, value);
  return value;
}

/*template<typename T, typename std::enable_if<!std::is_pointer<T>::value, int>::type> T yaml::parse(const N& node)
{
  return node.as<T>();
}*/

template<> LBXID yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, LBXID> mapping = {
    { "backgrnd", LBXID::BACKGRND },
    
    { "figures1", LBXID::FIGURES1 },
    { "figures3", LBXID::FIGURES3 },
    { "figures4", LBXID::FIGURES4 },
    { "figures8", LBXID::FIGURES8 },
    { "figures9", LBXID::FIGURES9 },
    { "figure10", LBXID::FIGURE10 },
    { "figure11", LBXID::FIGURE11 },
    { "figure12", LBXID::FIGURE12 },
    
    { "lilwiz", LBXID::LILWIZ },
    { "magic", LBXID::MAGIC },
    { "monster", LBXID::MONSTER },
    { "moodwiz", LBXID::MOODWIZ },
    { "portrait", LBXID::PORTRAIT },
    
    { "special", LBXID::SPECIAL },
    { "special2", LBXID::SPECIAL2 },
    { "spellscr", LBXID::SPELLSCR },
    
    { "units1", LBXID::UNITS1 },
    { "units2", LBXID::UNITS2 },
    
    { "wizards", LBXID::WIZARDS },
    { "wizlab", LBXID::WIZLAB },

  };
  
  FETCH_OR_FAIL("LBXID", mapping, node);
}

//  enum class Class : u8 { MELEE = 0, RANGED, MELEE_STAFF, STAFF_WAND, ARMOR, MISC };

template<> items::Class yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, items::Class> mapping = {
    { "melee", items::Class::MELEE },
    { "ranged", items::Class::RANGED },
    { "melee_staff", items::Class::MELEE_STAFF},
    { "staff_wand", items::Class::STAFF_WAND },
    { "armor", items::Class::ARMOR },
    { "jewelry", items::Class::MISC }
  };

  FETCH_OR_FAIL("items::Class", mapping, node);
}

template<> SpellRarity yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, SpellRarity> mapping = {
    { "common", SpellRarity::COMMON },
    { "uncommon", SpellRarity::UNCOMMON },
    { "rare", SpellRarity::RARE },
    { "very_rare", SpellRarity::VERY_RARE },
  };
  
  FETCH_OR_FAIL("SpellRarity", mapping, node);
}

template<> SpellKind yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, SpellKind> mapping = {
    { "city_enchantment", SpellKind::CITY },
    { "unit_enchantment", SpellKind::UNIT_SPELL },
    { "summoning", SpellKind::SUMMONING }
  };
  
  FETCH_OR_FAIL("SpellKind", mapping, node);
}

template<> Target yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, Target> mapping = {
    { "friendly_city", Target::FRIENDLY_CITY },
    { "friendly_unit", Target::FRIENDLY_UNIT },
    { "friendly_army", Target::FRIENDLY_ARMY },
    
    { "enemy_city", Target::ENEMY_CITY },
    { "enemy_unit", Target::ENEMY_UNIT },
    { "enemy_army", Target::ENEMY_ARMY },
    { "enemy_unit_spell", Target::ENEMY_UNIT_SPELL },
    
    { "both_armies", Target::BOTH_ARMIES },
    { "map_tile", Target::MAP_TILE },
    { "viewport", Target::VIEWPORT },
    
    { "global", Target::GLOBAL },
    { "none", Target::NONE },
  };
  
  FETCH_OR_FAIL("SpellTarget", mapping, node);
}


template<> HouseType yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, HouseType> mapping = {
    { "mud", HouseType::MUD },
    { "tree", HouseType::TREE },
    { "normal", HouseType::NORMAL }
  };
  
  FETCH_OR_FAIL("HouseType", mapping, node);
}


template<> School yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, School> mapping = {
    { "arcane", School::ARCANE },
    { "nature", School::NATURE },
    { "sorcery", School::SORCERY },
    { "chaos", School::CHAOS },
    { "life", School::LIFE },
    { "death", School::DEATH }
  };
  
  FETCH_OR_FAIL("School", mapping, node);
}

template<> Ranged yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, Ranged> mapping = {
    { "none", Ranged::NONE },
    { "rock", Ranged::ROCK },
    { "arrow", Ranged::ARROW },
    { "bullet", Ranged::BULLET },
    { "chaos", Ranged::CHAOS },
    { "nature", Ranged::NATURE },
    { "sorcery", Ranged::SORCERY },
    { "life", Ranged::LIFE },
    { "death", Ranged::DEATH }
  };
  
  FETCH_OR_FAIL("Ranged", mapping, node);
}

template<> Property yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, Property> mapping = {
    { "to_hit", Property::TO_HIT },
    { "melee", Property::MELEE },
    { "defense", Property::SHIELDS },
    { "defense_ranged", Property::SHIELDS_RANGED },
    { "defense_chaos", Property::SHIELDS_CHAOS },
    { "defense_sorcery", Property::SHIELDS_SORCERY },
    { "defense_nature", Property::SHIELDS_NATURE },
    { "defense_life", Property::SHIELDS_LIFE },
    { "defense_death", Property::SHIELDS_DEATH },
    { "resistance", Property::RESIST },
    { "resist_chaos", Property::RESIST_CHAOS },
    { "resist_nature", Property::RESIST_NATURE },
    { "resist_death", Property::RESIST_DEATH },
    { "hits", Property::HIT_POINTS },
    { "health_regen", Property::HEALTH_REGEN },
    { "figures", Property::FIGURES },
    { "sight", Property::SIGHT }
    
  };
  
  FETCH_OR_FAIL("Property", mapping, node);
}

template<> CombatBonus::Phase yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, CombatBonus::Phase> mapping = {
    { "attacker", CombatBonus::Phase::ATTACKER },
    { "defender", CombatBonus::Phase::DEFENDER },
    { "always", CombatBonus::Phase::ALWAYS }
  };
 
  FETCH_OR_FAIL("CombatBonus::Phase", mapping, node);
}

template<> PropertyBonus::Mode yaml::parse(const N& node)
{
  using Mode = PropertyBonus::Mode;
  static const std::unordered_map<std::string, Mode> mapping = {
    { "additive", Mode::ADDITIVE },
    { "override", Mode::OVERRIDE },
    { "override_if_greater", Mode::OVERRIDE_IF_GREATER },
    { "override_if_lesser", Mode::OVERRIDE_IF_LESSER }

  };
  
  FETCH_OR_FAIL("SkillEffectGroup::Mode", mapping, node);
}

template<> RangedInfo yaml::parse(const N& node)
{
  if (node == "none")
    return RangedInfo();
  else
  {
    assert(node.IsSequence() && node.size() == 3);
    s16 strength = node[0];
    Ranged type = parse<Ranged>(node[1]);
    s16 ammo = node[2];
    
    return RangedInfo(type, strength, ammo);
  }
}

template<> skills::Type yaml::parse(const N& node)
{
  if (node == "native")
    return skills::Type::NATIVE;
  else if (node == "hero")
    return skills::Type::HERO;
  else if (node == "spell")
    return skills::Type::SPELL;
  else
    assert(false);
}

template<> school_value_map yaml::parse(const N& node)
{
  school_value_map values = school_value_map(0);
  
  assert(node.IsSequence() && node.size() % 2 == 0);
  for (size_t i = 0; i < node.size(); i += 2)
  {
    School school = parse<School>(node[i]);
    s16 value = parse<s16>(node[i+1]);
    values.set(school, value);
  }
  
  return values;
}

template<typename T> void yaml::parse(const N& node, std::vector<T>& dest)
{
  assert(node.IsSequence());
  //dest.reserve(node.size());
  for (const auto& entry : node)
    dest.push_back(parse<T>(entry));
}

template<typename T> T yaml::optionalParse(const N& node, const char* key, T def)
{
  N n = node.getWithoutCheck(key);
  return n.IsDefined() ? parse<T>(n) : def;
}

template<> void yaml::parse(const N& node, SpriteInfo& v)
{
  if (node.asString() == "none")
    v = LSI_PLACEHOLD;
  else
  {
    assert(node.IsSequence());
    if (node[0] == "lbx")
      v = SpriteInfo(parse<LBXID>(node[1]), node[2]);
    else
      assert(false);
  }
}

template<> Upkeep yaml::parse(const N& node)
{
  if (node.asString() == "none")
    return Upkeep();
  else
  {
    if (node.IsSequence())
    {
      if (node.size() == 1)
        return Upkeep(node[0]);
      else if (node.size() == 2)
        return Upkeep(node[0], node[1]);
      else if (node.size() == 3)
        return Upkeep(node[0], node[1], node[2]);
      else
        assert(false);
    }
    else if (node.isIntegral())
    {
      return Upkeep(node.as<int>());
    }
    else
      assert(false);
  }
}

#pragma mark Spell
template<> const Spell* yaml::parse(const N& node)
{
  assert(node.IsMap());
  
  I18 name = i18n::keyForString(N(node["visuals"])["i18n"]);
  
  SpellKind kind = parse<SpellKind>(node["kind"]);
  SpellRarity rarity = parse<SpellRarity>(node["rarity"]);
  School school = parse<School>(node["school"]);
  
  s16 manaCost = parse<s16>(node["mana_cost"]);
  s16 combatManaCost = optionalParse<s16>(node, "combat_mana_cost", -1);
  s16 researchCost = parse<s16>(node["research_cost"]);
  s16 upkeep = optionalParse<s16>(node, "upkeep", -1);
  
  switch (kind)
  {
    case SpellKind::SUMMONING:
    {
      const std::string unitIdentifier = node["unit"];
      const SummonSpec* spec = Data::unit(unitIdentifier)->as<SummonSpec>();
      YAML_ASSERT_OR_RETURN_NULL(spec, "SummonSpec '%s' not found", unitIdentifier.c_str());
      return new SummonSpell(name, rarity, school, researchCost, manaCost, combatManaCost, spec);
    }
      
    case SpellKind::UNIT_SPELL:
    {
      const std::string skillIdentifier = node["skill"];
      const Skill* skill = Data::skill(skillIdentifier);
      YAML_ASSERT_OR_RETURN_NULL(skill, "Skill '%s' not found", skillIdentifier.c_str());
      return new UnitSpell(name, rarity, school, researchCost, manaCost, combatManaCost, upkeep, skill);
    }
      
    case SpellKind::CITY:
    {
      //SpellDuration duration = parse<SpellDuration>(node["duration"]);
      Target target = parse<Target>(node["target"]);
      YAML_ASSERT_OR_RETURN_NULL(target == Target::ENEMY_CITY || target == Target::FRIENDLY_CITY, "CitySpell target must be a city, found '%s'", node["target"].asString().c_str());
      return new CitySpell(name, rarity, school, SpellDuration::CONTINUOUS, target, researchCost, manaCost, combatManaCost, upkeep);
    }
      
    default:
      break;
  }
  
  return nullptr;
}

#pragma mark SkillEffectGroup

// TODO: it's here because it's not needed by the game but for consistency
// maybe we should move it to Data
static std::unordered_map<std::string, const SkillEffectGroup*> skillGroups;

template<> SkillEffectGroup::Mode yaml::parse(const N& node)
{
  using Mode = SkillEffectGroup::Mode;
  static const std::unordered_map<std::string, Mode> mapping = {
    { "keep_greater", SkillEffectGroup::Mode::KEEP_GREATER },
    { "unique", SkillEffectGroup::Mode::UNIQUE }
  };
  
  FETCH_OR_FAIL("SkillEffectGroup::Mode", mapping, node);
}
                                          
template<> const SkillEffectGroup* yaml::parse(const N& node)
{
  //const std::string& identifier = node["identifier"];
  SkillEffectGroup::Mode mode = parse<SkillEffectGroup::Mode>(node["policy"]);
  return new SkillEffectGroup(mode);
}

#pragma mark SkillEffect
template<> const SkillEffect* yaml::parse(const N& node)
{
  const std::string& type = node["type"];
  SkillEffect* effect = nullptr;
  
  if (type == "unit_bonus")
  {
    Property property = parse<Property>(node["property"]);
    s16 value = parse<s16>(node["value"]);
    PropertyBonus::Mode mode = optionalParse(node, "mode", PropertyBonus::Mode::ADDITIVE);
    effect = new UnitBonus(property, mode, value);
  }
  else if (type == "unit_level_bonus")
  {
    Property property = parse<Property>(node["property"]);
    float multiplier = parse<float>(node["multiplier"]);
    effect = new UnitLevelBonus(property, multiplier);
  }
  else if (type == "army_bonus")
  {
    Property property = parse<Property>(node["property"]);
    s16 value = parse<s16>(node["value"]);
    PropertyBonus::Mode mode = optionalParse(node, "mode", PropertyBonus::Mode::ADDITIVE);

    effect = new ArmyBonus(property, mode, value, ArmyBonus::Type::WHOLE_ARMY);
  }
  else if (type == "combat_bonus")
  {
    Property property = parse<Property>(node["property"]);
    s16 value = parse<s16>(node["value"]);
    CombatBonus::Phase owner = optionalParse(node, "owner_phase", CombatBonus::Phase::ALWAYS);
    CombatBonus::Phase other = optionalParse(node, "enemy_phase", CombatBonus::Phase::ALWAYS);
    
    effect = new CombatBonus(property, value, owner, other, false);
  }
  else if (type == "special_attack")
  {
    static std::unordered_map<std::string, SpecialAttackType> mapping = {
      { "thrown_weapon", SpecialAttackType::THROWN_ATTACK },
      { "fire_breath", SpecialAttackType::FIRE_BREATH },
      { "poison_touch", SpecialAttackType::POISON_TOUCH }
    };
    
    if (mapping.find(node["kind"]) == mapping.end())
      assert(false);
    
    s16 value = parse<s16>(node["value"]);
    SpecialAttackType kind = mapping[node["kind"]];
    
    effect = new SpecialAttackEffect(kind, value);
  }
  else if (type == "ability" || type == "parametric_ability")
  {
    static std::unordered_map<std::string, SimpleEffect::Type> mapping = {
      { "first_strike", SimpleEffect::Type::FIRST_STRIKE },
      { "healer", SimpleEffect::Type::HEALER },
      { "purify", SimpleEffect::Type::PURIFY },
      { "create_outpost", SimpleEffect::Type::CREATE_OUTPOST },
      { "create_road", SimpleEffect::Type::CREATE_ROAD },
      { "meld_with_node", SimpleEffect::Type::MELD_NODE },
      { "wall_crusher", SimpleEffect::Type::WALL_CRUSHING }
    };
    
    if (mapping.find(node["kind"]) == mapping.end())
    {
      PARSE_ERROR("unknown effect type '%s'", node["kind"].asString().c_str());
      assert(false);
    }
    
    SimpleEffect::Type kind = mapping[node["kind"]];
    
    if (type == "parametric_ability")
    {
      s16 value = parse<s16>(node["value"]);
      effect = new SimpleParametricEffect(SkillEffect::Type::ABILITY, kind, value);
    }
    else
      effect = new SimpleEffect(SkillEffect::Type::ABILITY, kind);
  }
  else if (type == "movement")
  {
    static std::unordered_map<std::string, MovementType> mapping = {
      { "mountaineer", MovementType::MOUNTAINWALK },
      { "flying", MovementType::FLYING },
      { "swimming", MovementType::SWIMMING },
      { "non_corporeal", MovementType::NON_CORPOREAL }
    };
    
    if (mapping.find(node["kind"]) == mapping.end())
      assert(false);
    
    MovementType kind = mapping[node["kind"]];
    
    effect = new MovementEffect(kind);
  }
  else if (type == "spell_grant")
  {
    std::string spellName = node["spell"];
    //TODO: can't be mapped now because spells are not loaded yet
    u16 times = parse<u16>(node["times"]);
    s16 strength = optionalParse(node, "strength", 0);
    
    effect = new SpellGrantEffect(nullptr, times, strength);
  }
  else
  {
    PARSE_ERROR("ability type '%s' not found", type.c_str());
    assert(false);
    return nullptr;
  }
  
  /* parse stackable group if present */
  {
    bool hasStackableGroup = node.hasChild("group");
    
    if (hasStackableGroup)
    {
      std::string groupIdentifier = node["group"];
      const auto it = skillGroups.find(groupIdentifier);
      
      if (it == skillGroups.end())
      {
        PARSE_ERROR("skill group '%s' is not defined", node["group"].asString().c_str());
        assert(false);
      }
      
      effect->setGroup(it->second);
    }
  }

  return effect;
}

#pragma mark Skill
template<> const Skill* yaml::parse(const N& node)
{
  skills::Type type = optionalParse(node, "type", skills::Type::NATIVE);
  
  std::vector<const SkillEffect*> effects;
  parse(node["effects"], effects);
  
  auto visuals = node["visuals"];
  
  skills::VisualInfo visualInfo;
  visualInfo.hidden = optionalParse(visuals, "hidden", false);
  if (!visualInfo.hidden)
  {
    visualInfo.name = i18n::keyForString(visuals["i18n"]);
    visualInfo.hideValue =  optionalParse(visuals, "hide_value", false);
    visualInfo.icon = parse<SpriteInfo>(visuals["icon"]);
  }
  
  return new skills::ConcreteSkill(type, effect_list(effects), visualInfo);
}

template<> void yaml::parse(const N& node, skill_init_list& skills)
{
  assert(node.IsSequence());
  std::transform(node.begin(), node.end(), std::back_inserter(skills), [](const std::string& identifier) {
    return Data::skill(identifier);
  });
}

#pragma mark UnitSpec
template<> std::pair<const UnitSpec*, UnitGfxSpec> yaml::parse(const N& node)
{
  assert(node.IsMap());
  
  const std::string& type = node["type"];
  
  /* common properties */
  s16 melee = node["melee"];
  s16 defense = node["defense"];
  s16 resistance = node["resistance"];
  s16 hits = node["hits"];
  s16 figures = node["figures"];

  s16 movement = node["movement"];
  s16 sight = node["sight"];
  s16 upkeep = node["upkeep"];
  s16 cost = node["cost"];
  RangedInfo ranged = parse<RangedInfo>(node["ranged"]);
  skill_init_list skills = optionalParse<skill_init_list>(node, "skills", skill_init_list());

  const N visuals = node["visuals"];
  SpriteInfo gfxIcon = parse<SpriteInfo>(visuals["icon"]);
  SpriteInfo gfxFigure = parse<SpriteInfo>(visuals["figure"]);
  I18 gfxName = i18n::keyForString(visuals["i18n"]);
  bool gfxIsFlying = optionalParse(visuals, "is_flying", false);
  
  
  if (figures != 1 && figures != 2 && figures != 4 && figures != 6 && figures != 8)
    assert(false);
  
  std::pair<const UnitSpec*, UnitGfxSpec> data = std::make_pair(nullptr, UnitGfxSpec(gfxName, gfxIcon, gfxFigure, gfxIsFlying));
  
  /* create racial unit spec */
  if (type == "racial")
  {
    const Race* race = Data::get<const Race*>(node["race"]);
    
    data.first = new RaceUnitSpec(
                            race,
                            upkeep,
                            cost,
                            melee,
                            ranged,
                            defense,
                            resistance,
                            hits,
                            figures,
                            movement,
                            sight,
                            skills // TODO
    );
  }
  else if (type == "hero")
  {
    s32 requiredFame = node["required_fame"];
    
    const N yslots = node["slots"];
    using iclass = items::Class;
    items::Slots slots = items::Slots({parse<iclass>(yslots[0]), parse<iclass>(yslots[1]), parse<iclass>(yslots[2])});

    data.first = new HeroSpec(
                        HeroType::HERO,
                        requiredFame,
                        slots,
                        upkeep,
                        cost,
                        melee,
                        ranged,
                        defense,
                        resistance,
                        hits,
                        figures,
                        movement,
                        sight,
                        skills // TODO
    );
    
    data.second.hero.portrait = parse<SpriteInfo>(visuals["portrait"]);
  }
  else if (type == "fantastic")
  {
    School school = parse<School>(node["school"]);

    data.first = new SummonSpec(
                                school,
                                upkeep,
                                cost,
                                melee,
                                ranged,
                                defense,
                                resistance,
                                hits,
                                figures,
                                movement,
                                sight,
                                skills // TODO
    );
    
    data.second.fantastic.summonFigure = parse<SpriteInfo>(visuals["summon"]);
  }
  
  const N buildingsRequired = node.getWithoutCheck("requires");
  if (buildingsRequired.IsDefined())
  {
    assert(buildingsRequired.IsSequence());
    for (const std::string& ybuilding : buildingsRequired)
    {
      const Building* building = Data::building(ybuilding);
      Data::unitDependsOnBuilding.insert(std::make_pair(data.first, building));
    }
  }
  
  return data;
}

#pragma mark Race
template<> std::pair<const Race*, RaceGfxSpec> yaml::parse(const N& node)
{
  assert(node.IsMap());

  std::pair<const Race*, RaceGfxSpec> data;
  
  s16 cityGrowthModifier = node["city_growth_modifier"];
  float outpostGrowthChance = node["outpost_growth_chance"];
  s16 baseProduction = node["base_production"];
  s16 foodProductionPerFramer = node["food_production_per_farmer"];
  float taxIncomeMultiplier = node["tax_income_multiplier"];
  float miningBonusMultiplier = node["mining_bonus_multiplier"];
  float manaProducedPerCitizen = node["mana_produced_per_citizen"];
  
  const Node& ycityNames = node["city_names"];
  assert(ycityNames.IsSequence());
  
  std::vector<std::string> cityNames;
  for (size_t i = 0; i < ycityNames.size(); ++i)
    cityNames.push_back(ycityNames[i].asString());
  
  data.first = new Race(cityGrowthModifier, outpostGrowthChance, baseProduction, taxIncomeMultiplier, miningBonusMultiplier, manaProducedPerCitizen, foodProductionPerFramer, cityNames);
  
  /* parse graphics */
  data.second.houseType = parse<HouseType>(node["visuals"]["house_type"]);
  data.second.cityWorker = parse<SpriteInfo>(node["visuals"]["graphics"]["worker"]);
  data.second.cityFarmer = parse<SpriteInfo>(node["visuals"]["graphics"]["farmer"]);
  data.second.cityRebel = parse<SpriteInfo>(node["visuals"]["graphics"]["rebel"]);
  data.second.name = i18n::keyForString(node["visuals"]["i18n"]);
  data.second.unitName = i18n::keyForString(node["visuals"]["i18n_unit"]);
  
  return data;
}

#pragma mark Wizard
template<> std::pair<const Wizard*, WizardGfxSpec> yaml::parse(const N& node)
{
  assert(node.IsMap());
  
  
  // TODO: don't set fields, use constructor
  std::pair<const Wizard*, WizardGfxSpec> data;
  
  Wizard* wizard = new Wizard();
  wizard->defaultBooks = parse<school_value_map>(node["default_books"]);
  
  if (node.hasChild("default_retorts"))
  {
    std::vector<std::string> retortNames;
    parse(node["default_retorts"], retortNames);
    std::transform(
      retortNames.begin(),
      retortNames.end(),
      std::inserter(wizard->defaultRetorts, wizard->defaultRetorts.begin()),
      [](const std::string& retortName) { return Data::retort(retortName); }
    );
  }
  
  data.first = wizard;
  
  /* visual */
  {
    const Node& visual = node["visuals"];
    auto& gfx = data.second;
    
    I18 name = i18n::keyForString(visual["i18n"]);
    SpriteInfo portraitSmall = parse<SpriteInfo>(visual["portrait_small"]);
    SpriteInfo portraitLarge = parse<SpriteInfo>(visual["portrait_large"]);
    SpriteInfo diplomacyMood = parse<SpriteInfo>(visual["diplomacy_mood"]);
    SpriteInfo gemmedPortrait = parse<SpriteInfo>(visual["gemmed_portrait"]);
    SpriteInfo researchPose = parse<SpriteInfo>(visual["research_pose"]);
    SpriteInfo summonPose = parse<SpriteInfo>(visual["summon_pose"]);
    
    gfx.name = name;
    gfx.portraitSmall = portraitSmall;
    gfx.portraitLarge = portraitLarge;
    gfx.diplomacyMood = diplomacyMood;
    gfx.gemmedPortrait = gemmedPortrait;
    gfx.researchPose = researchPose;
    gfx.summonPose = summonPose;
  }
  
  return data;
}

#pragma mark Retort
template<> Retort::Requirement::Type yaml::parse(const N& node)
{
  if (node == "any_school")
    return Retort::Requirement::Type::ANY_SCHOOL_AT_LEAST;
  else if (node == "specific_school")
    return Retort::Requirement::Type::SPECIFIC_SCHOOL_AT_LEAST;
  else
    assert(false);
}

template<> Retort::Requirement yaml::parse(const N& node)
{
  using Type = Retort::Requirement::Type;
  
  Type type = parse<Type>(node["type"]);
  School school = School::NO_SCHOOL;
  u8 count = parse<u16>(node["count"]);
  u8 times = optionalParse(node, "times", 1);
  
  if (type == Type::SPECIFIC_SCHOOL_AT_LEAST)
    school = parse<School>(node["school"]);
  
  assert((school == School::NO_SCHOOL) ^ (type == Type::SPECIFIC_SCHOOL_AT_LEAST));
  
  return { type, school, count, times };
}

template<> const Retort* yaml::parse(const N& node)
{
  assert(node.IsMap());

  //TODO: ugly, should not set fields directly but just use constructors
  
  std::string identifier = node["identifier"];
  u16 cost = node["cost"];
  I18 i18n = i18n::keyForString(node["i18n"]);
  
  Retort* retort = new Retort(identifier, cost);
  retort->i18n = i18n;
  
  if (node.hasChild("requirements"))
    parse(node["requirements"], retort->requirements);
  
  return retort;
}

void yaml::parseLevels()
{
  N file = parse("levels.yaml");
  auto node = file["levels"];
  assert(node.IsMap());
  
  for (size_t i = 0; i < 2; ++i)
  {
    const N levels = i == 0 ? node["units"] : node["heroes"];
    experience_levels& dest = i == 0 ? Data::normalUnitLevels : Data::heroLevels;
    
    assert(levels.IsSequence());
    dest.resize(levels.size());
  
    const Level* next = nullptr;
    for (ssize_t i = levels.size() - 1; i >= 0; --i)
    {
      const N node = levels[i];
      
      //const std::string& identifier = node["identifier"];
      const s32 xp = optionalParse(node,"experience", 0);
      bool canAdvanceToNext = optionalParse(node, "can_advance_to_next", true);
      
      SpriteInfo icon = parse<SpriteInfo>(node["visuals"]["icon"]);
      I18 name = i18n::keyForString(node["visuals"]["i18n"]);
      
      const Level* level = new Level(i, name, icon, next, xp, level_bonuses(), canAdvanceToNext);
      dest[i] = std::unique_ptr<const Level>(level);
      next = level;
    }
  }
}

const std::string& yaml::getIdentifier(const N& node, const char* key)
{
  if (!node.hasChild(key))
  {
    PARSE_ERROR("Identifier field not found for entry in file %s", currentFile.c_str());
    assert(false);
  }
  
  const std::string& identifier = node[key];
  currentEntry = identifier;
  return identifier;
}

void yaml::parseSkills()
{
  N file = parse("skills.yaml");
  
  auto groups = file["groups"];
  for (const auto& ygroup : groups)
  {
    const std::string& identifier = getIdentifier(ygroup);
    skillGroups[identifier] = parse<const SkillEffectGroup*>(ygroup);
  }
  
  auto skills = file["skills"];
  for (const auto& yskill : skills)
  {
    const std::string& identifier = getIdentifier(yskill);
    const Skill* skill = parse<const Skill*>(yskill);
    Data::registerData(identifier, skill);
  }
}

void yaml::parseSpells()
{
  N file = parse("spells.yaml");
  auto spells = file["spells"];
  
  for (const auto& yspell : spells)
  {
    const std::string& identifier = getIdentifier(yspell);
    const Spell* spell = parse<const Spell*>(yspell);
    Data::registerData(identifier, spell);
  }
}

void yaml::parseUnits()
{
  N file = parse("units.yaml");
  auto units = file["units"];
  
  for (const auto& yunit : units)
  {
    const std::string& identifier = getIdentifier(yunit);
    const std::pair<const UnitSpec*, UnitGfxSpec> unit = parse<std::pair<const UnitSpec*, UnitGfxSpec>>(yunit);
    Data::registerData(identifier, unit.first);
    GfxData::registerData(unit.first, unit.second);
  }
}

void yaml::parseWizards()
{
  N file = parse("wizards.yaml");
  
  auto retorts = file["retorts"];
  for (const auto& yretort : retorts)
  {
    const std::string& identifier = getIdentifier(yretort);
    const Retort* retort = parse<const Retort*>(yretort);
    Data::registerData(identifier, retort);
  }
  
  auto wizards = file["wizards"];
  for (const auto& ywizard : wizards)
  {
    const std::string& identifier = getIdentifier(ywizard);
    const std::pair<const Wizard*, WizardGfxSpec> pair = parse<std::pair<const Wizard*, WizardGfxSpec>>(ywizard);
    const Wizard* wizard = pair.first;
    const WizardGfxSpec& gfx = pair.second;
    Data::registerData(identifier, wizard);
    GfxData::registerData(wizard, gfx);
  }
  
  
}

void yaml::parseRaces()
{
  N file = parse("races.yaml");
  auto races = file["races"];
  
  for (const auto& yrace : races)
  {
    const std::string& identifier = getIdentifier(yrace);
    const std::pair<const Race*, RaceGfxSpec> race = parse<std::pair<const Race*, RaceGfxSpec>>(yrace);
    Data::registerData(identifier, race.first);
    GfxData::registerData(race.first, race.second);
  }
}

void yaml::parseLocalization()
{
  N file = parse("english.yaml");
  auto entries = file["entries"];
  
  for (const auto entry : entries)
  {
    const std::string& key = entry.first;
    const std::string& value = entry.second;
    i18n::mapCustomEntry(key, value);
  }
}

void yaml::parse()
{
  parseLocalization();
  parseRaces();
  parseSkills();
  parseLevels();
  parseUnits();
  parseSpells();
  parseWizards();
  
  const auto i18missing = i18n::unlocalizedEntries();
  for (const auto& entry : i18missing)
    LOGD("[i18n] Missing localized entry: %s", entry.c_str());
}
