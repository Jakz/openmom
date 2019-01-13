#include "tests.h"

#include "Unit.h"
#include "UnitSpec.h"
#include "Skill.h"

#include "save/Data.h"

#include "game/combat/Combat.h"
#include "game/combat/CombatMechanics.h"

#include "common/mystrings.h"

class DummyEffect : public SkillEffect {
public:
  std::string v;
  DummyEffect(std::string v) : SkillEffect(SkillEffect::Type::MOVEMENT), v(v) { }
};

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
  using cunit_ptr = std::unique_ptr<combat::CombatUnit>;
  using unit_spec_ptr = std::unique_ptr<const UnitSpec>;
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
  
  unit_ptr anyRaceUnit() { return raceUnit("barbarian_berserkers"); }
  unit_ptr anyRaceUnitWithSkills(identifier_list spellSkills)
  {
    return raceUnitWithSkills("barbarian_swordsmen", spellSkills);
  }
  
  cunit_ptr cunit(const unit_ptr& unit) { return std::make_unique<combat::CombatUnit>(nullptr, combat::Side::ATTACKER, unit.get()); }

  
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
  
  unit_spec_ptr anyRaceUnitSpecWithSkills(identifier_list skillNames)
  {
    skill_list skills;
    
    std::for_each(skillNames.begin(), skillNames.end(), [&skills] (identifier ident) {
      const Skill* skill = Data::skill(ident);
      skills.push_back(skill);
    });
    
    return std::unique_ptr<const UnitSpec>(new RaceUnitSpec(Data::race("barbarians"), 1, 10, 1, RangedInfo(), 1, 1, 1, 1, 1, 1, skills));
  }
}

#pragma mark Utilities Functions

TEST_CASE("direction mask operators") {
  REQUIRE((DirJoin::N >> 1) == DirJoin::NW);
  REQUIRE((DirJoin::N >> 2) == DirJoin::W);
  REQUIRE((DirJoin::EDGE_S | DirJoin::EDGE_W) == (DirJoin::EDGE_S | DirJoin::W | DirJoin::NW));
}

TEST_CASE("grouping of numbers formatting") {
  REQUIRE(strings::groupDigits(1) == "1");
  REQUIRE(strings::groupDigits(1000) == "1,000");
}

#pragma mark Unit Stats

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
}

#pragma mark Nature Spells

TEST_CASE("nature spells") {
  SECTION("nature unit spells") {
    SECTION("resist elements spell") {
      const auto pair = test::anyRaceUnitPairWithSkills({ Skills::SPELL_NATURE_RESIST_ELEMENTS });
      test::testModifiers(pair.second, pair.first, {
        { Property::SHIELDS_NATURE, 3 },
        { Property::SHIELDS_CHAOS, 3 },
        { Property::RESIST_NATURE, 3 },
        { Property::RESIST_CHAOS, 3 }
      });
    }

    SECTION("elemental armor spell") {
      const auto pair = test::anyRaceUnitPairWithSkills({ Skills::SPELL_NATURE_ELEMENTAL_ARMOR });
      test::testModifiers(pair.second, pair.first, {
        { Property::SHIELDS_NATURE, 10 },
        { Property::SHIELDS_CHAOS, 10 },
        { Property::RESIST_NATURE, 10 },
        { Property::RESIST_CHAOS, 10 }
      });
    }

    SECTION("elemental armor overrides resist elements") {
      const auto pair = test::anyRaceUnitPairWithSkills({ Skills::SPELL_NATURE_RESIST_ELEMENTS, Skills::SPELL_NATURE_ELEMENTAL_ARMOR });
      test::testModifiers(pair.second, pair.first, {
        { Property::SHIELDS_NATURE, 10 },
        { Property::SHIELDS_CHAOS, 10 },
        { Property::RESIST_NATURE, 10 },
        { Property::RESIST_CHAOS, 10 }
      });
    }
  }
}

#pragma mark effect_list

