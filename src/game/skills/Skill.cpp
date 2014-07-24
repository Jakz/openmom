#include "Skill.h"

#include "Localization.h"
#include "UnitSpec.h"

using namespace std;

bool Skill::isHeroBase(SkillBase base)
{
  switch (base) {
    case SkillBase::HERO_AGILITY:
    case SkillBase::HERO_ARCANE_POWER:
    case SkillBase::HERO_ARMS_MASTER:
    case SkillBase::HERO_BLADE_MASTER:
    case SkillBase::HERO_CHARMED:
    case SkillBase::HERO_CONSTITUTION:
    case SkillBase::HERO_LEADERSHIP:
    case SkillBase::HERO_LEGENDARY:
    case SkillBase::HERO_LUCK:
    case SkillBase::HERO_MIGHT:
    case SkillBase::HERO_NOBLE:
    case SkillBase::HERO_PRAYER_MASTER:
    case SkillBase::HERO_SAGE:
    case SkillBase::HERO_SPELL_CASTER:
      return true;
      
    default: return false;
  }
}

bool Skill::isMovementBase(SkillBase base)
{
  switch (base) {
    case SkillBase::FORESTWALK:
    case SkillBase::FLYING:
    case SkillBase::UNDERGROUND:
    case SkillBase::MOUNTAINWALK:
    case SkillBase::NON_CORPOREAL:
    case SkillBase::PATH_FINDER:
    case SkillBase::PLANAR_TRAVEL:
    case SkillBase::TELEPORT:
    case SkillBase::SWIMMING:
    case SkillBase::WINDWALK:
    case SkillBase::SAILING:
    case SkillBase::DESERTWALK:
    case SkillBase::SWAMPWALK:
      return true;
      
    default: return false;
  }
}

bool Skill::Comparator::operator()(const Skill *b1, const Skill *b2)
{
  if (isHeroBase(b1->base)) return true;
  else if (isHeroBase(b2->base)) return false;
  
  if (isMovementBase(b1->base)) return true;
  else if (isMovementBase(b2->base)) return false;
  
  return i18n::s(b1->base).compare(i18n::s(b2->base)) < 0;
}







const string Skill::name()
{
  return i18n::s(base);
}

const string ConcreteSkill::name()
{
  if (hideValue) return Skill::name();
  else
  {
    const SkillEffect *effect = effects[0];
    
    if (effect->type == SkillEffect::Type::UNIT_BONUS)
      return string("+") + to_string(static_cast<const PropertyBonus*>(effect)->value) + " " + Skill::name();
    else if (effects[0]->type == SkillEffect::Type::SPECIAL_ATTACK)
      return Skill::name() + " (" + to_string(static_cast<const SpecialAttackEffect*>(effect)->strength) + ")";
  }
  
  return "";
}

bool ConcreteSkill::hasSimpleEffect(SimpleEffect::Type type)
{
  auto it = find_if(effects.begin(), effects.end(), [&](const SkillEffect* effect) {
    switch (effect->type)
    {
      case SkillEffect::Type::IMMUNITY:
      case SkillEffect::Type::MOVEMENT:
      case SkillEffect::Type::ABILITY:
        return static_cast<const SimpleEffect*>(effect)->effect == type;
      default: return false;
    }
  });
  
  return it != effects.end();
}

const SpecialAttackEffect* ConcreteSkill::hasEffect(SimpleEffect::Type ident)
{
  auto it = find_if(effects.begin(), effects.end(), [&](const SkillEffect* effect) { return effect->type == SkillEffect::Type::SPECIAL_ATTACK && static_cast<const SimpleEffect*>(effect)->effect == ident; });
  return it != effects.end() ? static_cast<const SpecialAttackEffect*>(*it) : nullptr;
}

