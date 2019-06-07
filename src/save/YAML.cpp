#include "YAML.h"

#include <iostream>

#include "yaml-cpp/yaml.h"
#include "Platform.h"

#include "Data.h"
#include "GfxData.h"
#include "Texture.h"
#include "Localization.h"
#include "Help.h"

#include "Wizard.h"
#include "Level.h"
#include "UnitSpec.h"
#include "Spells.h"
#include "Skill.h"
#include "City.h"
#include "Effects.h"
#include "Items.h"

#include <fstream>

//TODO: no idea why symbol is not found in lib by linker
#if defined(WIN32) || (defined(__unix__) && !defined(__APPLE__))
std::string YAML::detail::node_data::empty_scalar;
#endif

#define PARSE_ERROR(x, ...) do { LOGD("[yaml] parse error: " x, __VA_ARGS__); } while (false)

#define FETCH_OR_FAIL(name, map, n) do { \
  auto it = map.find(n.asString()); \
  if (it != map.end()) return it->second; \
  PARSE_ERROR("mapped value '%s' not found for %s", n.asString().c_str(), name); \
  assert(false); \
  } while(false)

#define YAML_ASSERT_OR_RETURN_NULL(c, x, ...) do { if (!(c)) { PARSE_ERROR(x, __VA_ARGS__); return nullptr; } } while (false)

#define YAML_ASSERT(c, x, ...) do { if (!(c)) { PARSE_ERROR(x, __VA_ARGS__); } } while (false)

using namespace YAML;
using N = yaml::N;
template<typename T> using string_map = std::unordered_map<std::string, T>;

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

  operator SpriteInfo() const { return yaml::parse<SpriteInfo>(*this); }

  //TODO: this is istantiated for all char[4], char[8], char[11]...
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
      return YAML::Node();
    }
  }
  
  bool isIntegral() const
  {
    std::string v = asString();
    return std::all_of(v.begin(), v.end(), isdigit);
  }

#if defined(DEBUG)
  void debugPrint(size_t i) const
  {
    std::string v = asString();
    printf("%s\n", v.c_str());
    
    for (const auto& child : *this)
    {
      Node(child).debugPrint(i + 2);
    }
  }
#endif  
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
bool operator!=(const N& n, const std::string& string) { return n.Scalar() != string; }


N yaml::parse(const std::string& fileName)
{
  currentFile = fileName;
  std::string path = yamlPath(fileName);

  std::string content = file_handle(path, file_mode::READING).toString();

  LOGD("[yaml] loaded %s, size: %zu bytes", fileName.c_str(), content.length());

#if __cpp_exceptions == 199711
  try
  {
#endif
    Node node = YAML::Load(content.c_str());
    return N(node);
#if __cpp_exceptions == 199711
  }
  catch (YAML::ParserException e) { PARSE_ERROR("parse exception: %s", e.what()); }
  return N(YAML::Node());
#endif
}

Path yaml::yamlPath(const std::string& fileName)
{
  Path path = Platform::instance()->getResourcePath() + "/data/yaml/" + fileName;

#ifdef _WIN32
  path = path.convertSeparatorToWindows();
#endif

  
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

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4715)
#endif

