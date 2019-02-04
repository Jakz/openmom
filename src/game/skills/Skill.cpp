#include "Skill.h"

#include "Localization.h"
#include "UnitSpec.h"

// TODO: remove after refactor
#include "Texture.h"

using namespace std;

const std::string skills::ConcreteSkill::name() const
{
  std::string base = i18n::s(_visual.name);
  if (_visual.hideValue) return base;
  else
  {
    const UnitEffect *effect = _effects[0];
    
    if (effect->type == UnitEffectType::UNIT_BONUS)
      return string("+") + to_string(static_cast<const UnitPropertyBonus*>(effect)->modifier().truncatedValue()) + " " + base;
    else if (_effects[0]->type == UnitEffectType::SPECIAL_ATTACK)
      return base + " (" + to_string(effect->as<SpecialAttackEffect>()->strength()) + ")";
  }
  
  return base;
}

bool Skill::Comparator::operator()(const Skill *b1, const Skill *b2) const
{
  /* TODO: reenable after new management is complete by using skill->type */
  /*if (isHeroBase(b1->base)) return true;
  else if (isHeroBase(b2->base)) return false;
  
  if (isMovementBase(b1->base)) return true;
  else if (isMovementBase(b2->base)) return false;*/
  
  return i18n::s(b1->base).compare(i18n::s(b2->base)) < 0;
}





SpriteInfo Skill::icon() const
{
  return SpriteInfo(TextureID::UNIT_SKILL_ICONS, static_cast<u16>(base));
}


const string Skill::name() const
{
  return i18n::s(base);
}

const string ConcreteSkill::name() const
{
  if (hideValue) return Skill::name();
  else
  {
    const UnitEffect *effect = effects[0];
    
    if (effect->type == UnitEffectType::UNIT_BONUS)
      return string("+") + to_string(static_cast<const UnitPropertyBonus*>(effect)->modifier().truncatedValue()) + " " + Skill::name();
    else if (effects[0]->type == UnitEffectType::SPECIAL_ATTACK)
      return Skill::name() + " (" + to_string(effect->as<SpecialAttackEffect>()->strength()) + ")";
  }
  
  return "";
}

bool ConcreteSkill::hasSimpleEffect(SimpleEffect::Type type)
{
  auto it = find_if(effects.begin(), effects.end(), [&](const UnitEffect* effect) {
    switch (effect->type)
    {
      case UnitEffectType::IMMUNITY:
      case UnitEffectType::MOVEMENT:
      case UnitEffectType::ABILITY:
        return static_cast<const SimpleEffect*>(effect)->effect == type;
      default: return false;
    }
  });
  
  return it != effects.end();
}

const SpecialAttackEffect* ConcreteSkill::hasEffect(SimpleEffect::Type ident)
{
  auto it = find_if(effects.begin(), effects.end(), [&](const UnitEffect* effect) { return effect->type == UnitEffectType::SPECIAL_ATTACK && static_cast<const SimpleEffect*>(effect)->effect == ident; });
  return it != effects.end() ? static_cast<const SpecialAttackEffect*>(*it) : nullptr;
}

namespace skillimpl
{
  static const ConcreteSkill CREATE_ROAD = ConcreteSkill(SkillBase::CREATE_ROAD, {new SimpleEffect(UnitEffectType::ABILITY, SimpleEffect::Type::CREATE_ROAD)});
  static const ConcreteSkill WALL_CRUSHING = ConcreteSkill(SkillBase::WALL_CRUSHING, {new SimpleEffect(UnitEffectType::ABILITY, SimpleEffect::Type::WALL_CRUSHING)}); // maybe should be combat?
  
  //static const ConcreteSkill LUCKY = ConcreteSkill(SkillBase::LUCKY, unit_bonus_build({ Property::TO_HIT, Property::TO_DEFEND, Property::RESIST }, 1));
  
  static const ConcreteSkill NEGATE_FIRST_STRIKE = ConcreteSkill(SkillBase::NEGATE_FIRST_STRIKE, {new SimpleEffect(UnitEffectType::ABILITY, SimpleEffect::Type::NEGATE_FIRST_STRIKE)} );
  static const ConcreteSkill ARMOR_PIERCING = ConcreteSkill(SkillBase::FIRST_STRIKE, {new SimpleEffect(UnitEffectType::ABILITY, SimpleEffect::Type::ARMOR_PIERCING)} );
  static const ConcreteSkill LONG_RANGE = ConcreteSkill(SkillBase::FIRST_STRIKE, {new SimpleEffect(UnitEffectType::ABILITY, SimpleEffect::Type::LONG_RANGE)} );
  static const ConcreteSkill ILLUSIONARY_ATTACK = ConcreteSkill(SkillBase::ILLUSIONARY_ATTACK, {new SimpleEffect(UnitEffectType::ABILITY, SimpleEffect::Type::ILLUSIONARY_ATTACK)} );