namespace skillimpl
{
  const ConcreteSkill MELD = ConcreteSkill(SkillBase::MELD, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::MELD_NODE)});
  const ConcreteSkill CREATE_OUTPOST = ConcreteSkill(SkillBase::CREATE_OUTPOST, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::CREATE_OUTPOST)});
  const ConcreteSkill CREATE_ROAD = ConcreteSkill(SkillBase::CREATE_ROAD, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::CREATE_ROAD)});
  const ConcreteSkill WALL_CRUSHING = ConcreteSkill(SkillBase::WALL_CRUSHING, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::WALL_CRUSHING)}); // maybe should be combat?
  const ConcreteSkill PURIFY = ConcreteSkill(SkillBase::MELD, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::PURIFY)});
  
  const ConcreteSkill LUCKY = ConcreteSkill(SkillBase::LUCKY, UnitBonus::build({Property::TO_HIT, Property::TO_DEFEND, Property::RESIST}, 1));
  
  const ConcreteSkill LARGE_SHIELD = ConcreteSkill(SkillBase::LARGE_SHIELD, {new UnitBonus(Property::SHIELDS_RANGED,2)} );
  const ConcreteSkill FIRST_STRIKE = ConcreteSkill(SkillBase::FIRST_STRIKE, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::FIRST_STRIKE)} );
  const ConcreteSkill NEGATE_FIRST_STRIKE = ConcreteSkill(SkillBase::NEGATE_FIRST_STRIKE, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::NEGATE_FIRST_STRIKE)} );
  const ConcreteSkill ARMOR_PIERCING = ConcreteSkill(SkillBase::FIRST_STRIKE, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::ARMOR_PIERCING)} );
  const ConcreteSkill LONG_RANGE = ConcreteSkill(SkillBase::FIRST_STRIKE, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::LONG_RANGE)} );
  const ConcreteSkill ILLUSIONARY_ATTACK = ConcreteSkill(SkillBase::ILLUSIONARY_ATTACK, {new SimpleEffect(SkillEffect::Type::ABILITY, SimpleEffect::Type::ILLUSIONARY_ATTACK)} );

  const ConcreteSkill POISON_ATTACK1 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 1)}, false);
  const ConcreteSkill POISON_ATTACK4 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 4)}, false);
  const ConcreteSkill POISON_ATTACK6 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 6)}, false);
  const ConcreteSkill POISON_ATTACK15 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 15)}, false);

  const ConcreteSkill LIFE_STEALING0 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, 0)});
  const ConcreteSkill LIFE_STEALING3 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, -3)}, false);
  const ConcreteSkill LIFE_STEALING4 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, -4)}, false);
  const ConcreteSkill LIFE_STEALING5 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, -5)}, false);
  
  const ConcreteSkill IMMOLATION = ConcreteSkill(SkillBase::IMMOLATION, {new SpecialAttackEffect(SpecialAttackEffect::Type::IMMOLATION, 4)});
  
  const ConcreteSkill FIERY_BREATH5 = ConcreteSkill(SkillBase::FIERY_BREATH, {new SpecialAttackEffect(SpecialAttackEffect::Type::FIRE_BREATH, 5)}, false);
  const ConcreteSkill FIERY_BREATH20 = ConcreteSkill(SkillBase::FIERY_BREATH, {new SpecialAttackEffect(SpecialAttackEffect::Type::FIRE_BREATH, 20)}, false);

  const ConcreteSkill LIGHTING_BREATH20 = ConcreteSkill(SkillBase::LIGHTING_BREATH, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIGHTING_BREATH, 20)}, false);
  
  const ConcreteSkill DOOM_GAZE4 = ConcreteSkill(SkillBase::DOOM_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::DOOM_GAZE, 4)}, false);
  
  const ConcreteSkill DEATH_GAZE2 = ConcreteSkill(SkillBase::DEATH_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::DEATH_GAZE, -2)}, false);
  const ConcreteSkill DEATH_GAZE4 = ConcreteSkill(SkillBase::DEATH_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::DEATH_GAZE, -4)}, false);
  
  const ConcreteSkill STONE_GAZE1 = ConcreteSkill(SkillBase::STONE_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_GAZE, -1)}, false);
  const ConcreteSkill STONE_GAZE3 = ConcreteSkill(SkillBase::STONE_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_GAZE, -3)}, false);
  const ConcreteSkill STONE_GAZE4 = ConcreteSkill(SkillBase::STONE_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_GAZE, -4)}, false);

  const ConcreteSkill STONE_TOUCH3 = ConcreteSkill(SkillBase::STONE_TOUCH, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_TOUCH, -3)}, false);

  // TODO: a parametric throw weapon is needed for hero ability
  const ConcreteSkill THROW_WEAPON1 = ConcreteSkill(SkillBase::THROW_WEAPON, {new SpecialAttackEffect(SpecialAttackEffect::Type::THROWN_ATTACK, 1)}, false);
  const ConcreteSkill THROW_WEAPON2 = ConcreteSkill(SkillBase::THROW_WEAPON, {new SpecialAttackEffect(SpecialAttackEffect::Type::THROWN_ATTACK, 2)}, false);

  const ConcreteSkill SCOUTING1 = ConcreteSkill(SkillBase::SCOUTING, {new UnitBonus(Property::SIGHT,1)}, false);
  const ConcreteSkill SCOUTING2 = ConcreteSkill(SkillBase::SCOUTING, {new UnitBonus(Property::SIGHT,2)}, false);
  const ConcreteSkill SCOUTING3 = ConcreteSkill(SkillBase::SCOUTING, {new UnitBonus(Property::SIGHT,3)}, false);
  
  const ConcreteSkill TO_HIT1 = ConcreteSkill(SkillBase::TO_HIT, {new UnitBonus(Property::TO_HIT,1)}, false);
  const ConcreteSkill TO_HIT2 = ConcreteSkill(SkillBase::TO_HIT, {new UnitBonus(Property::TO_HIT,2)}, false);
  const ConcreteSkill TO_HIT3 = ConcreteSkill(SkillBase::TO_HIT, {new UnitBonus(Property::TO_HIT,3)}, false);
  
  const ConcreteSkill MITHRIL_WEAPONS = ConcreteSkill(SkillBase::MITHRIL_WEAPONS, {new SkillEffect(SkillEffect::Type::MAGIC_WEAPONS), new UnitBonus(Property::MELEE,1)});
  const ConcreteSkill ADAMANTIUM_WEAPONS = ConcreteSkill(SkillBase::ADAMANTIUM_WEAPONS, {new SkillEffect(SkillEffect::Type::MAGIC_WEAPONS), new UnitBonus(Property::MELEE,2)});
  
  const ConcreteSkill FORESTWALK = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::FORESTWALK});
  const ConcreteSkill FLYING = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::FLYING});
  const ConcreteSkill UNDERGROUND = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::UNDERGROUND});
  const ConcreteSkill MOUNTAINWALK = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::MOUNTAINWALK});
  const ConcreteSkill NON_CORPOREAL = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::NON_CORPOREAL});
  const ConcreteSkill PATH_FINDER = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::PATH_FINDER});
  const ConcreteSkill PLANAR_TRAVEL = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::PLANAR_TRAVEL});
  const ConcreteSkill TELEPORT = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::TELEPORT});
  const ConcreteSkill SWIMMING = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::SWIMMING});
  const ConcreteSkill WINDWALK = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::WINDWALK});
  const ConcreteSkill DESERTWALK = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::DESERTWALK});
  const ConcreteSkill SWAMPWALK = ConcreteSkill(SkillBase::FORESTWALK, {&Effects::SWAMPWALK});

  
  const ConcreteSkill IMMUNITY_MAGIC = ConcreteSkill(SkillBase::IMMUNITY_MAGIC, {new SimpleEffect(SkillEffect::Type::IMMUNITY, SimpleEffect::Type::IMMUNITY_MAGIC), new UnitBonus(Property::RESIST, 50)} );
  const ConcreteSkill IMMUNITY_MISSILE = ConcreteSkill(SkillBase::IMMUNITY_MISSILE, {new SimpleEffect(SkillEffect::Type::IMMUNITY, SimpleEffect::Type::IMMUNITY_MISSILE), new UnitBonus(Property::SHIELDS_RANGED, 50)} );
  const ConcreteSkill IMMUNITY_ILLUSIONS = ConcreteSkill(SkillBase::IMMUNITY_ILLUSIONS, {new SimpleEffect(SkillEffect::Type::IMMUNITY, SimpleEffect::Type::IMMUNITY_ILLUSIONS)} );
  
  
  
  const ConcreteSkill HERO_AGILITY = ConcreteSkill(SkillBase::HERO_AGILITY, {new UnitLevelBonus(Property::SHIELDS, 1.0f)});
  // TODO: ARCANE_POWER
  const ConcreteSkill HERO_ARMS_MASTER = ConcreteSkill(SkillBase::HERO_ARMS_MASTER, {new ArmyBonus(Property::XP, 2, ArmyBonus::Type::NORMAL_UNITS)});
  const ConcreteSkill HERO_BLADE_MASTER = ConcreteSkill(SkillBase::HERO_BLADE_MASTER, {new UnitLevelBonus(Property::TO_HIT, 0.5f)}); // TODO: missin effect in mechanics?
  // TODO: CASTER
  const ConcreteSkill HERO_CHARMED = ConcreteSkill(SkillBase::HERO_CHARMED, {new UnitBonus(Property::RESIST, 30)}); // TODO: missing effect in mechanics?
  const ConcreteSkill HERO_CONSTITUTION = ConcreteSkill(SkillBase::HERO_CONSTITUTION, {new UnitLevelBonus(Property::HIT_POINTS, 1.0f)}); // TODO: missin effect in mechanics?
  // TODO: LEADERSHIP
  // TODO: LEGENDARY
  const WrapSkill HERO_LUCK = WrapSkill(SkillBase::HERO_LUCK, LUCKY);
  const ConcreteSkill HERO_MIGHT = ConcreteSkill(SkillBase::HERO_MIGHT, {new UnitLevelBonus(Property::MELEE, 1.0f)}); // TODO: missin effect in mechanics?
  // TODO: NOBLE
  const ConcreteSkill HERO_PRAYER_MASTER = ConcreteSkill(SkillBase::HERO_PRAYER_MASTER, {new ArmyLevelBonus(Property::RESIST, 1.0f, ArmyBonus::Type::WHOLE_ARMY)});
  // TODO: SAGE
  // TODO: SPELL_CASTER
  
  const ConcreteSkill SPELL_ELDRITCH_WEAPON = ConcreteSkill(SkillBase::MITHRIL_WEAPONS, {new SkillEffect(SkillEffect::Type::MAGIC_WEAPONS), new CombatBonus(Property::TO_DEFEND, -1, CombatBonus::Phase::ATTACKER, CombatBonus::Phase::DEFENDER, false)});
  const ConcreteSkill SPELL_FLAME_BLADE = ConcreteSkill(SkillBase::SPELL_FLAME_BLADE, UnitBonus::build({Property::MELEE, Property::RANGED, Property::THROWN_ATTACK}, 2));

  const ConcreteSkill SPELL_RESIST_ELEMENTS = ConcreteSkill(SkillBase::SPELL_RESIST_ELEMENTS, UnitBonus::build({Property::RESIST_CHAOS, Property::RESIST_NATURE, Property::SHIELDS_CHAOS, Property::SHIELDS_NATURE}, 3));
  const ConcreteSkill SPELL_GIANT_STRENGTH = ConcreteSkill(SkillBase::SPELL_GIANT_STRENGTH, {new UnitBonus(Property::MELEE,1)} );
  const ConcreteSkill SPELL_STONE_SKIN = ConcreteSkill(SkillBase::SPELL_STONE_SKIN, {new UnitBonus(Property::SHIELDS,1)} );
  const WrapSkill SPELL_SWIMMING = WrapSkill(SkillBase::SPELL_SWIMMING, SWIMMING);
  const WrapSkill SPELL_PATH_FINDER = WrapSkill(SkillBase::SPELL_PATH_FINDER, PATH_FINDER);
  const ConcreteSkill SPELL_ELEMENTAL_ARMOR = ConcreteSkill(SkillBase::SPELL_ELEMENTAL_ARMOR, UnitBonus::build({Property::RESIST_CHAOS, Property::RESIST_NATURE, Property::SHIELDS_CHAOS, Property::SHIELDS_NATURE}, 10));
  const ConcreteSkill SPELL_IRON_SKIN = ConcreteSkill(SkillBase::SPELL_IRON_SKIN, {new UnitBonus(Property::SHIELDS,10)} );

  const ConcreteSkill SPELL_RESIST_MAGIC = ConcreteSkill(SkillBase::SPELL_RESIST_MAGIC, {new UnitBonus(Property::RESIST,3)} );
  const WrapSkill SPELL_GUARDIAN_WIND = WrapSkill(SkillBase::SPELL_GUARDIAN_WIND, IMMUNITY_MISSILE);
  const WrapSkill SPELL_FLYING = WrapSkill(SkillBase::SPELL_FLYING, FLYING);
  
  const ConcreteSkill SPELL_BLESS = ConcreteSkill(SkillBase::SPELL_BLESS, UnitBonus::build({Property::RESIST_DEATH, Property::RESIST_CHAOS, Property::SHIELDS_DEATH, Property::SHIELDS_CHAOS}, 3));
  const ConcreteSkill SPELL_ENDURANCE = ConcreteSkill(SkillBase::SPELL_ENDURANCE, {new UnitBonus(Property::MOVEMENT,1)} );
  const WrapSkill SPELL_HEROISM = WrapSkill(SkillBase::SPELL_HEROISM, FLYING); // TODO
  const ConcreteSkill SPELL_HOLY_ARMOR = ConcreteSkill(SkillBase::SPELL_HOLY_ARMOR, {new UnitBonus(Property::SHIELDS,2)}); // TODO: should be correct
  const ConcreteSkill SPELL_HOLY_WEAPON = ConcreteSkill(SkillBase::SPELL_HOLY_WEAPON, {new SkillEffect(SkillEffect::Type::MAGIC_WEAPONS), new UnitBonus(Property::MELEE,1)});
  const WrapSkill SPELL_PLANAR_TRAVEL = WrapSkill(SkillBase::SPELL_PLANAR_TRAVEL, PLANAR_TRAVEL);
  const WrapSkill SPELL_TRUE_SIGHT = WrapSkill(SkillBase::SPELL_TRUE_SIGHT, IMMUNITY_ILLUSIONS);

}