template<> LBXID yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, LBXID> mapping = {
    { "backgrnd", LBXID::BACKGRND },
    { "cityscap", LBXID::CITYSCAP },
    
    { "diplomac", LBXID::DIPLOMAC },
    
    { "figures1", LBXID::FIGURES1 },
    { "figures3", LBXID::FIGURES3 },
    { "figures4", LBXID::FIGURES4 },
    { "figures5", LBXID::FIGURES5 },
    { "figures8", LBXID::FIGURES8 },
    { "figures9", LBXID::FIGURES9 },
    { "figure10", LBXID::FIGURE10 },
    { "figure11", LBXID::FIGURE11 },
    { "figure12", LBXID::FIGURE12 },
    
    { "itemisc", LBXID::ITEMISC},
    
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

template<> TextureID yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, TextureID> mapping = {
    { "additional-skill-icons", TextureID::ADDITIONAL_SKILL_ICONS }
  };

  FETCH_OR_FAIL("TextureID", mapping, node);
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

template<> Plane yaml::parse(const N& node)
{
  if (node.asString() == "arcanus") return Plane::ARCANUS;
  else if (node.asString() == "myrran") return Plane::MYRRAN;
  
  PARSE_ERROR("Plane not found for identifier '%s'", node.asString().c_str());
  assert(false);
  return Plane::ARCANUS;
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

template<> WizardAttribute yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, WizardAttribute> mapping = {
    { "research_point", WizardAttribute::RESEARCH },
    { "gold_gain", WizardAttribute::GOLD_GAIN }
  };

  FETCH_OR_FAIL("WizardAttribute", mapping, node);
}

template<> Property yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, Property> mapping = {
    { "to_hit", Property::TO_HIT },
    { "melee", Property::MELEE },
    { "thrown_attack", Property::THROWN_ATTACK },
    { "ranged_missile", Property::RANGED_MISSILE},
    { "ranged_boulder", Property::RANGED_BOULDER },
    { "ranged_magic", Property::RANGED_MAGIC },
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
    { "sight", Property::SIGHT },
    { "gold_upkeep", Property::GOLD_UPKEEP }
    
  };
  
  FETCH_OR_FAIL("Property", mapping, node);
}

template<> CombatBonus::Phase yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, CombatBonus::Phase> mapping = {
    { "attacking", CombatBonus::Phase::ATTACKING },
    { "defending", CombatBonus::Phase::DEFENDING },
    { "both", CombatBonus::Phase::BOTH }
  };
 
  FETCH_OR_FAIL("CombatBonus::Phase", mapping, node);
}

template<> CombatBonus::Target yaml::parse(const N& node)
{
  static const std::unordered_map<std::string, CombatBonus::Target> mapping = {
    { "attacker", CombatBonus::Target::ATTACKER },
    { "defender", CombatBonus::Target::DEFENDER },
  };
  
  FETCH_OR_FAIL("CombatBonus::Target", mapping, node);
}

#ifdef _WIN32
#pragma warning(pop)
#endif

template<> RangedInfo yaml::parse(const N& node)
{
  if (node == "none")
    return RangedInfo();
  else
  {
    assert(node.IsSequence() && node.size() == 3);
    value_t strength = node[0];
    Ranged type = parse<Ranged>(node[1]);
    value_t ammo = node[2];
    
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
  else if (node == "item_power")
    return skills::Type::ITEM_POWER;
  else if (node == "combat_effect")
    return skills::Type::COMBAT_EFFECT;
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
    value_t value = parse<value_t>(node[i+1]);
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
    else if (node[0] == "png")
      v = SpriteInfo(parse<TextureID>(node[1]), node[2]);
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
      //TODO: parse effects
      return new CitySpell(name, rarity, school, SpellDuration::CONTINUOUS, target, researchCost, manaCost, combatManaCost, upkeep, { });
    }
      
    default:
      assert(false);
      break;
  }
  
  return nullptr;
}

#pragma mark EffectGroup

// TODO: it's here because it's not needed by the game but for consistency
// maybe we should move it to Data
static std::unordered_map<std::string, const EffectGroup*> skillGroups;

template<> EffectGroup::Mode yaml::parse(const N& node)
{
  using Mode = EffectGroup::Mode;
  static const std::unordered_map<std::string, Mode> mapping = {
    { "keep_greater", EffectGroup::Mode::KEEP_GREATER },
    { "unique", EffectGroup::Mode::UNIQUE },
    { "priority", EffectGroup::Mode::PRIORITY }
  };
  
  FETCH_OR_FAIL("EffectGroup::Mode", mapping, node);
}
                                          
template<> const EffectGroup* yaml::parse(const N& node)
{
  //const std::string& identifier = node["identifier"];
  EffectGroup::Mode mode = parse<EffectGroup::Mode>(node["policy"]);
  return new EffectGroup(mode);
}

