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
    
    if (effect->type == SkillEffect::Type::PROPERTY_BONUS)
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


const ConcreteSkill Skills::IMMUNITY_MAGIC = ConcreteSkill(SkillBase::IMMUNITY_MAGIC, {new SimpleEffect(SkillEffect::Type::IMMUNITY, SimpleEffect::Type::IMMUNITY_MAGIC), new UnitBonus(Property::RESIST, 50)} );
const ConcreteSkill Skills::IMMUNITY_MISSILE = ConcreteSkill(SkillBase::IMMUNITY_MISSILE, {new SimpleEffect(SkillEffect::Type::IMMUNITY, SimpleEffect::Type::IMMUNITY_MISSILE), new UnitBonus(Property::SHIELDS_RANGED, 50)} );
const ConcreteSkill Skills::IMMUNITY_ILLUSIONS = ConcreteSkill(SkillBase::IMMUNITY_ILLUSIONS, {new SimpleEffect(SkillEffect::Type::IMMUNITY, SimpleEffect::Type::IMMUNITY_ILLUSIONS)} );

const ConcreteSkill Skills::LUCKY = ConcreteSkill(SkillBase::LUCKY, UnitBonus::build({Property::TO_HIT, Property::TO_DEFEND, Property::RESIST}, 1));


const ConcreteSkill Skills::LARGE_SHIELD = ConcreteSkill(SkillBase::LARGE_SHIELD, {new UnitBonus(Property::SHIELDS_RANGED,2)} );