const Skill* Skills::MELD = &skillimpl::MELD;
const Skill* Skills::CREATE_OUTPOST = &skillimpl::CREATE_OUTPOST;
const Skill* Skills::CREATE_ROAD = &skillimpl::CREATE_ROAD;
const Skill* Skills::WALL_CRUSHING = &skillimpl::WALL_CRUSHING; // maybe should be combat?
const Skill* Skills::PURIFY = &skillimpl::PURIFY;

const Skill* Skills::LUCKY = &skillimpl::LUCKY;

const Skill* Skills::LARGE_SHIELD = &skillimpl::LARGE_SHIELD;
const Skill* Skills::FIRST_STRIKE = &skillimpl::FIRST_STRIKE;
const Skill* Skills::NEGATE_FIRST_STRIKE = &skillimpl::NEGATE_FIRST_STRIKE;
const Skill* Skills::ARMOR_PIERCING = &skillimpl::ARMOR_PIERCING;
const Skill* Skills::LONG_RANGE = &skillimpl::LONG_RANGE;
const Skill* Skills::ILLUSIONARY_ATTACK = &skillimpl::ILLUSIONARY_ATTACK;

const Skill* Skills::POISON_ATTACK1 = &skillimpl::POISON_ATTACK1;
const Skill* Skills::POISON_ATTACK4 = &skillimpl::POISON_ATTACK4;
const Skill* Skills::POISON_ATTACK6 = &skillimpl::POISON_ATTACK6;
const Skill* Skills::POISON_ATTACK15 = &skillimpl::POISON_ATTACK15;