#pragma mark Effect
template<typename ModifierType> ModifierType yaml::parseModifier(const N& node)
{
  using Mode = typename ModifierType::Mode;
  using Priority = typename ModifierType::Priority;
  
  if (node.IsScalar())
  {
    if (node.asString().find('.') != std::string::npos)
      return ModifierType(node.as<float>());
    else
      return ModifierType(node.as<value_t>());
  }
  else if (node.IsSequence())
  {
    /* fixed value, assuming additive */
    if (node.size() == 1)
      return ModifierType(Mode::ADDITIVE, node[0].as<value_t>(), Priority::ANY);
    else if (node.size() >= 2)
    {
      const std::string& ttype = node[1];

      Mode mode = Mode::ADDITIVE;
      Priority priority = Priority::ANY;
      bool asFloat = false;

      if (ttype == "per_level")
      {
        mode = ModifierType::Mode::ADDITIVE_PARAMETRIC;
        asFloat = true;
      }
      else if (ttype == "fixed")
      {
        mode = ModifierType::Mode::FIXED;
        asFloat = false;
      }
      else if (ttype == "multiplier")
      {
        mode = ModifierType::Mode::MULTIPLICATIVE;
        asFloat = true;
      }
      else if (ttype == "additive")
        ;
      else
        assert(false);

      if (node.size() > 2)
      {
        const std::string& tpriority = node[2];

        bool isNumber = tpriority.find_first_not_of("0123456789") == std::string::npos;

        if (isNumber) priority = ModifierType::priorityFor(node[2].as<value_t>());
        else if (tpriority == "last") priority = Priority::LAST;
        else if (tpriority == "first") priority = Priority::FIRST;
        else assert(false);
      }

      if (asFloat)
        return ModifierType(mode, node[0].as<float>(), priority);
      else
        return ModifierType(mode, node[0].as<value_t>(), priority);
    }
  }

  //TODO: fix macro
  PARSE_ERROR("Cannot parse ModifierType%s", "");
  assert(false);
  return ModifierType(0);
}

template<> const WizardEffect* yaml::parse(const N& node)
{
  const std::string& type = node["type"];
  WizardEffect* effect = nullptr;

  if (type == "wizard_bonus")
  {
    WizardAttribute attribute = parse<WizardAttribute>(node["property"]);
    WizardModifierValue modifier = parseModifier<WizardModifierValue>(node["modifier"]);
    effect = new RetortModifierEffect(attribute, modifier);
  }
  else
    assert(false);

  return effect;
}

template<> const CityEffect* yaml::parse(const N& node)
{
  const std::string& type = node["type"];
  CityEffect* effect = nullptr;

  if (type == "city_bonus")
  {
    static string_map<CityAttribute> mapping = {
      { "mana_power", CityAttribute::MANA_POWER_OUTPUT },
      { "research_point", CityAttribute::RESEARCH_OUTPUT },
      { "unrest_count", CityAttribute::UNREST_COUNT }
    };

    if (mapping.find(node["property"]) == mapping.end())
      assert(false);

    CityAttribute attribute = mapping[node["property"]];
    CityModifierValue modifier = parseModifier<CityModifierValue>(node["modifier"]);

    effect = new SpecificModifierEffect<CityEffect, CityEffectType::CITY_BONUS, CityModifierValue, CityAttribute>(attribute, modifier);
  }
  else if (type == "city_effect")
  {
    static string_map<SimpleCityEffect> mapping = {
      { "allows-nightshade", SimpleCityEffect::ALLOWS_NIGHTSHADE }
    };

    if (mapping.find(node["kind"]) == mapping.end())
      assert(false);

    effect = new EnumEffect<CityEffect, CityEffectType::SIMPLE_EFFECT, SimpleCityEffect>(mapping[node["kind"]]);
  }

  assert(effect);
  return effect;
}