  /*static const ConcreteSkill POISON_ATTACK1 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 1)}, false);
  static const ConcreteSkill POISON_ATTACK4 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 4)}, false);
  static const ConcreteSkill POISON_ATTACK6 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 6)}, false);
  static const ConcreteSkill POISON_ATTACK15 = ConcreteSkill(SkillBase::POISON_ATTACK, {new SpecialAttackEffect(SpecialAttackEffect::Type::POISON, 15)}, false);

  static const ConcreteSkill LIFE_STEALING0 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, 0)});
  static const ConcreteSkill LIFE_STEALING3 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, -3)}, false);
  static const ConcreteSkill LIFE_STEALING4 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, -4)}, false);
  static const ConcreteSkill LIFE_STEALING5 = ConcreteSkill(SkillBase::LIFE_STEALING, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIFE_STEALING, -5)}, false);
  
  static const ConcreteSkill IMMOLATION = ConcreteSkill(SkillBase::IMMOLATION, {new SpecialAttackEffect(SpecialAttackEffect::Type::IMMOLATION, 4)});
  
  static const ConcreteSkill LIGHTING_BREATH20 = ConcreteSkill(SkillBase::LIGHTING_BREATH, {new SpecialAttackEffect(SpecialAttackEffect::Type::LIGHTING_BREATH, 20)}, false);
  
  static const ConcreteSkill DOOM_GAZE4 = ConcreteSkill(SkillBase::DOOM_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::DOOM_GAZE, 4)}, false);
  
  static const ConcreteSkill DEATH_GAZE2 = ConcreteSkill(SkillBase::DEATH_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::DEATH_GAZE, -2)}, false);
  static const ConcreteSkill DEATH_GAZE4 = ConcreteSkill(SkillBase::DEATH_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::DEATH_GAZE, -4)}, false);
  
  static const ConcreteSkill STONE_GAZE1 = ConcreteSkill(SkillBase::STONE_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_GAZE, -1)}, false);
  static const ConcreteSkill STONE_GAZE3 = ConcreteSkill(SkillBase::STONE_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_GAZE, -3)}, false);
  static const ConcreteSkill STONE_GAZE4 = ConcreteSkill(SkillBase::STONE_GAZE, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_GAZE, -4)}, false);

  static const ConcreteSkill STONE_TOUCH3 = ConcreteSkill(SkillBase::STONE_TOUCH, {new SpecialAttackEffect(SpecialAttackEffect::Type::STONE_TOUCH, -3)}, false);*/

  // TODO: a parametric throw weapon is needed for hero ability
  
  /**/
  static const ConcreteSkill MITHRIL_WEAPONS = ConcreteSkill(SkillBase::MITHRIL_WEAPONS, {new UnitEffect(UnitEffectType::MAGIC_WEAPONS), new UnitPropertyBonus(Property::MELEE,1)});
  static const ConcreteSkill ADAMANTIUM_WEAPONS = ConcreteSkill(SkillBase::ADAMANTIUM_WEAPONS, {new UnitEffect(UnitEffectType::MAGIC_WEAPONS), new UnitPropertyBonus(Property::MELEE,2)});
  
  static const ConcreteSkill IMMUNITY_MAGIC = ConcreteSkill(SkillBase::IMMUNITY_MAGIC, {new SimpleEffect(UnitEffectType::IMMUNITY, SimpleEffect::Type::IMMUNITY_MAGIC), new UnitPropertyBonus(Property::RESIST, 50)} );
  static const ConcreteSkill IMMUNITY_MISSILE = ConcreteSkill(SkillBase::IMMUNITY_MISSILE, {new SimpleEffect(UnitEffectType::IMMUNITY, SimpleEffect::Type::IMMUNITY_MISSILE), new UnitPropertyBonus(Property::SHIELDS_RANGED, 50)} );
  static const ConcreteSkill IMMUNITY_ILLUSIONS = ConcreteSkill(SkillBase::IMMUNITY_ILLUSIONS, {new SimpleEffect(UnitEffectType::IMMUNITY, SimpleEffect::Type::IMMUNITY_ILLUSIONS)} );
  