const Skill* Skills::LIFE_STEALING0 = &skillimpl::LIFE_STEALING0;
const Skill* Skills::LIFE_STEALING3 = &skillimpl::LIFE_STEALING3;
const Skill* Skills::LIFE_STEALING4 = &skillimpl::LIFE_STEALING4;
const Skill* Skills::LIFE_STEALING5 = &skillimpl::LIFE_STEALING5;

const Skill* Skills::IMMOLATION = &skillimpl::IMMOLATION;

const Skill* Skills::FIERY_BREATH5 = &skillimpl::FIERY_BREATH5;
const Skill* Skills::FIERY_BREATH20 = &skillimpl::FIERY_BREATH20;
const Skill* Skills::LIGHTING_BREATH20 = &skillimpl::LIGHTING_BREATH20;

const Skill* Skills::DOOM_GAZE4 = &skillimpl::DOOM_GAZE4;
const Skill* Skills::DEATH_GAZE2 = &skillimpl::DEATH_GAZE2;
const Skill* Skills::DEATH_GAZE4 = &skillimpl::DEATH_GAZE4;
const Skill* Skills::STONE_GAZE1 = &skillimpl::STONE_GAZE1;
const Skill* Skills::STONE_GAZE3 = &skillimpl::STONE_GAZE3;
const Skill* Skills::STONE_GAZE4 = &skillimpl::STONE_GAZE4;
const Skill* Skills::STONE_TOUCH3 = &skillimpl::STONE_TOUCH3;