template<> const UnitEffect* yaml::parse(const N& node)
{
  const std::string& type = node["type"];
  UnitEffect* effect = nullptr;
  
  if (type == "unit_bonus")
  {
    Property property = parse<Property>(node["property"]);
    UnitModifierValue modifier = parseModifier<UnitModifierValue>(node["modifier"]);
    effect = new UnitPropertyBonus(property, modifier);
  }
  else if (type == "army_bonus")
  {
    Property property = parse<Property>(node["property"]);
    UnitModifierValue modifier = parseModifier<UnitModifierValue>(node["modifier"]);
    //TODO: affects all, normal only etc
    effect = new ArmyPropertyBonus(property, modifier);
  }
  else if (type == "wizard_bonus")
  {
    WizardAttribute attribute = parse<WizardAttribute>(node["property"]);
    UnitModifierValue modifier = parseModifier<UnitModifierValue>(node["modifier"]);
    effect = new WizardAttributeModifier(attribute, modifier);
  }
  else if (type == "combat_bonus")
  {
    Property property = parse<Property>(node["property"]);
    s16 value = parse<s16>(node["value"]);
    CombatBonus::Phase phase = parse<CombatBonus::Phase>(node["trigger_on"]);
    CombatBonus::Target target = parse<CombatBonus::Target>(node["affects"]);
    
    effect = new CombatBonus(property, value, phase, target, false);
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
      { "wall_crusher", SimpleEffect::Type::WALL_CRUSHING },
      { "invisibility", SimpleEffect::Type::INVISIBILITY },
      { "allow-melee-attacks-against-flying", SimpleEffect::Type::ALLOW_MELEE_ATTACKS_AGAINST_FLYING },
      { "use-mana-pool-for-ranged-attacks", SimpleEffect::Type::USE_MANA_POOL_FOR_RANGED_ATTACKS },
      { "magical-weapons", SimpleEffect::Type::MAGICAL_WEAPONS },
    };
    
    if (mapping.find(node["kind"]) == mapping.end())
    {
      PARSE_ERROR("unknown effect type '%s'", node["kind"].asString().c_str());
      assert(false);
    }
    
    SimpleEffect::Type kind = mapping[node["kind"]];
    
    if (type == "parametric_ability")
    {
      value_t value = parse<value_t>(node["value"]);
      effect = new SimpleParametricEffect(UnitEffectType::ABILITY, kind, value);
    }
    else
      effect = new SimpleEffect(UnitEffectType::ABILITY, kind);
  }
  else if (type == "movement" || type == "disallow-movement")
  {
    static std::unordered_map<std::string, MovementType> mapping = {
      { "mountaineer", MovementType::MOUNTAINWALK },
      { "forester", MovementType::FORESTWALK },
      { "flying", MovementType::FLYING },
      { "swimming", MovementType::SWIMMING },
      { "non_corporeal", MovementType::NON_CORPOREAL },
      { "windwalking", MovementType::WINDWALK },
      { "sailing", MovementType::SAILING }
    };
    
    if (mapping.find(node["kind"]) == mapping.end())
      assert(false);
    
    bool isDisallow = type == "disallow-movement";
    
    MovementType kind = mapping[node["kind"]];
    
    effect = isDisallow ? static_cast<UnitEffect*>(new MovementDisallowEffect(kind)) :  new MovementEffect(kind);
  }
  else if (type == "spell_grant")
  {
    std::string spellName = node["spell"];
    //TODO: can't be mapped now because spells are not loaded yet
    u16 times = parse<u16>(node["times"]);
    s16 strength = optionalParse(node, "strength", 0);
    
    effect = new SpellGrantEffect(nullptr, times, strength);
  }
  else if (type == "compound")
  {
    std::vector<const UnitEffect*> effects;
    parse(node["elements"], effects);

    //TODO: change with generic code for effects of different sources
    effect = new CompoundEffect<UnitEffect>(effects);
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
      std::string groupIdentifier;
      EffectGroupParam groupParam = 0;

      if (node["group"].IsSequence())
      {
        groupIdentifier = node["group"][0].asString();
        groupParam = node["group"][1];
      }
      else
      {
        groupIdentifier = node["group"].asString();
      }
      
      const auto it = skillGroups.find(groupIdentifier);
      
      if (it == skillGroups.end())
      {
        PARSE_ERROR("skill group '%s' is not defined", node["group"].asString().c_str());
        assert(false);
      }
      
      effect->setGroup(it->second, groupParam);
    }
  }

  return effect;
}

std::unordered_map<Skill*, std::string> effectsCopyReferences;

void yaml::solveCopiedSkillEffects()
{
  for (const auto& e : effectsCopyReferences)
  {
    Skill* dest = e.first;
    std::string srcIdent = e.second;
    const Skill* src = Data::skill(srcIdent);
    
    if (src == nullptr)
      PARSE_ERROR("missing skill '%s' for referenced effects copy", srcIdent.c_str());
    
    dest->setEffects(src->getEffects());
  }
}