TEST_CASE("effect_list class") {
  SECTION("flatSize method with CompoundEffect") {
    SECTION("nested once") {
      const auto me1 = new MovementEffect(MovementType::FLYING);
      const auto me2 = new MovementEffect(MovementType::FORESTWALK);
      const auto me3 = new MovementEffect(MovementType::FORESTWALK);
      const auto ce = new CompoundEffect({ me1, me2 });

      effect_list effects = effect_list({ ce, me3 });

      REQUIRE(effects.size() == 2);
      REQUIRE(effects.flatSize() == 3);
    }
  }
  
  SECTION("deep iteration of effect_list") {    
    struct helper {
      static effect_list build(const std::string& encoding) {
        std::stack<effect_list> effects;
        effects.push(effect_list());
        
        for (char c : encoding)
        {
          if (c == '{') effects.push(effect_list());
          else if (c == '}') {
            effect_list ceffs = effects.top();
            effects.pop();
            effects.top().push_back(new CompoundEffect(ceffs));
          }
          else
            effects.top().push_back(new DummyEffect(std::string(1,c)));
        }
        
        return effects.top();
      }
      
      static std::string print(const effect_list& effects)
      {
        std::string v = "";
        
        auto it = effects.dbegin();
        for (; it != effects.dend(); ++it)
          v += static_cast<const DummyEffect*>(*it)->v;
        return v;
      }
    };
    
    SECTION("flat list") {
      REQUIRE(helper::print(helper::build("1234")) == "1234");
      REQUIRE(helper::print(helper::build("1")) == "1");
    }
    
    SECTION("single level nesting") {
      REQUIRE(helper::print(helper::build("1{23}4")) == "1234");
      REQUIRE(helper::print(helper::build("1{}234")) == "1234");
      REQUIRE(helper::print(helper::build("1{}2{}34")) == "1234");
      REQUIRE(helper::print(helper::build("{}1234")) == "1234");
      REQUIRE(helper::print(helper::build("{1}234")) == "1234");
      REQUIRE(helper::print(helper::build("1234{}")) == "1234");
      REQUIRE(helper::print(helper::build("{1234}")) == "1234");
    }

    SECTION("multiple nesting levels") {
      REQUIRE(helper::print(helper::build("{{1234}}")) == "1234");
      REQUIRE(helper::print(helper::build("12{3{}}4")) == "1234");
      REQUIRE(helper::print(helper::build("{12{3}4}")) == "1234");
      REQUIRE(helper::print(helper::build("{12{3{4}}}")) == "1234");
    }
  }

  SECTION("effect groups") {
    SECTION("grouping by priority keeps higher priority effect")
    {
      SkillEffectGroup group = SkillEffectGroup(SkillEffectGroup::Mode::PRIORITY);
      DummyEffect e1 = DummyEffect("first"), e2 = DummyEffect("second");
      e1.setGroup(&group, 0);
      e2.setGroup(&group, 1);

      effect_list effects = effect_list({ &e2, &e1 });
      effect_list actuals = effects.actuals(nullptr);

      REQUIRE(actuals.size() == 1);
      REQUIRE((*actuals.begin())->as<DummyEffect>()->v == "first");
    }

    SECTION("grouping by priority works with compound effects") {
      SkillEffectGroup group = SkillEffectGroup(SkillEffectGroup::Mode::PRIORITY);
      DummyEffect e1 = DummyEffect("first"), e2 = DummyEffect("second");
      CompoundEffect c1 = CompoundEffect({ &e1 }), c2 = CompoundEffect({ &e2 });
      c1.setGroup(&group, 0);
      c2.setGroup(&group, 1);

      effect_list effects = effect_list({ &c2, &c1 });
      effect_list actuals = effects.actuals(nullptr);

      REQUIRE(effects.flatSize() == 2);
      REQUIRE(actuals.size() == 1);
      REQUIRE((*actuals.begin())->as<DummyEffect>()->v == "first");
    }
  }


}

