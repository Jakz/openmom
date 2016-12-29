#include "Items.h"

#include "Skill.h"
#include "UnitSpec.h"

#include "Localization.h"


using namespace items;

//const PowerEnchant ItemEnchants::POWER_BLESS = PowerEnchant(new WrapSkill(SkillBase::ITEM_BLESS, Skills::BLESS), LIFE, 2)

// TODO
//const ItemClassSpec ItemEnchants::SPEC_SWORD

// SWORD = 0, MACE, AXE, BOW, STAFF, WAND, MISC, SHIELD, CHAIN, PLATE

#pragma mark Affixes

static const PropertyAffixSpec melee_attack_affix = PropertyAffixSpec(Property::MELEE, "Attack", {1,2,3,4,5,6}, {50,100,200,350,550,800});
static const PropertyAffixSpec ranged_magical_attack_affix = PropertyAffixSpec(Property::RANGED, "Attack", {1,2,3,4,5,6}, {50,100,200,350,550,800});
static const PropertyAffixSpec ranged_melee_attack_affix = PropertyAffixSpec(Property::RANGED, "Attack", {1,2,3,4,5,6}, {50,100,200,350,550,800});
static const PropertyAffixSpec defense_affix = PropertyAffixSpec(Property::SHIELDS, "Defense", {1,2,3,4,5,6}, {50,100,200,350,550,800});
static const PropertyAffixSpec movement_affix = PropertyAffixSpec(Property::MOVEMENT, "Movement", {1,2,3,4}, {100,200,400,800});
static const PropertyAffixSpec resistance_affix = PropertyAffixSpec(Property::RESIST, "Resistance", {1,2,3,4,5,6}, {50,100,200,350,550,800});
static const PropertyAffixSpec spell_skill_affix = PropertyAffixSpec(Property::SPELL_SKILL, "Spell Skill", {5,10,15,20}, {200,400,800,1600});
static const PropertyAffixSpec spell_save_affix = PropertyAffixSpec(Property::SPELL_SKILL, "Spell Save", {-1,-2,-3,-4}, {100,200,400,800});
static const PropertyAffixSpec to_hit_affix = PropertyAffixSpec(Property::TO_HIT, "To Hit", {1,2,3}, {400,800,1200});


static const std::vector<PropertyAffixSpec> sword_affixes =
{
  PropertyAffixSpec(melee_attack_affix, 3),
  PropertyAffixSpec(defense_affix, 3),
  PropertyAffixSpec(to_hit_affix, 3),
  PropertyAffixSpec(spell_skill_affix, 2)
};

static const std::vector<PropertyAffixSpec> mace_affixes =
{
  PropertyAffixSpec(melee_attack_affix, 4),
  PropertyAffixSpec(defense_affix, 1),
  PropertyAffixSpec(to_hit_affix, 3),
  PropertyAffixSpec(spell_skill_affix, 2)
};

static const std::vector<PropertyAffixSpec> axe_affixes =
{
  PropertyAffixSpec(melee_attack_affix, 6),
  PropertyAffixSpec(to_hit_affix, 2),
  PropertyAffixSpec(spell_skill_affix, 2)
};

static const std::vector<PropertyAffixSpec> bow_affixes =
{
  PropertyAffixSpec(ranged_melee_attack_affix, 6),
  PropertyAffixSpec(defense_affix, 3),
  PropertyAffixSpec(to_hit_affix, 3),
  PropertyAffixSpec(spell_skill_affix, 2)
};

static const std::vector<PropertyAffixSpec> staff_affixes =
{
  PropertyAffixSpec(ranged_magical_attack_affix, 6),
  PropertyAffixSpec(defense_affix, 3),
  PropertyAffixSpec(to_hit_affix, 3),
  PropertyAffixSpec(spell_skill_affix, 4),
  PropertyAffixSpec(spell_save_affix, 4)
};

static const std::vector<PropertyAffixSpec> wand_affixes =
{
  PropertyAffixSpec(ranged_magical_attack_affix, 2),
  PropertyAffixSpec(to_hit_affix, 1),
  PropertyAffixSpec(spell_skill_affix, 2),
  PropertyAffixSpec(spell_save_affix, 4)
};