#pragma mark Skill
template<> const Skill* yaml::parse(const N& node)
{
  Skill* skill = nullptr;
  
  skills::Type type = optionalParse(node, "type", skills::Type::NATIVE);
  
  auto visuals = node["visuals"];
  
  skills::VisualInfo visualInfo;
  visualInfo.hidden = optionalParse(visuals, "hidden", false);

  if (!visualInfo.hidden)
  {
    visualInfo.name = i18n::keyForString(visuals["i18n"]);
    visualInfo.hideValue =  optionalParse(visuals, "hide_value", false);
    visualInfo.icon = parse<SpriteInfo>(visuals["icon"]);
  }
  
  /* use same key used for the name of the skill to search for help mapping */
  visualInfo.help = help::Data::get(visuals["i18n"]);
  
  /* this is valid only for hero skills */
  if (node.hasChild("classes"))
  {
    YAML_ASSERT(type == skills::Type::HERO, "only hero skills support classes entry but '%s' is is not", currentEntry.c_str());
  }
  
  if (node.hasChild("effects"))
  {
    std::vector<const UnitEffect*> effects;
    parse(node["effects"], effects);
    skill = new skills::ConcreteSkill(type, unit_effect_list(effects), visualInfo);
  }
  else if (node.hasChild("copy_effects_from"))
  {
    skill = new skills::ConcreteSkill(type, unit_effect_list(), visualInfo);
    effectsCopyReferences[skill] = node["copy_effects_from"].asString();
  }
  else
  {
    PARSE_ERROR("skill '%s' has no effects or copy_effects_from element", currentEntry.c_str());
    return nullptr;
  }
  
  if (type == skills::Type::ITEM_POWER)
  {
    school_value_map requiredBooks = parse<school_value_map>(node["required_books"]);
    Data::_itemPowerRequirements[skill] = requiredBooks;
  }
  
  return skill;
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
    const Race* race = node["race"].asString() != "any" ? Data::get<const Race*>(node["race"]) : nullptr;
    
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
  /* hero unit spec */
  else if (type == "hero")
  {
    value_t requiredFame = node["required_fame"];
    
    const N yslots = node["slots"];
    using iclass = items::Class;
    items::AllowedSlots<3> slots = items::AllowedSlots<3>({parse<iclass>(yslots[0]), parse<iclass>(yslots[1]), parse<iclass>(yslots[2])});
    
    /* names of heroes per player */
    std::vector<std::string> names;
    parse(node["names"], names);
    
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
                        names,
                        skills // TODO
    );
    
    data.second.hero.portrait = parse<SpriteInfo>(visuals["portrait"]);
  }
  /* fantastic unit */
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
  Plane startingPlane = parse<Plane>(node["starting_plane"]);
  
  const Node& ycityNames = node["city_names"];
  assert(ycityNames.IsSequence());
  
  std::vector<std::string> cityNames;
  for (size_t i = 0; i < ycityNames.size(); ++i)
    cityNames.push_back(ycityNames[i].asString());
  
  data.first = new Race(cityGrowthModifier, outpostGrowthChance, baseProduction, taxIncomeMultiplier, miningBonusMultiplier, manaProducedPerCitizen, foodProductionPerFramer, startingPlane, cityNames);
  
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
    SpriteInfo gemmedPortrait = parse<SpriteInfo>(visual["gemmed_portrait"]);
    SpriteInfo researchPose = parse<SpriteInfo>(visual["research_pose"]);
    SpriteInfo summonPose = parse<SpriteInfo>(visual["summon_pose"]);
    
    SpriteInfo diplomacyMood = parse<SpriteInfo>(visual["diplomacy_mood"]);
    SpriteInfo diplomacySpeaking = parse<SpriteInfo>(visual["diplomacy_speaking"]);
    SpriteInfo diplomacyFadeIn = parse<SpriteInfo>(visual["diplomacy_fadein"]);
    
    gfx.name = name;
    gfx.portraitSmall = portraitSmall;
    gfx.portraitLarge = portraitLarge;
    gfx.gemmedPortrait = gemmedPortrait;
    gfx.researchPose = researchPose;
    gfx.summonPose = summonPose;
    
    gfx.diplomacy.mood = diplomacyMood;
    gfx.diplomacy.speaking = diplomacySpeaking;
    gfx.diplomacy.fadeIn = diplomacyFadeIn;
  }
  
  return data;
}