const Skill* Skills::THROW_WEAPON1 = &skillimpl::THROW_WEAPON1;
const Skill* Skills::THROW_WEAPON2 = &skillimpl::THROW_WEAPON2;

const Skill* Skills::SCOUTING1 = &skillimpl::SCOUTING1;
const Skill* Skills::SCOUTING2 = &skillimpl::SCOUTING2;
const Skill* Skills::SCOUTING3 = &skillimpl::SCOUTING3;

const Skill* Skills::TO_HIT1 = &skillimpl::TO_HIT1;
const Skill* Skills::TO_HIT2 = &skillimpl::TO_HIT2;
const Skill* Skills::TO_HIT3 = &skillimpl::TO_HIT3;

const Skill* Skills::MITHRIL_WEAPONS = &skillimpl::MITHRIL_WEAPONS;
const Skill* Skills::ADAMANTIUM_WEAPONS = &skillimpl::ADAMANTIUM_WEAPONS;

const Skill* Skills::FORESTWALK = &skillimpl::FORESTWALK;
const Skill* Skills::FLYING = &skillimpl::FLYING;
const Skill* Skills::UNDERGROUND = &skillimpl::UNDERGROUND;
const Skill* Skills::MOUNTAINWALK = &skillimpl::MOUNTAINWALK;
const Skill* Skills::NON_CORPOREAL = &skillimpl::NON_CORPOREAL;
const Skill* Skills::PATH_FINDER = &skillimpl::PATH_FINDER;
const Skill* Skills::PLANAR_TRAVEL = &skillimpl::PLANAR_TRAVEL;
const Skill* Skills::TELEPORT = &skillimpl::TELEPORT;
const Skill* Skills::SWIMMING = &skillimpl::SWIMMING;
const Skill* Skills::WINDWALK = &skillimpl::WINDWALK;
const Skill* Skills::DESERTWALK = &skillimpl::DESERTWALK;
const Skill* Skills::SWAMPWALK = &skillimpl::SWAMPWALK;


