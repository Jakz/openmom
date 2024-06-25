#include "Localization.h"

#include "UnitSpec.h"


using namespace std;


I18 i18n::customMappingFreeIndex = I18::FIRST_AVAILABLE_INDEX;

std::unordered_map<std::string, I18>& i18n::getMapping()
{
  static std::unordered_map<std::string, I18> customMapping;
  return customMapping;
}

void i18n::mapCustomEntry(std::string key, std::string value)
{
  getData()[keyForString(key)] = value;
}

I18 i18n::keyForString(const std::string& key)
{
  auto it = getMapping().find(key);
  if (it != getMapping().end())
    return it->second;
  
  I18 nkey = customMappingFreeIndex;
  getMapping()[key] = nkey;
  customMappingFreeIndex = static_cast<I18>(static_cast<u32>(customMappingFreeIndex) + 1);
  
  getData()[nkey] = "#"+key+"#";
  
  return nkey;
}

#if defined(DEBUG)
std::vector<std::string> i18n::unlocalizedEntries()
{
  std::vector<std::string> entries;
  
  for (const auto& key : getMapping())
  {
    auto it = getData().find(key.second);
    if (it == getData().end() || it->second == "#"+key.first+"#")
      entries.push_back(key.first);
  }
  
  std::sort(entries.begin(), entries.end());
  return entries;
}
#endif

constexpr const I18 i18n::CITY_SIZE_NAMES[];
constexpr I18 i18n::SPELL_KIND_NAMES[];

