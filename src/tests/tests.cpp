#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include "mystrings.h"

#include <iostream>

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
    REQUIRE(unit->skills()->hasSkill(Skills::LARGE_SHIELD));
    
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

  
}