#pragma mark Buildings

#include "CityScape.h"

template<> std::pair<const Building*, BuildingGfxSpec> yaml::parse(const N& node)
{
  value_t cost = 0;
  Upkeep upkeep;

  auto type = Building::Type::NORMAL;

  if (node["type"] == "special") type = Building::Type::SPECIAL;
  else if (node["type"] == "spell") type = Building::Type::SPELL;
  else if (node["type"] != "normal") assert(false);

  if (type == Building::Type::NORMAL)
  {
    cost = node["cost"];
    upkeep.gold = node["upkeep"][0];
    upkeep.mana = node["upkeep"][1];
  }

  std::vector<const CityEffect*> effects;
  parse(node["effects"], effects);


  const Building* building = new Building(type, cost, upkeep, city_effect_list(effects));
  BuildingGfxSpec gfx;

  /* requirements and replacements are parsed by the caller since they require
     all the buildings to be loaded
  */

  /* visuals */
  {
    const auto& visuals = node["visuals"];

    gfx.name = i18n::keyForString(visuals["i18n"]);
    gfx.desc = i18n::keyForString(visuals["i18desc"]);
    
    gfx.gfx = parse<SpriteInfo>(visuals["gfx"]["sprite"]);
    gfx.width = parse<coord_t>(visuals["gfx"]["width"]);
    gfx.slotSize = Size(visuals["gfx"]["slot_size"][0].as<int_type>(), visuals["gfx"]["slot_size"][1].as<int_type>());
  }
  
  return std::make_pair(building, gfx);
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

  std::vector<const WizardEffect*> effects;

  if (node.hasChild("effects"))
    parse(node["effects"], effects);
  
  Retort* retort = new Retort(identifier, cost, effects);
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
    for (int i = levels.size() - 1; i >= 0; --i)
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
    skillGroups[identifier] = parse<const EffectGroup*>(ygroup);
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
    const auto pair = parse<std::pair<const Wizard*, WizardGfxSpec>>(ywizard);
    const Wizard* wizard = pair.first;
    const WizardGfxSpec& gfx = pair.second;
    Data::registerData(identifier, wizard);
    GfxData::registerData(wizard, gfx);
  }
  
  
}