//TODO: maybe find a better solution than having a local static variable to avoid problems with
//      order of initialzation of static variables
i18n::strings_table<I18>& i18n::getData()
{
  static strings_table<I18> data = {
    {I18::CITY_HAMLET, "Hamlet"},
    { I18::CITY_VILLAGE, "Village" },
    { I18::CITY_TOWN, "Town" },
    { I18::CITY_CITY, "City" },
    { I18::CITY_CAPITOL, "Capitol" },

    { I18::MESSAGE_NEW_BUILDING, "The {} of {} has completed the construction of a {}." },
    { I18::MESSAGE_OUTPOST_GROWN_TO_CITY, "The outpost of %s has grown into a hamlet." },

    { I18::SURVEYOR_CITY_FORBID_DISTANCE, "Cities cannot be built less than 3 squares form any other city." },
    { I18::SURVEYOR_CITY_FORBID_WATER, "Cities cannot be built on water." },
    { I18::SURVEYOR_MAX_POPULATION, "Maximum Pop %u" },
    { I18::SURVEYOR_PRODUCTION_BONUS, "Prod Bonus +%u%%" },

    { I18::HERO_DWARF,"Dwarf" },
    { I18::HERO_ORC_WARRIOR,"Orc Warrior" },

    { I18::UNIT_MAGIC_SPIRIT,"Magic Spirit" },
    { I18::UNIT_HELL_HOUNDS,"Hell Hounds" },
    { I18::UNIT_GREAT_DRAKE,"Great Drake" },

    { I18::SPELL_KIND_SUMMONING,"Summoning" },
    { I18::SPELL_KIND_SPECIAL,"Special Spells" },
    { I18::SPELL_KIND_CITY,"City Spells" },
    { I18::SPELL_KIND_ENCHANTMENT,"Enchantment" },
    { I18::SPELL_KIND_UNIT_SPELL,"Unit Spells" },
    { I18::SPELL_KIND_COMBAT_SPELL,"Combat Spells" },
    { I18::SPELL_KIND_RESEARCH,"Research" },
    { I18::SPELL_KIND_SPELLS,"Spells" },

    { I18::SPELL_ENDURANCE, "Endurance" },
    { I18::SPELL_GUARDIAN_SPIRIT, "Guardian Spirit" },
    { I18::SPELL_HEALING, "Healing" },
    { I18::SPELL_HEROISM, "Heroism" },
    { I18::SPELL_HOLY_ARMOR, "Holy Armor" },
    { I18::SPELL_HOLY_WEAPON, "Holy Weapon" },
    { I18::SPELL_JUST_CAUSE, "Just Cause" },
    { I18::SPELL_STAR_FIRES, "Star Fires" },
    { I18::SPELL_TRUE_LIGHT, "True Light" },

    { I18::SPELL_CORRUPTION, "Corruption" },
    { I18::SPELL_CHANGE_TERRAIN, "Change Terrain" },
    { I18::SPELL_RAISE_VOLCANO, "Raise Volcano" },

    { I18::EVENT_BAD_MOON, "Bad Moon" },
    { I18::EVENT_RED_CONJUNCTION, "Conjunction" },
    { I18::EVENT_GREEN_CONJUNCTION, "Conjunction" },
    { I18::EVENT_BLUE_CONJUNCTION, "Conjunction" },
    { I18::EVENT_DEPLETION, "Depletion" },
    { I18::EVENT_DIPLOMATIC_MARRIAGE, "Diplomatic Marriage" },
    { I18::EVENT_DISJUNCTION, "Disjunction" },
    { I18::EVENT_DONATION, "Donation" },
    { I18::EVENT_EARTHQUAKE, "Earthquake" },
    { I18::EVENT_THE_GIFT, "The Gift" },
    { I18::EVENT_GOOD_MOON, "Good Moon" },
    { I18::EVENT_GREAT_METEOR, "Great Meteor" },
    { I18::EVENT_MANA_SHORT, "Mana Short" },
    { I18::EVENT_NEW_MINERALS, "New Minerals" },
    { I18::EVENT_PIRACY, "Piracy" },
    { I18::EVENT_PLAGUE, "Plague" },
    { I18::EVENT_POPULATION_BOOM, "Population Boom" },
    { I18::EVENT_REBELLION, "Rebellion" },

    { I18::ITEM_SWORD, "Sword" },
    { I18::ITEM_MACE, "Mace" },
    { I18::ITEM_AXE, "Axe" },
    { I18::ITEM_BOW, "Bow" },
    { I18::ITEM_STAFF, "Staff" },
    { I18::ITEM_WAND, "Wand" },
    { I18::ITEM_AMULET, "Amulet" },
    { I18::ITEM_RING, "Ring" },
    { I18::ITEM_CLOAK, "Cloak" },
    { I18::ITEM_GAUNTLET, "Gauntlet" },
    { I18::ITEM_HELM, "Helm" },
    { I18::ITEM_ORB, "Orb" },
    { I18::ITEM_SHIELD, "Shield" },
    { I18::ITEM_CHAIN_MAIL, "Chain Mail" },
    { I18::ITEM_PLATE_MAIL, "Plate Mail" },

    { I18::RELATION_HATRED, "Hatred" },
    { I18::RELATION_TROUBLED, "Troubled" },
    { I18::RELATION_TENSE, "Tense" },
    { I18::RELATION_RESTLESS, "Restless" },
    { I18::RELATION_UNEASY, "Uneasy" },
    { I18::RELATION_NEUTRAL, "Neutral" },
    { I18::RELATION_RELAXED, "Relaxed" },
    { I18::RELATION_CALM, "Calm" },
    { I18::RELATION_PEACEFUL, "Peaceful" },
    { I18::RELATION_FRIENDLY, "Friendly" },
    { I18::RELATION_HARMONY, "Harmony" },

    { I18::UI_UNIT_DETAIL_MOVES, "Moves" },
    { I18::UI_UNIT_DETAIL_UPKEEP, "Upkeep" },

    { I18::EMPTY, "" },
    { I18::PLACEHOLDER, "Placeholder" },
  };

  return data;
}


