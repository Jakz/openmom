#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include "Font.h"

#include <iostream>

TEST_CASE("grouping of numbers formatting") {
  REQUIRE(Fonts::groupDigits(1) == "1");
  REQUIRE(Fonts::groupDigits(1000) == "1,000");
}

#include "UnitSpec.h"
#include "Skill.h"

TEST_CASE("basic stats of units are correct") {
  
  SECTION("basic hit and defend chances") {
    const UnitSpec* spec = UnitSpec::unitSpec(UnitID::SPEARMEN, RaceID::BARBARIANS);
    
    REQUIRE(spec != nullptr);
    
    THEN("basic to hit and to defend should be 30%") {
      REQUIRE(spec->getProperty(Property::TO_HIT) == 30);
      REQUIRE(spec->getProperty(Property::TO_DEFEND) == 30);
    }
  }
  
  GIVEN("large shield modifier") {
    const UnitSpec* spec = UnitSpec::unitSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS);
    
    REQUIRE(spec != nullptr);
    REQUIRE(spec->skills.hasSkill(SkillBase::LARGE_SHIELD));
    
    THEN("ranged and magic defense should be +2 compared to normal defense") {
      REQUIRE(spec->getProperty(Property::SHIELDS) == spec->getProperty(Property::SHIELDS_CHAOS) - 2);
      REQUIRE(spec->getProperty(Property::SHIELDS) == spec->getProperty(Property::SHIELDS_RANGED) - 1);
    }
  }
  
}
