#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include "Font.h"

#include <iostream>

TEST_CASE("grouping of numbers formatting") {
  REQUIRE(Fonts::groupDigits(1) == "1");
  REQUIRE(Fonts::groupDigits(1000) == "1,000");
}

#include "Unit.h"
#include "UnitSpec.h"
#include "Skill.h"

TEST_CASE("basic stats of units") {
  
  SECTION("basic hit and defend chances") {
    const UnitSpec* spec = UnitSpec::unitSpec(UnitID::SPEARMEN, RaceID::BARBARIANS);
    
    REQUIRE(spec != nullptr);
    
    THEN("basic to hit and to defend should be 30%") {
      REQUIRE(spec->getProperty(Property::TO_HIT) == 30);
      REQUIRE(spec->getProperty(Property::TO_DEFEND) == 30);
    }
  }
  
  SECTION("large shield modifier") {
    const RaceUnitSpec* spec = UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS);
    Unit* unit = new RaceUnit(spec);

    REQUIRE(spec != nullptr);
    REQUIRE(unit->skills()->hasSkill(Skills::LARGE_SHIELD));
    
    THEN("ranged and magic defense should be +2 compared to normal defense") {
      REQUIRE(unit->getProperty(Property::SHIELDS) == (unit->getProperty(Property::SHIELDS_CHAOS) - 2));
      REQUIRE(unit->getProperty(Property::SHIELDS) == (unit->getProperty(Property::SHIELDS_RANGED) - 2));
    }
    
    delete unit;
  }
  
  SECTION("to hit modifiers") {
    const RaceUnitSpec* spec = UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS);
    
    GIVEN("holy weapon spell") {
      Unit* unit = new RaceUnit(spec);
      unit->skills()->add(Skills::SPELL_HOLY_WEAPON);
      REQUIRE(spec->getProperty(Property::TO_HIT) == 30);

      THEN("holy weapon modifies to hit") {
        REQUIRE(unit->getProperty(Property::TO_HIT) == 40);
      }
      
      delete unit;
    }
  }

  
}