unordered_map<SkillBase, string, enum_hash> i18n::skills =
{
  {SkillBase::HERO_AGILITY, "Agility"},
  {SkillBase::HERO_ARCANE_POWER, "Arcane Power"},
  {SkillBase::HERO_ARMS_MASTER, "Arms Master"},
  {SkillBase::HERO_BLADE_MASTER, "Blade Master"},
  {SkillBase::HERO_CHARMED, "Charmed"},
  {SkillBase::HERO_CONSTITUTION, "Constitution"},
  {SkillBase::HERO_LEADERSHIP, "Leadership"},
  {SkillBase::HERO_LEGENDARY, "Legendary"},
  {SkillBase::HERO_LUCK, "Luck"},
  {SkillBase::HERO_MIGHT, "Might"},
  {SkillBase::HERO_NOBLE, "Noble"},
  {SkillBase::HERO_PRAYER_MASTER, "Prayer Master"},
  {SkillBase::HERO_SAGE, "Sage"},
  {SkillBase::HERO_SPELL_CASTER, "Spell Caster"},
  
  {SkillBase::FIRST_STRIKE, "First Strike"},
  {SkillBase::NEGATE_FIRST_STRIKE, "Negate First Strike"},
  {SkillBase::ARMOR_PIERCING, "Armor Piercing"},
  {SkillBase::LONG_RANGE, "Long Range"},
  {SkillBase::DISPEL_EVIL, "Dispel Evil"},
  {SkillBase::CAUSE_FEAR, "Cause Fear Spell"},
  {SkillBase::WEB_SPELL, "Web Spell"},
  {SkillBase::CREATE_UNDEAD, "Create Undead"},
  {SkillBase::SUMMON_DEMONS, "Summon Demons"},
  {SkillBase::CASTER, "Caster"},
  {SkillBase::HEALING_SPELL, "Healing Spell"},
  {SkillBase::FIRE_BALL_SPELL, "Fire Ball Spell"},
  {SkillBase::DOOM_BOLT_SPELL, "Doom Bolt Spell"},
  
  {SkillBase::ILLUSIONARY_ATTACK, "Illusionary Attack"},
  {SkillBase::POISON_ATTACK, "Poison Attack"},
  {SkillBase::LIFE_STEALING, "Life Stealing"},
  {SkillBase::FIERY_BREATH, "Fiery Breath"},
  {SkillBase::LIGHTING_BREATH, "Lighting Breath"},
  {SkillBase::DOOM_GAZE, "Doom Gaze"},
  {SkillBase::DEATH_GAZE, "Death Gaze"},
  {SkillBase::STONE_GAZE, "Stoning Gaze"},
  {SkillBase::STONE_TOUCH, "Stoning Touch"},
  {SkillBase::IMMOLATION, "Immolation"},
  {SkillBase::THROW_WEAPON, "Throw Weapon"},
  
  {SkillBase::MELD, "Meld With Node"},
  {SkillBase::PURIFY, "Purify"},
  {SkillBase::CREATE_OUTPOST, "Create Outpost"},
  {SkillBase::CREATE_ROAD, "Create Road"},
  {SkillBase::WALL_CRUSHING, "Wall Crushing"},
  
  {SkillBase::TO_HIT, "To Hit"},
  {SkillBase::LUCKY, "Lucky"},
  {SkillBase::HOLY_BONUS, "Holy Bonus"},
  {SkillBase::RESISTANCE_TO_ALL, "Resistance to All"},
  {SkillBase::HEALER, "Healer"},
  
  {SkillBase::FORESTWALK, "Forester"},
  {SkillBase::FLYING, "Flight"},
  {SkillBase::UNDERGROUND, "Merging"},
  {SkillBase::MOUNTAINWALK, "Mountaineer"},
  {SkillBase::NON_CORPOREAL, "Non Corporeal"},
  {SkillBase::PATH_FINDER, "Path Finding"},
  {SkillBase::PLANAR_TRAVEL, "Plane Shift"},
  {SkillBase::TELEPORT, "Teleport"},
  {SkillBase::SWIMMING, "Waterwalking"},
  {SkillBase::WINDWALK, "Windwalking"},
  {SkillBase::SAILING, "Sailing"},
  {SkillBase::DESERTWALK, "Sandwalk"},
  {SkillBase::SWAMPWALK, "Swampwalking"},
  
  {SkillBase::SPELL_ELDRITCH_WEAPON, "Eldritch Weapon"},
  {SkillBase::SPELL_FLAME_BLADE, "Flame blade"},
  {SkillBase::SPELL_CHAOS_CHANNELS_SHIELD, "Chaos Channels"},
  {SkillBase::SPELL_CHAOS_CHANNELS_WINGS, "Chaos Channels"},
  {SkillBase::SPELL_CHAOS_CHANNELS_BREATH, "Chaos Channels"},
  
  {SkillBase::SPELL_RESIST_ELEMENTS, "Resist Elements"},
  {SkillBase::SPELL_STONE_SKIN, "Stone Skin"},
  {SkillBase::SPELL_GIANT_STRENGTH, "Giant Strength"},
  
  {SkillBase::SPELL_ELEMENTAL_ARMOR, "Elemental Armor"},
  {SkillBase::SPELL_IRON_SKIN, "Iron Skin"},
  {SkillBase::SPELL_REGENERATION, "Regeneration"},
  
  {SkillBase::SPELL_RESIST_MAGIC, "Resist Magic"},
  {SkillBase::SPELL_GUARDIAN_WIND, "Guardian Wind"},
  {SkillBase::SPELL_SPELL_LOCK, "Spell Lock"},
  {SkillBase::SPELL_INVISIBILITY, "Invisibility"},
  {SkillBase::SPELL_HASTE, "Haste"},
  
  {SkillBase::SPELL_HOLY_WEAPON, "Holy Weapon"},
  {SkillBase::SPELL_ENDURANCE, "Endurance"},
  {SkillBase::SPELL_TRUE_SIGHT, "True Sight"},
  {SkillBase::SPELL_HEROISM, "Heroism"},
  {SkillBase::SPELL_HOLY_ARMOR, "Holy Armor"},
  {SkillBase::SPELL_LION_HEART, "Lion Heart"},
  {SkillBase::SPELL_INVULNERABILITY, "Invulnerability"},
  {SkillBase::SPELL_RIGHTEOUSNESS, "Righteousness"},
  
  {SkillBase::SPELL_UNDEAD, "Undead"},
  
  {SkillBase::SPELL_CLOAK_OF_FEAR, "Cloak of Fear"},
  {SkillBase::SPELL_BLACK_CHANNELS, "Black Channels"},
  {SkillBase::SPELL_WRAITH_FORM, "Wraith Form"},
  {SkillBase::SPELL_BERSERK, "Berserk"},
  
  {SkillBase::MITHRIL_WEAPONS, "Mithril Weapons"},
  {SkillBase::ADAMANTIUM_WEAPONS, "Adamantium Weapons"},
  
  {SkillBase::STATUS_WEB, "Web"},
  {SkillBase::STATUS_CONFUSION, "Confusion"},
  {SkillBase::STATUS_VERTIGO, "Vertigo"},
  {SkillBase::STATUS_HIT_PENALTY, "To Hit"},
  {SkillBase::STATUS_SHATTER, "Shatter"},
  {SkillBase::STATUS_WARP_CREATURE_DEFENSE, "Warp Creature"},
  {SkillBase::STATUS_WARP_CREATURE_ATTACK, "Warp Creature"},
  {SkillBase::STATUS_WARP_CREATURE_MAGIC_DEFENSE, "Warp Creature"},
  {SkillBase::STATUS_BLACK_SLEEP, "Black Sleep"},
  {SkillBase::STATUS_WEAKNESS, "Weakness"},
  {SkillBase::STATUS_POSESSION, "Posession"}
};

std::unordered_map<TileType, std::vector<std::string>, enum_hash> i18n::surveyorDescs
{
  {TileType::FOREST, {"Forest", "1/2 production", "+3% production"} },
  {TileType::MOUNTAIN, {"Mountain", "+5% production"} },
  {TileType::DESERT, {"Desert"} },
  {TileType::GRASS, {"Grasslands"} },
  {TileType::HILL, {"Hills"} },
  {TileType::MOUNTAIN, {"Mountain"} },
  {TileType::TUNDRA, {"Tundra"} },
  {TileType::SWAMP, {"Swamp"} },
  {TileType::VOLCANO, {"Volcano"} },
  {TileType::OCEAN, {"Ocean"} },
  {TileType::SHORE, {"Shore", "1/2 food", "+10% gold"} },
  {TileType::RIVER_MOUTH, {"River Mouth"} },
};