static const std::vector<PropertyAffixSpec> jewelry_affixes =
{
  PropertyAffixSpec(melee_attack_affix, 4),
  PropertyAffixSpec(defense_affix, 4),
  PropertyAffixSpec(to_hit_affix, 2),
  PropertyAffixSpec(movement_affix, 3),
  PropertyAffixSpec(resistance_affix, 6),
  PropertyAffixSpec(spell_skill_affix, 3),
  PropertyAffixSpec(spell_save_affix, 4)
};

static const std::vector<PropertyAffixSpec> armor_affixes =
{
  PropertyAffixSpec(defense_affix, 6),
  PropertyAffixSpec(movement_affix, 4),
  PropertyAffixSpec(resistance_affix, 6)
};



static const std::vector<PropertyAffixSpec> empty_affixes = { };

Affixes Affixes::forType(Item::TypeID type)
{
  switch (type)
  {
    case Item::TypeID::SWORD: return { sword_affixes };
    case Item::TypeID::MACE: return { mace_affixes };
    case Item::TypeID::AXE: return { axe_affixes };
    case Item::TypeID::BOW: return { bow_affixes };
    case Item::TypeID::STAFF: return { staff_affixes };
    case Item::TypeID::WAND: return { wand_affixes };
    case Item::TypeID::MISC: return { jewelry_affixes };
    case Item::TypeID::SHIELD:
    case Item::TypeID::CHAIN:
    case Item::TypeID::PLATE:
      return { armor_affixes };
  }
}


static Item::Type types[] = {
  Item::Type(Item::TypeID::SWORD, Item::Class::MELEE, { {I18::ITEM_SWORD, 9} }),
  Item::Type(Item::TypeID::MACE, Item::Class::MELEE, { {I18::ITEM_MACE, 11} }),
  Item::Type(Item::TypeID::AXE, Item::Class::MELEE, { {I18::ITEM_AXE, 9} }),
  Item::Type(Item::TypeID::BOW, Item::Class::RANGED, { {I18::ITEM_BOW, 9} }),
  Item::Type(Item::TypeID::STAFF, Item::Class::STAFF_WAND, { {I18::ITEM_STAFF, 9} }),
  Item::Type(Item::TypeID::WAND, Item::Class::STAFF_WAND, { {I18::ITEM_WAND, 9} }),
  Item::Type(Item::TypeID::MISC, Item::Class::MISC, { {I18::ITEM_AMULET, 6}, {I18::ITEM_RING, 6}, {I18::ITEM_CLOAK, 6}, {I18::ITEM_GAUNTLET, 4}, {I18::ITEM_HELM, 7}, {I18::ITEM_ORB, 6} }),
  Item::Type(Item::TypeID::SHIELD, Item::Class::ARMOR, { {I18::ITEM_SHIELD, 10} }),
  Item::Type(Item::TypeID::CHAIN, Item::Class::ARMOR, { {I18::ITEM_CHAIN_MAIL, 8} }),
  Item::Type(Item::TypeID::PLATE, Item::Class::ARMOR, { {I18::ITEM_PLATE_MAIL, 7} }),
};

const Item::Type* Item::typeForItem(Item::TypeID type)
{
  return &types[static_cast<u16>(type)];
}


static Slots slots[] = {
  Slots({{ {Item::Class::MELEE,Item::Class::RANGED}, {Item::Class::ARMOR}, {Item::Class::MISC} }}, {{145, 149, 148}}),
  Slots({{ {Item::Class::MELEE,Item::Class::STAFF_WAND}, {Item::Class::ARMOR}, {Item::Class::MISC} }}, {{146, 149, 148}}),
  Slots({{ {Item::Class::STAFF_WAND}, {Item::Class::MISC}, {Item::Class::MISC} }}, {{147, 148, 148}}),
  Slots({{ {Item::Class::MELEE},{Item::Class::ARMOR},{Item::Class::MISC} }}, {{144, 149, 148}})
};

const Slots* Slots::slotsFor(Slots::Type type)
{
  return &slots[static_cast<u16>(type)];
}


