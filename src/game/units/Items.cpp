#include "Items.h"

#include "Skill.h"
#include "Localization.h"

using namespace ItemEnchant;

//const PowerEnchant ItemEnchants::POWER_BLESS = PowerEnchant(new WrapSkill(SkillBase::ITEM_BLESS, Skills::BLESS), LIFE, 2)

// TODO
//const ItemClassSpec ItemEnchants::SPEC_SWORD


static Item::Type types[] = {
  Item::Type(Item::TypeID::SWORD, Item::Class::MELEE, 0, { {I18::ITEM_SWORD, 9} }),
  Item::Type(Item::TypeID::MACE, Item::Class::MELEE, 9, { {I18::ITEM_MACE, 11} }),
  Item::Type(Item::TypeID::AXE, Item::Class::MELEE, 20, { {I18::ITEM_AXE, 9} }),
  Item::Type(Item::TypeID::BOW, Item::Class::RANGED, 29, { {I18::ITEM_BOW, 9} }),
  Item::Type(Item::TypeID::STAFF, Item::Class::STAFF_WAND, 38, { {I18::ITEM_STAFF, 9} }),
  Item::Type(Item::TypeID::MISC, Item::Class::MISC, 107, { {I18::ITEM_AMULET, 6}, {I18::ITEM_RING, 6}, {I18::ITEM_CLOAK, 6}, {I18::ITEM_GAUNTLET, 4}, {I18::ITEM_HELM, 7}, {I18::ITEM_ORB, 6} }),
  Item::Type(Item::TypeID::SHIELD, Item::Class::ARMOR, 62, { {I18::ITEM_SHIELD, 10} }),
  Item::Type(Item::TypeID::CHAIN, Item::Class::ARMOR, 47, { {I18::ITEM_CHAIN_MAIL, 8} }),
  Item::Type(Item::TypeID::PLATE, Item::Class::ARMOR, 55, { {I18::ITEM_PLATE_MAIL, 7} }),
};

static const Item::Type* typeForItem(Item::TypeID type)
{
  return &types[static_cast<u16>(type)];
}


static ItemSlots slots[] = {
  ItemSlots({{ {Item::Class::MELEE,Item::Class::RANGED}, {Item::Class::ARMOR}, {Item::Class::MISC} }}, {{145, 149, 148}}),
  ItemSlots({{ {Item::Class::MELEE,Item::Class::STAFF_WAND}, {Item::Class::ARMOR}, {Item::Class::MISC} }}, {{146, 149, 148}}),
  ItemSlots({{ {Item::Class::STAFF_WAND}, {Item::Class::MISC}, {Item::Class::MISC} }}, {{147, 148, 148}}),
  ItemSlots({{ {Item::Class::MELEE},{Item::Class::ARMOR},{Item::Class::MISC} }}, {{144, 149, 148}})
};

const ItemSlots* ItemSlots::slotsFor(ItemSlots::Type type)
{
  return &slots[static_cast<u16>(type)];
}