void yaml::parseBuildings()
{
  N file = parse("buildings.yaml");
  auto buildings = file["buildings"];

  /* load data */
  for (const auto& ybuilding : buildings)
  {
    const std::string& identifier = getIdentifier(ybuilding);
    const auto building = parse<std::pair<const Building*, BuildingGfxSpec>>(ybuilding);

    Data::registerData(identifier, building.first);
    GfxData::registerData(building.first, building.second);
  }

  /* load requirements, must be done later because we need to populate data before */
  {
    for (const auto& ybuilding : buildings)
    {
      const std::string& identifier = getIdentifier(ybuilding);
      const Building* building = Data::building(identifier);

      if (ybuilding["requires"].IsDefined())
      {
        std::vector<std::string> requirements;
        parse(ybuilding["requires"], requirements);

        std::transform(requirements.begin(), requirements.end(), std::inserter(Data::buildingDependsOnBuilding, Data::buildingDependsOnBuilding.end()), [building](const std::string& key) {
          const Building* requirement = Data::building(key);

          if (requirement == nullptr)
            assert(false);

          return std::make_pair(building, requirement);
        });
      }

      const Building* replacedBuilding = ybuilding["replaces"].IsDefined() ? Data::building(ybuilding["replaces"]) : nullptr;
      Data::buildingReplacedByBuilding[replacedBuilding] = building;
    }
  }

  /* TODO: hardcoded for testing */
  const CityEffect* effect = Data::building("mage_fortress")->effects()[0];
  effect->as<SpecificModifierEffect<CityEffect, CityEffectType::CITY_BONUS, CityModifierValue, CityAttribute>>()->setCondition([](const City* city) { return city->getPosition().plane == Plane::MYRRAN; });
  
  //TODO: hardcoded because they're useful in code for now, let's hope to be able to remove most of them through effects
  Building::BUILDERS_HALL = Data::building("builders_hall");
  Building::BARRACKS = Data::building("barracks");
  Building::ARMORY = Data::building("armory");
  Building::FIGHTERS_GUILD = Data::building("fighters_guild");
  Building::ARMORERS_GUILD = Data::building("armorers_guild");
  Building::WAR_COLLEGE = Data::building("war_college");
  Building::SMITHY = Data::building("smithy");
  Building::STABLE = Data::building("stable");
  Building::ANIMISTS_GUILD = Data::building("animists_guild");
  Building::FANTASTIC_STABLE = Data::building("fantastic_stable");
  Building::SHIP_WRIGHTS_GUILD = Data::building("ship_wrights_guild");
  Building::SHIP_YARD = Data::building("ship_yard");
  Building::MARITIME_GUILD = Data::building("maritime_guild");
  Building::SAWMILL = Data::building("sawmill");
  Building::LIBRARY = Data::building("library");
  Building::SAGES_GUILD = Data::building("sages_guild");
  Building::ORACLE = Data::building("oracle");
  Building::ALCHEMISTS_GUILD = Data::building("alchemists_guild");
  Building::UNIVERSITY = Data::building("university");
  Building::WIZARDS_GUILD = Data::building("wizards_guild");
  Building::SHRINE = Data::building("shrine");
  Building::TEMPLE = Data::building("temple");
  Building::PARTHENON = Data::building("parthenon");
  Building::CATHEDRAL = Data::building("cathedral");
  Building::MARKETPLACE = Data::building("marketplace");
  Building::BANK = Data::building("bank");
  Building::MERCHANTS_GUILD = Data::building("merchants_guild");
  Building::GRANARY = Data::building("granary");
  Building::FARMERS_MARKET = Data::building("farmers_market");
  Building::FORESTERS_GUILD = Data::building("foresters_guild");
  Building::BUILDERS_HALL = Data::building("builders_hall");
  Building::MECHANICIANS_GUILD = Data::building("mechanicians_guild");
  Building::MINERS_GUILD = Data::building("miners_guild");
  Building::CITY_WALLS = Data::building("city_walls");
  Building::TRADE_GOODS = Data::building("trade_goods");
  Building::HOUSING = Data::building("housing");
  Building::MAGE_FORTRESS = Data::building("mage_fortress");
  Building::SUMMONING_CIRCLE = Data::building("summoning_circle");
}

void yaml::parseRaces()
{
  N file = parse("races.yaml");
  auto races = file["races"];
  
  for (const auto& yrace : races)
  {
    const std::string& identifier = getIdentifier(yrace);
    const auto race = parse<std::pair<const Race*, RaceGfxSpec>>(yrace);
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

void yaml::parseHelp()
{
  N file = parse("help.yaml");
  auto entries = file["entries"];
  
  for (const auto entry : entries)
  {
    const std::string& key = entry.first;
    
    // support for structure [lbx, string] only for now
    assert(entry.second.IsSequence() && entry.second.size() == 2);

    const help::Paragraph* paragraph = help::Data::getFromLBX(entry.second[1].asString());
    help::Data::mapping[key] = paragraph != nullptr ? paragraph : new help::Paragraph(fmt::format("#help#{}#", key).c_str(), fmt::format("#help#{}#", key).c_str());
  }
    
}

void yaml::parse()
{
  parseLocalization();
  parseHelp();
  parseRaces();
  parseBuildings();
  parseSkills();
  parseLevels();
  parseUnits();
  parseSpells();
  parseWizards();
  
  solveCopiedSkillEffects();

#if defined(DEBUG)
  LOGD("[yaml] loaded %zu skills", Data::containerFor<const Skill*>().size());
  LOGD("[yaml] loaded %zu spells", Data::containerFor<const Spell*>().size());
  LOGD("[yaml] loaded %zu units", Data::containerFor<const UnitSpec*>().size());

  const auto i18missing = i18n::unlocalizedEntries();
  for (const auto& entry : i18missing)
    LOGD("[i18n] Missing localized entry: %s", entry.c_str());
#endif
}
