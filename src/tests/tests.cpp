#define CATCH_CONFIG_RUNNER
#include "tests/catch.hpp"

#include "save/Data.h"
#include "save/YAML.h"

#include "mystrings.h"

#include <iostream>

int main( int argc, char* argv[] )
{
  yaml::parse();
  int result = Catch::Session().run( argc, argv );
  return result < 0xff ? result : 0xff;
}

TEST_CASE("direction mask operators") {
  REQUIRE(DirJoin::N << 1 == DirJoin::NW);
  REQUIRE(DirJoin::N << 2 == DirJoin::W);
}

TEST_CASE("grouping of numbers formatting") {
  REQUIRE(strings::groupDigits(1) == "1");
  REQUIRE(strings::groupDigits(1000) == "1,000");
}

#include "Data.h"
#include "Unit.h"
#include "UnitSpec.h"
#include "Skill.h"

struct PropertyModifier
{
  Property property;
  s16 value;
};

namespace test
{
  static const std::array<Property, 12> properties = { {
    Property::SHIELDS,
    Property::SHIELDS_LIFE,
    Property::SHIELDS_DEATH,
    Property::SHIELDS_SORCERY,
    Property::SHIELDS_CHAOS,
    Property::SHIELDS_NATURE,
    
    Property::RESIST,
    Property::RESIST_LIFE,
    Property::RESIST_DEATH,
    Property::RESIST_SORCERY,
    Property::RESIST_CHAOS,
    Property::RESIST_NATURE,
  } };
  
  bool testModifiers(const Unit* unit, const std::initializer_list<PropertyModifier>& modifiers)
  {
    for (Property p : properties)
    {
      auto it = std::find_if(modifiers.begin(), modifiers.end(), [&p](const PropertyModifier& mod) { return mod.property == p; });
      
      if (it == modifiers.end() && unit->getProperty(p) != unit->spec->getProperty(p))
        return false;
      else if (unit->getProperty(p) != (unit->spec->getProperty(p) + it->value))
        return false;
    }
    
    return true;
  }
  
  std::unique_ptr<Unit> raceUnitWithSpells(const std::string& unitSpec, const std::initializer_list<std::string>& spellSkills)
  {
    const UnitSpec* spec = Data::unit(unitSpec);
    Unit* unit = new RaceUnit(spec->as<RaceUnitSpec>());
    
    for (const std::string& spellSkill : spellSkills)
      unit->skills()->add(Data::skill(spellSkill));

    return std::unique_ptr<Unit>(unit);
  }
  
  std::unique_ptr<Unit> anyRaceUnitWithSpells(const std::initializer_list<std::string>& spellSkills)
  {
    return raceUnitWithSpells("barbarian_swordsmen", spellSkills);
  }
}



TEST_CASE("basic stats of units") {
  
  SECTION("basic hit and defend chances") {
    const UnitSpec* spec = Data::unit("barbarian_spearmen");
    
    REQUIRE(spec != nullptr);
    
    THEN("basic to hit and to defend should be 30%") {
      REQUIRE(spec->getProperty(Property::TO_HIT) == 30);
      REQUIRE(spec->getProperty(Property::TO_DEFEND) == 30);
    }
  }
  
  SECTION("large shield modifier") {
    const UnitSpec* spec = Data::unit("barbarian_swordsmen");
    Unit* unit = new RaceUnit(spec->as<RaceUnitSpec>());

    REQUIRE(spec != nullptr);
    REQUIRE(unit->skills()->hasSkill(Data::skill("large_shield")));
    
    THEN("ranged and magic defense should be +2 compared to normal defense") {
      REQUIRE(unit->getProperty(Property::SHIELDS) == (unit->getProperty(Property::SHIELDS_CHAOS) - 2));
      REQUIRE(unit->getProperty(Property::SHIELDS) == (unit->getProperty(Property::SHIELDS_RANGED) - 2));
    }
    
    delete unit;
  }
  
  SECTION("to hit modifiers") {
    const UnitSpec* spec = Data::unit("barbarian_swordsmen");
    
    GIVEN("holy weapon spell") {
      Unit* unit = new RaceUnit(spec->as<RaceUnitSpec>());
      unit->skills()->add(Skills::SPELL_HOLY_WEAPON);
      REQUIRE(spec->getProperty(Property::TO_HIT) == 30);

      THEN("holy weapon modifies to hit") {
        REQUIRE(unit->getProperty(Property::TO_HIT) == 40);
      }
      
      delete unit;
    }
  }
  
  SECTION("nature unit spells") {
    
    GIVEN("resist elements spell") {
      const auto unit = test::anyRaceUnitWithSpells({"spell_resist_elements"});
      REQUIRE(test::testModifiers(unit.get(), { {Property::SHIELDS_NATURE, 3}, {Property::SHIELDS_CHAOS, 3} }));
    }
  }

  
}