#pragma mark SkillSet

TEST_CASE("skill set class") {
  SECTION("empty constructor") {
    SkillSet set = SkillSet(skill_init_list{});
    REQUIRE(set.size() == 0);
  }

  SECTION("passing skills to constructor") {
    SkillSet set = SkillSet({ Data::skill(Skills::SPELL_NATURE_ELEMENTAL_ARMOR), Data::skill(Skills::SPELL_NATURE_RESIST_ELEMENTS) });
    REQUIRE(set.size() == 2);
    REQUIRE(*set.begin() == Data::skill(Skills::SPELL_NATURE_ELEMENTAL_ARMOR));
    REQUIRE(*++set.begin() == Data::skill(Skills::SPELL_NATURE_RESIST_ELEMENTS));
  }
}

TEST_CASE("basic skill set functions") {
  WHEN("a specific SimpleEffect is present") {
    THEN("effect can be found through normal lookup methods on UnitSpec") {
      const auto unitSpec = test::anyRaceUnitSpecWithSkills({"invisibility"});
      REQUIRE(unitSpec->skills.hasSimpleEffect(SimpleEffect::Type::INVISIBILITY));
    }
    
    THEN("effect can be found through normal lookup methods on Unit") {
      const auto unit = test::anyRaceUnitWithSkills({"invisibility"});
      REQUIRE(unit->skills()->hasSimpleEffect(SimpleEffect::Type::INVISIBILITY));
    }
  }
}