const Skill* Skills::IMMUNITY_MAGIC = &skillimpl::IMMUNITY_MAGIC;
const Skill* Skills::IMMUNITY_MISSILE = &skillimpl::IMMUNITY_MISSILE;
const Skill* Skills::IMMUNITY_ILLUSIONS = &skillimpl::IMMUNITY_ILLUSIONS;


const Skill* Skills::SPELL_ELDRITCH_WEAPON = &skillimpl::SPELL_ELDRITCH_WEAPON;
const Skill* Skills::SPELL_FLAME_BLADE = &skillimpl::SPELL_FLAME_BLADE;

const Skill* Skills::SPELL_RESIST_ELEMENTS = &skillimpl::SPELL_RESIST_ELEMENTS;
const Skill* Skills::SPELL_GIANT_STRENGTH = &skillimpl::SPELL_GIANT_STRENGTH;
const Skill* Skills::SPELL_STONE_SKIN = &skillimpl::SPELL_STONE_SKIN;
const Skill* Skills::SPELL_SWIMMING = &skillimpl::SPELL_SWIMMING;
const Skill* Skills::SPELL_PATH_FINDER = &skillimpl::SPELL_PATH_FINDER;
const Skill* Skills::SPELL_ELEMENTAL_ARMOR = &skillimpl::SPELL_ELEMENTAL_ARMOR;
const Skill* Skills::SPELL_IRON_SKIN = &skillimpl::SPELL_IRON_SKIN;

const Skill* Skills::SPELL_RESIST_MAGIC = &skillimpl::SPELL_RESIST_MAGIC;
const Skill* Skills::SPELL_GUARDIAN_WIND = &skillimpl::SPELL_GUARDIAN_WIND;
const Skill* Skills::SPELL_FLYING = &skillimpl::SPELL_FLYING;

const Skill* Skills::SPELL_BLESS = &skillimpl::SPELL_BLESS;
const Skill* Skills::SPELL_ENDURANCE = &skillimpl::SPELL_ENDURANCE;
const Skill* Skills::SPELL_HEROISM = &skillimpl::SPELL_HEROISM;
const Skill* Skills::SPELL_HOLY_ARMOR = &skillimpl::SPELL_HOLY_ARMOR;
const Skill* Skills::SPELL_HOLY_WEAPON = &skillimpl::SPELL_HOLY_WEAPON;
const Skill* Skills::SPELL_PLANAR_TRAVEL = &skillimpl::SPELL_PLANAR_TRAVEL;
const Skill* Skills::SPELL_TRUE_SIGHT = &skillimpl::SPELL_TRUE_SIGHT;

const Skill* Skills::HERO_AGILITY = &skillimpl::HERO_AGILITY;
const Skill* Skills::HERO_ARMS_MASTER = &skillimpl::HERO_ARMS_MASTER;
const Skill* Skills::HERO_BLADE_MASTER = &skillimpl::HERO_BLADE_MASTER;
const Skill* Skills::HERO_CHARMED = &skillimpl::HERO_CHARMED;
const Skill* Skills::HERO_CONSTITUTION = &skillimpl::HERO_CONSTITUTION;
const Skill* Skills::HERO_LUCK = &skillimpl::HERO_LUCK;
const Skill* Skills::HERO_MIGHT = &skillimpl::HERO_MIGHT;
const Skill* Skills::HERO_PRAYER_MASTER = &skillimpl::HERO_PRAYER_MASTER;



