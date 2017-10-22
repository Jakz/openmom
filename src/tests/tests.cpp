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

#include "Data.h"
#include "Unit.h"
#include "UnitSpec.h"
#include "Skill.h"

struct PropertyModifier
{
  Property property;
  s16 value;
};

static const std::unordered_map<Property, std::string, enum_hash> propertyNames = {
  { Property::SHIELDS, "shields" },
  { Property::SHIELDS_LIFE, "shields_life" },
  { Property::SHIELDS_DEATH, "shields_death" },
  { Property::SHIELDS_SORCERY, "shields_sorcery" },
  { Property::SHIELDS_CHAOS, "shields_chaos" },
  { Property::SHIELDS_NATURE, "shields_nature" },
  
  { Property::RESIST, "resistance" },
  { Property::RESIST_LIFE, "resistance_life" },
  { Property::RESIST_DEATH, "resistance_death" },
  { Property::RESIST_SORCERY, "resistance_sorcery" },
  { Property::RESIST_CHAOS, "resistance_chaos" },
  { Property::RESIST_NATURE, "resistance_nature" }
};

std::ostream& operator<<(std::ostream& out, Property p) { out << propertyNames.find(p)->second; return out; }

namespace test
{
  using unit_ptr = std::unique_ptr<Unit>;
  using unit_pair = std::pair<unit_ptr, unit_ptr>;
  using identifier = const std::string&;
  using identifier_list = const std::initializer_list<std::string>&;
  
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
  

  
  
  void testModifiers(const Unit* unit1, const Unit* unit2, const std::initializer_list<PropertyModifier>& modifiers)
  {
    for (Property p : properties)
    {
      auto it = std::find_if(modifiers.begin(), modifiers.end(), [&p](const PropertyModifier& mod) { return mod.property == p; });
      
      if (it == modifiers.end())
      {
        if (unit1->getProperty(p) != unit2->getProperty(p))
          FAIL("Property " << p << " should be equal for both units but it's " << unit1->getProperty(p) << " != " << unit2->getProperty(p));
        //return false;
      }
      else
      {
        if (unit1->getProperty(p) != (unit2->getProperty(p) + it->value))
          FAIL("Property " << p << " should match equation for both units but it's " << unit1->getProperty(p) << " != " << unit2->getProperty(p) << " + " << it->value);

      }
    }
  }
  void testModifiers(const unit_ptr& unit1, const unit_ptr& unit2, const std::initializer_list<PropertyModifier>& modifiers) { testModifiers(unit1.get(), unit2.get(), modifiers); }

  
  unit_ptr raceUnit(identifier unitSpec) { return unit_ptr(new RaceUnit(Data::unit(unitSpec)->as<RaceUnitSpec>())); }
  unit_ptr raceUnitWithSkills(identifier unitSpec, identifier_list skills)
  {
    auto unit = raceUnit(unitSpec);
    
    for (const std::string& skill : skills)
      unit->skills()->add(Data::skill(skill));

    return unit;
  }
  
  unit_pair raceUnitPairWithSkills(identifier unitSpec, identifier_list skills)
  {
    return std::make_pair(raceUnit(unitSpec), raceUnitWithSkills(unitSpec, skills));
  }
  
  unit_ptr anyRaceUnit() { return raceUnit("barbarian_swordsmen"); }
  unit_ptr anyRaceUnitWithSkills(identifier_list spellSkills)
  {
    return raceUnitWithSkills("barbarian_swordsmen", spellSkills);
  }
  
  unit_pair anyRaceUnitPairWithSkills(identifier_list skills)
  {
    return std::make_pair(raceUnit("barbarian_swordsmen"), raceUnitWithSkills("barbarian_swordsmen", skills));
  }
  
  effect_list effectListWithSkills(identifier_list skills)
  {
    effect_list effects;
    
    std::for_each(skills.begin(), skills.end(), [&effects] (identifier ident) {
      const Skill* skill = Data::skill(ident);
      effects += skill->getEffects();
    });
    
    return effects;
  }
}

TEST_CASE("direction mask operators") {
  REQUIRE(DirJoin::N << 1 == DirJoin::NW);
  REQUIRE(DirJoin::N << 2 == DirJoin::W);
}

TEST_CASE("grouping of numbers formatting") {
  REQUIRE(strings::groupDigits(1) == "1");
  REQUIRE(strings::groupDigits(1000) == "1,000");
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
      const auto pair = test::anyRaceUnitPairWithSkills({"spell_resist_elements"});
      test::testModifiers(pair.second, pair.first, {
        { Property::SHIELDS_NATURE, 3 },
        { Property::SHIELDS_CHAOS, 3 },
        { Property::RESIST_NATURE, 3 },
        { Property::RESIST_CHAOS, 3 }
      });
    }
  }
}

TEST_CASE("skill effects groups") {
  SECTION("keep greater group") {
    GIVEN("two skill effects, one more powerful") {
      const auto unit = test::anyRaceUnit();
      const effect_list effects = test::effectListWithSkills({"resistance_to_all_2", "resistance_to_all_1"});
      const effect_list actuals = effects.actuals(unit.get());
      
      THEN("only the most powerful is kept") {
        REQUIRE(actuals.size() == 1);
        REQUIRE(*actuals.begin() == *effects.begin());
      }
    
    }
  }
}