  static const ConcreteSkill HERO_AGILITY = ConcreteSkill(SkillBase::HERO_AGILITY, {new UnitPropertyBonus(Property::SHIELDS, 1.0f)});
  // TODO: ARCANE_POWER
  static const ConcreteSkill HERO_ARMS_MASTER = ConcreteSkill(SkillBase::HERO_ARMS_MASTER, {new ArmyPropertyBonus(Property::XP, 2/*TODO , ArmyPropertyBonus::Type::NORMAL_UNITS*/)});
  static const ConcreteSkill HERO_BLADE_MASTER = ConcreteSkill(SkillBase::HERO_BLADE_MASTER, {new UnitPropertyBonus(Property::TO_HIT, 0.5f)}); // TODO: missin effect in mechanics?
  // TODO: CASTER
  static const ConcreteSkill HERO_CHARMED = ConcreteSkill(SkillBase::HERO_CHARMED, {new UnitPropertyBonus(Property::RESIST, 30)}); // TODO: missing effect in mechanics?
  static const ConcreteSkill HERO_CONSTITUTION = ConcreteSkill(SkillBase::HERO_CONSTITUTION, {new UnitPropertyBonus(Property::HIT_POINTS, 1.0f)}); // TODO: missin effect in mechanics?
  // TODO: LEADERSHIP
  // TODO: LEGENDARY
  static const ConcreteSkill HERO_MIGHT = ConcreteSkill(SkillBase::HERO_MIGHT, {new UnitPropertyBonus(Property::MELEE, 1.0f)}); // TODO: missin effect in mechanics?
  // TODO: NOBLE
  static const ConcreteSkill HERO_PRAYER_MASTER = ConcreteSkill(SkillBase::HERO_PRAYER_MASTER, {new ArmyPropertyBonus(Property::RESIST, 1.0f/*, ArmyPropertyBonus::Type::WHOLE_ARMY*/)});
  // TODO: SAGE
  // TODO: SPELL_CASTER
  
  static const ConcreteSkill SPELL_ELDRITCH_WEAPON = ConcreteSkill(SkillBase::MITHRIL_WEAPONS, {new UnitEffect(UnitEffectType::MAGIC_WEAPONS), new CombatBonus(Property::TO_DEFEND, -1, CombatBonus::Phase::ATTACKING, CombatBonus::Target::DEFENDER, false)});

  static const ConcreteSkill SPELL_GIANT_STRENGTH = ConcreteSkill(SkillBase::SPELL_GIANT_STRENGTH, {new UnitPropertyBonus(Property::MELEE,1)} );
  static const ConcreteSkill SPELL_STONE_SKIN = ConcreteSkill(SkillBase::SPELL_STONE_SKIN, {new UnitPropertyBonus(Property::SHIELDS,1)} );
  static const ConcreteSkill SPELL_SWIMMING = ConcreteSkill(SkillBase::SPELL_SWIMMING, {new MovementEffect(MovementType::SWIMMING)});
  static const ConcreteSkill SPELL_PATH_FINDER = ConcreteSkill(SkillBase::SPELL_PATH_FINDER, {new MovementEffect(MovementType::PATH_FINDER)});
  static const ConcreteSkill SPELL_IRON_SKIN = ConcreteSkill(SkillBase::SPELL_IRON_SKIN, {new UnitPropertyBonus(Property::SHIELDS,10)} );

  static const ConcreteSkill SPELL_RESIST_MAGIC = ConcreteSkill(SkillBase::SPELL_RESIST_MAGIC, {new UnitPropertyBonus(Property::RESIST,3)} );
  
  static const ConcreteSkill SPELL_ENDURANCE = ConcreteSkill(SkillBase::SPELL_ENDURANCE, {new UnitPropertyBonus(Property::MOVEMENT,1)} );
  static const ConcreteSkill SPELL_HOLY_ARMOR = ConcreteSkill(SkillBase::SPELL_HOLY_ARMOR, {new UnitPropertyBonus(Property::SHIELDS,2)}); // TODO: should be correct
  static const ConcreteSkill SPELL_HOLY_WEAPON = ConcreteSkill(SkillBase::SPELL_HOLY_WEAPON, {new UnitEffect(UnitEffectType::MAGIC_WEAPONS), new UnitPropertyBonus(Property::TO_HIT,1)});
}

const Skill* Skills::CREATE_ROAD = &skillimpl::CREATE_ROAD;
const Skill* Skills::WALL_CRUSHING = &skillimpl::WALL_CRUSHING; // maybe should be combat?

const Skill* Skills::LUCKY = nullptr;

const Skill* Skills::NEGATE_FIRST_STRIKE = &skillimpl::NEGATE_FIRST_STRIKE;
const Skill* Skills::ARMOR_PIERCING = &skillimpl::ARMOR_PIERCING;
const Skill* Skills::LONG_RANGE = &skillimpl::LONG_RANGE;
const Skill* Skills::ILLUSIONARY_ATTACK = &skillimpl::ILLUSIONARY_ATTACK;

const Skill* Skills::POISON_ATTACK1 = nullptr;//&skillimpl::POISON_ATTACK1;
const Skill* Skills::POISON_ATTACK4 = nullptr;//&skillimpl::POISON_ATTACK4;
const Skill* Skills::POISON_ATTACK6 = nullptr;//&skillimpl::POISON_ATTACK6;
const Skill* Skills::POISON_ATTACK15 = nullptr;//&skillimpl::POISON_ATTACK15;