TEST_CASE("skill effects groups") {
  WHEN("a skill with a keep greater group") {
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
  
  SECTION("unique effect group" ) {
    GIVEN("multiple skill effect with same unique group") {
      const auto unit = test::anyRaceUnit();
      const effect_list effects = test::effectListWithSkills({"healer", "healer", "healer"});
      const effect_list actuals = effects.actuals(unit.get());

      THEN("only one effect is kept") {
        REQUIRE(actuals.size() == 1);
        REQUIRE(*actuals.begin() == *effects.begin());
      }
    }
  }
}


#pragma mark Combat Mechanics

#pragma mark Unit Figure Value

TEST_CASE("unit figure value helper") {
  SECTION("constructors") {
    REQUIRE(unit_figure_value(5) == std::vector<value_t>{ 0, 0, 0, 0, 0 });
    REQUIRE(unit_figure_value(5, [](value_t i) { return i*2; }) == std::vector<value_t>{ 0, 2, 4, 6, 8 });
  }
  
  SECTION("operators") {
    REQUIRE(unit_figure_value({0, 1, 2, 3, 4}) - unit_figure_value({4, 3, 2, 1, 0}) == std::vector<value_t>{-4, -2, 0, 2, 4});
  }
}

#pragma mark HitPoints
TEST_CASE("health management of units") {
  const auto unit = test::anyRaceUnit();
  const value_t figures = unit->getProperty(Property::FIGURES);
  const value_t hitPoints = unit->getProperty(Property::HIT_POINTS);
  auto* health = unit->health();
  
  SECTION("constructors") {
    REQUIRE(HitPoints(5, 4) == std::vector<value_t>{ 4, 4, 4, 4, 4 });
    REQUIRE(HitPoints(5, 4, 2) == std::vector<value_t>{ 2, 4, 4, 4, 4 });
  }
  
  SECTION("a fully healed unit") {
    REQUIRE(health->aliveCount() == figures);
    REQUIRE(health->sum() == figures*hitPoints);
  }
  
  SECTION("damage by less than its figure hitpoints") {
    const value_t dmg = unit->getProperty(Property::HIT_POINTS) - 1;
    health->applyDamage(dmg);
    REQUIRE(dmg > 0);
    REQUIRE(health->aliveCount() == figures);
    REQUIRE(health->sum() == figures*hitPoints - dmg);
    REQUIRE(health->hitsOfLeadFigure() == hitPoints - dmg);
  }
  
  SECTION("damage by more than its single figure hitpoints") {
    const value_t dmg = hitPoints - 1;
    health->applyDamage(dmg + hitPoints);
    REQUIRE(health->aliveCount() == figures - 1);
    REQUIRE(health->sum() == (figures - 1) * hitPoints - dmg);
    REQUIRE(health->hitsOfLeadFigure() == hitPoints - dmg);
  }
  
  SECTION("damage by its total hitpoints") {
    const value_t dmg = figures*hitPoints;
    health->applyDamage(dmg);
    REQUIRE(health->aliveCount() == 0);
    REQUIRE(health->sum() == 0);
    REQUIRE(!health->isAlive());
  }
  
  SECTION("damage by more its total hitpoints") {
    const value_t dmg = figures*hitPoints*2;
    health->applyDamage(dmg);
    REQUIRE(health->aliveCount() == 0);
    REQUIRE(health->sum() == 0);
    REQUIRE(!health->isAlive());
  }
}

#include "common/Util.h"

constexpr value_t MAX_CHANCE = 100;
constexpr value_t NO_CHANCE = 0;

TEST_CASE("general functions") {
  constexpr value_t TEST_COUNT = 10000;
  constexpr value_t CONFIDENCE = TEST_COUNT * 0.03f;
  SECTION("roll chance") {
    
    SECTION("0% chance") {
      value_t sum = 0;
      for (value_t i = 0; i < TEST_COUNT; ++i)
        sum += Math::chance(NO_CHANCE) ? 1 : 0;
      REQUIRE(sum == 0);
    }
    
    SECTION("100% chance") {
      value_t sum = 0;
      for (value_t i = 0; i < TEST_COUNT; ++i)
        sum += Math::chance(MAX_CHANCE) ? 1 : 0;
      REQUIRE(sum == TEST_COUNT);
    }
    
    for (value_t c = 0; c < 100; ++c)
    {
      DYNAMIC_SECTION("fixed chance " << c)
      {
        value_t sum = 0;
        for (u32 i = 0; i < TEST_COUNT; ++i)
          sum += Math::chance(c) ? 1 : 0;
        
        value_t expected = TEST_COUNT * (c / (float)MAX_CHANCE);
        REQUIRE(std::abs(expected - sum) < CONFIDENCE);
      }
    }
  }
}

TEST_CASE("combat formulas") {
  using namespace combat;

  
  const auto unit = test::anyRaceUnit();
  const auto cunit = test::cunit(unit);
  CombatFormulas f;

  
  SECTION("area damage formulas") {
    using namespace combat;
    f.computeAreaDamage(30, 10, 6, 4, 30, 2);
  }
  
  SECTION("physical damage formulas") {
    
    SECTION("each figure defends maximum hits")
    {
      /* first figure blocks 2, 3 inflicted, 6 pass, second figure blocks 2, 3 infliced, 1 pass which is blocked by third figure: result 6 */
      auto damage = f.computePhysicalDamage(MAX_CHANCE, 11, HitPoints(6, 3), MAX_CHANCE, 2);
      REQUIRE(damage == 6);
    }
    
    SECTION("each figure defend no hits")
    {
      auto damage = f.computePhysicalDamage(MAX_CHANCE, 11, HitPoints(6, 3), NO_CHANCE, 2);
      REQUIRE(damage == 11);
    }
    
    SECTION("attack strength is higher than total hits of unit")
    {
      HitPoints hp = HitPoints(6, 3);
      auto damage = f.computePhysicalDamage(MAX_CHANCE, 50, hp, NO_CHANCE, 2);
      REQUIRE(damage == hp.sum());
    }
    
    SECTION("attacker has no hit chance")
    {
      auto damage = f.computePhysicalDamage(NO_CHANCE, 11, HitPoints(6, 3), NO_CHANCE, 2);
      REQUIRE(damage == 0);
    }
  }
}


