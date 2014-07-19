#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdint>
#include <SDL2/SDL.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

typedef u32 Color;

enum I18 : u16;
enum TextureID : u16;
enum Plane : u8;

enum MouseButton : u8
{
  BUTTON_LEFT = SDL_BUTTON_LEFT,
  BUTTON_RIGHT = SDL_BUTTON_RIGHT,
  BUTTON_MIDDLE = SDL_BUTTON_MIDDLE
};


struct Position
{
  s16 x, y;
  Plane plane;
};

struct SpriteInfo
{
  TextureID texture;
  s16 x;
  s16 y;
};






#include <vector>

enum Plane : u8
{
  ARCANUS = 0,
  MYRRAN
};

enum School : s8
{
  ARCANE = 0,
  CHAOS,
  DEATH,
  LIFE,
  NATURE,
  SORCERY,
  
  SCHOOL_COUNT,
  
  NO_SCHOOL = -1
};

enum PlayerColor : u8
{
  GREEN = 0,
  BLUE,
  RED,
  PURPLE,
  YELLOW,
  NEUTRAL
};

enum WizardID : u8
{
  MERLIN = 0,
  RAVEN,
  SHAREE,
  LO_PAN,
  JAFAR,
  OBERIC,
  RJAK,
  SSS_RA,
  TAURON,
  FREYA,
  HORUS,
  ARIEL,
  TLALOC,
  KALI
};

struct Wizard
{
  I18 name;
};

enum TraitID : u8
{
  TRAIT_ALCHEMY = 0,
  TRAIT_WARLORD,
  TRAIT_CHANNELER,
  TRAIT_ARCHMAGE,
  TRAIT_ARTIFICER,
  TRAIT_CONJURER,
  TRAIT_SAGE_MASTER,
  TRAIT_MYRRAN,
  TRAIT_DIVINE_POWER,
  TRAIT_FAMOUS,
  TRAIT_RUNEMASTER,
  TRAIT_CHARISMATIC,
  TRAIT_CHAOS_MASTERY,
  TRAIT_NATURE_MASTERY,
  TRAIT_SORCERY_MASTERY,
  TRAIT_INFERNAL_POWER,
  TRAIT_MANA_FOCUSING,
  TRAIT_NODE_MASTERY
};

struct Trait
{
  const TraitID ident;
  const u8 cost;
  const School school;
};

enum SpellRarity : u8
{
  RARITY_COMMON = 0,
  RARITY_UNCOMMON,
  RARITY_RARE,
  RARITY_VERY_RARE,
  
  RARITY_COUNT
};

enum SpellKind : u8
{
  KIND_SUMMONING = 0,
  KIND_SPECIAL,
  KIND_CITY,
  KIND_ENCHANTMENT,
  KIND_UNIT_SPELL,
  KIND_COMBAT_SPELL,
  
  KIND_COUNT
};

enum SpellDuration : u8
{
  COMBAT_INSTANT,
  CONTINUOUS,
  COMBAT_CONTINUOUS,
  COMBAT_ENCHANTMENT,
  PERMANENT,
  UNDEFINED
};

enum SpellTarget : u8
{
  TARGET_FRIENDLY_UNIT,
  TARGET_ENEMY_UNIT,
  TARGET_FRIENDLY_CITY,
  TARGET_ENEMY_CITY,
  TARGET_ENEMY_UNIT_SPELL,
  TARGET_FRIENDLY_ARMY,
  TARGET_ENEMY_ARMY,
  TARGET_BOTH_ARMIES,
  TARGET_MAP_TILE,
  TARGET_VIEWPORT,
  TARGET_GLOBAL,
  
  TARGET_NONE
};

class Data
{
  public:
    static Color colorForSchool(const School school);
    static const Trait& trait(const TraitID trait);
    static const Wizard& wizard(const WizardID wizard);
  
    static const std::vector<const SpellKind>& spellKinds(bool combat);
};

#endif
