#pragma once

#include "common/Common.h"

#include "Effects.h"

struct Retort
{
  struct Requirement
  {
    enum class Type : u8
    {
      ANY_SCHOOL_AT_LEAST,
      SPECIFIC_SCHOOL_AT_LEAST
    };

    Type type;
    School school;
    u8 count;
    u8 times;

    bool isSatisfied(const school_value_map& books) const
    {
      switch (type)
      {
        case Type::SPECIFIC_SCHOOL_AT_LEAST: return books[school] >= count;
        case Type::ANY_SCHOOL_AT_LEAST:
        {
          u32 valid = 0;
          for (const auto& c : books)
            valid += c >= count ? 1 : 0;

          return valid >= times;
        }
      }

      return false;
    }
  };

  const std::string identifier;
  const value_t cost;
  std::vector<Requirement> requirements;
  wizard_effect_list effects;

  I18 i18n; //TODO: should be moved in GfxData?

  Retort(const std::string& identifier, value_t cost, const wizard_effect_list& effects) : identifier(identifier), cost(cost), effects(effects) { }
  Retort(const std::string& identifier, value_t cost, const std::vector<Requirement>& requirements) : identifier(identifier), cost(cost), requirements(requirements) { }

  bool canBePicked(u16 availablePicks, const school_value_map& books) const
  {
    if (availablePicks < cost)
      return false;

    return std::all_of(requirements.begin(), requirements.end(), [&books](const Requirement& req) { return req.isSatisfied(books); });
  }

  bool operator==(const std::string& identifier) const { return this->identifier == identifier; }
};

struct Wizard
{
  school_value_map defaultBooks;
  std::set<const Retort*> defaultRetorts;

  Wizard() : defaultBooks(0) { }
};

struct PlayerSetupInfo
{
  const Wizard* portrait;
  const Race* race;
  std::string name;
  school_value_map books;
  spell_enum_map<std::set<const Spell*>> spells;
  std::set<const Retort*> retorts;
  PlayerColor color;
};

using RetortModifierEffect = SpecificModifierEffect<WizardEffect, WizardEffectType::WIZARD_BONUS, WizardModifierValue, WizardAttribute>;