const Skill* Skills::LIFE_STEALING0 = nullptr;//&skillimpl::LIFE_STEALING0;
const Skill* Skills::LIFE_STEALING3 = nullptr;//&skillimpl::LIFE_STEALING3;
const Skill* Skills::LIFE_STEALING4 = nullptr;//&skillimpl::LIFE_STEALING4;
const Skill* Skills::LIFE_STEALING5 = nullptr;//&skillimpl::LIFE_STEALING5;

const Skill* Skills::IMMOLATION = nullptr;//&skillimpl::IMMOLATION;

const Skill* Skills::LIGHTING_BREATH20 = nullptr;//&skillimpl::LIGHTING_BREATH20;

const Skill* Skills::DOOM_GAZE4 = nullptr;//&skillimpl::DOOM_GAZE4;
const Skill* Skills::DEATH_GAZE2 = nullptr;//&skillimpl::DEATH_GAZE2;
const Skill* Skills::DEATH_GAZE4 = nullptr;//&skillimpl::DEATH_GAZE4;
const Skill* Skills::STONE_GAZE1 = nullptr;//&skillimpl::STONE_GAZE1;
const Skill* Skills::STONE_GAZE3 = nullptr;//&skillimpl::STONE_GAZE3;
const Skill* Skills::STONE_GAZE4 = nullptr;//&skillimpl::STONE_GAZE4;
const Skill* Skills::STONE_TOUCH3 = nullptr;//&skillimpl::STONE_TOUCH3;

const Skill* Skills::MITHRIL_WEAPONS = &skillimpl::MITHRIL_WEAPONS;
const Skill* Skills::ADAMANTIUM_WEAPONS = &skillimpl::ADAMANTIUM_WEAPONS;

const Skill* Skills::IMMUNITY_MAGIC = &skillimpl::IMMUNITY_MAGIC;
const Skill* Skills::IMMUNITY_MISSILE = &skillimpl::IMMUNITY_MISSILE;
const Skill* Skills::IMMUNITY_ILLUSIONS = &skillimpl::IMMUNITY_ILLUSIONS;


const Skill* Skills::SPELL_ELDRITCH_WEAPON = &skillimpl::SPELL_ELDRITCH_WEAPON;
const Skill* Skills::SPELL_FLAME_BLADE = nullptr;

const Skill* Skills::SPELL_RESIST_ELEMENTS = nullptr;
const Skill* Skills::SPELL_GIANT_STRENGTH = &skillimpl::SPELL_GIANT_STRENGTH;
const Skill* Skills::SPELL_STONE_SKIN = &skillimpl::SPELL_STONE_SKIN;
const Skill* Skills::SPELL_SWIMMING = &skillimpl::SPELL_SWIMMING;
const Skill* Skills::SPELL_PATH_FINDER = &skillimpl::SPELL_PATH_FINDER;
const Skill* Skills::SPELL_ELEMENTAL_ARMOR = nullptr;
const Skill* Skills::SPELL_IRON_SKIN = &skillimpl::SPELL_IRON_SKIN;

const Skill* Skills::SPELL_RESIST_MAGIC = &skillimpl::SPELL_RESIST_MAGIC;
const Skill* Skills::SPELL_FLYING = nullptr;//TODO &skillimpl::SPELL_FLYING;

const Skill* Skills::SPELL_ENDURANCE = &skillimpl::SPELL_ENDURANCE;
const Skill* Skills::SPELL_HEROISM = nullptr;//TODO &skillimpl::SPELL_HEROISM;
const Skill* Skills::SPELL_HOLY_ARMOR = &skillimpl::SPELL_HOLY_ARMOR;
const Skill* Skills::SPELL_HOLY_WEAPON = &skillimpl::SPELL_HOLY_WEAPON;
const Skill* Skills::SPELL_PLANAR_TRAVEL = nullptr;//TODO &skillimpl::SPELL_PLANAR_TRAVEL;

const Skill* Skills::HERO_AGILITY = &skillimpl::HERO_AGILITY;
const Skill* Skills::HERO_ARMS_MASTER = &skillimpl::HERO_ARMS_MASTER;
const Skill* Skills::HERO_BLADE_MASTER = &skillimpl::HERO_BLADE_MASTER;
const Skill* Skills::HERO_CHARMED = &skillimpl::HERO_CHARMED;
const Skill* Skills::HERO_CONSTITUTION = &skillimpl::HERO_CONSTITUTION;
const Skill* Skills::HERO_MIGHT = &skillimpl::HERO_MIGHT;
const Skill* Skills::HERO_PRAYER_MASTER = &skillimpl::HERO_PRAYER_MASTER;



