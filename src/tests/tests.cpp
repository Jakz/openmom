#include "tests.h"

#include "game/units/Unit.h"
#include "UnitSpec.h"
#include "Skill.h"

#include "save/Data.h"

#include "game/world/World.h"
#include "game/combat/Combat.h"
#include "game/combat/CombatMechanics.h"


#include "common/mystrings.h"

namespace mock
{
  class RaceUnitSpec;
  
  class dummy
  {
  public:
    static const mock::RaceUnitSpec raceUnitSpec;
  };

  class Level : public ::Level
  {
  public:
    Level(value_t multiplier) : ::Level(multiplier - 1, I18::PLACEHOLDER, SpriteInfo(), nullptr, 0, {}) { }
  };

  class RaceUnitSpec : public ::RaceUnitSpec
  {
  public:
    RaceUnitSpec() : ::RaceUnitSpec(nullptr, 1, 50, 1, RangedInfo(), 1, 1, 1, 1, 1, 1, {}) { }
    RaceUnitSpec(RangedInfo&& ranged) : ::RaceUnitSpec(nullptr, 1, 50, 1, ranged, 1, 1, 1, 1, 1, 1, { }) { }
    RaceUnitSpec(value_t melee, value_t defense, value_t resistance, value_t hits) : ::RaceUnitSpec(nullptr, 1, 50, melee, RangedInfo(), defense, resistance, hits, 1, 1, 1, { }) { }
    RaceUnitSpec(value_t melee, value_t defense, value_t resistance, value_t hits, value_t figures) : ::RaceUnitSpec(nullptr, 1, 50, melee, RangedInfo(), defense, resistance, hits, figures, 1, 1, { }) { }

  };

  class RaceUnit : public ::RaceUnit
  {
  private:
    const Level* level;

  public:
    RaceUnit(const RaceUnitSpec& spec) : ::RaceUnit(&spec) { }
    RaceUnit() : RaceUnit(dummy::raceUnitSpec) { }
    RaceUnit(skill_init_list skills) : RaceUnit()
    {
      for (const auto* skill : skills) this->skills()->add(skill);
    }

    value_t experienceMultiplier() const override { return level->index()+1; }

    void setLevel(const Level* level) { this->level = level;  }
  };

  class Skill : public ::ConcreteSkill
  {
  public:
    Skill() : ::ConcreteSkill(SkillBase::ARMOR_PIERCING, {}) { }
    Skill(unit_effect_list effects) : ::ConcreteSkill(SkillBase::ARMOR_PIERCING, effects) { }

    Skill(std::initializer_list<const UnitEffect*> effects) : ::ConcreteSkill(SkillBase::ARMOR_PIERCING, { })
    {
      for (const auto* effect : effects)
        addEffect(effect);
    }

    void addEffect(const UnitEffect* effect) { effects.push_back(effect); }
  };

  class Army : public ::Army
  {
  public:
    Army(std::initializer_list<Unit*> units) : ::Army(nullptr, units) { }

  };

  class Game : public ::Game
  {

  };

  class Player : public ::LocalPlayer
  {
  public:
    Player(Game* game) : ::LocalPlayer(game, "", nullptr, PlayerColor::BLUE, Data::race("beastmen"), 60, 40) { }
  };
  
  // City::City(Player* owner, const Race* race, const std::string& name, value_t population, Position position) :
  class City : public ::City
  {
  public:
    City(const Race* race, Position position, value_t population, std::initializer_list<const Building*> buildings = {}) : ::City(nullptr, race, "", population, position)
    {
      for (const auto* building : buildings)
        addBuilding(building);
    }
    
    void setFarmers(value_t farmers) { this->farmers = farmers; }
    void setWorkers(value_t workers) { this->workers = workers; }
  };

  const RaceUnitSpec dummy::raceUnitSpec = RaceUnitSpec();
}

namespace Catch
{
  template<> struct StringMaker<MovementType> {
    static std::string convert(MovementType value) {
      switch (value) {
        case MovementType::FLYING: return "flying";
        case MovementType::FORESTWALK: return "forester";
        case MovementType::MOUNTAINWALK: return "mountaineer";
        case MovementType::NON_CORPOREAL: return "non-corporeal";
        case MovementType::PATH_FINDER: return "path-finder";
        case MovementType::PLANAR_TRAVEL: return "planar-travel";
        case MovementType::NORMAL: return "normal";
        case MovementType::SWIMMING: return "swimming";
        case MovementType::WINDWALK: return "windwalk";
      }
    }
  };
}

class DummyEffect : public UnitEffect {
public:
  std::string v;
  DummyEffect(std::string v) : UnitEffect(UnitEffectType::MOVEMENT), v(v) { }
};

struct PropertyModifier
{
  Property property;
  value_t value;
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

  std::unique_ptr<Hero> anyHeroUnit() { return std::unique_ptr<Hero>(new Hero(Data::unit("hero_dwarf")->as<HeroSpec>())); }

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
  
  unit_effect_list effectListWithSkills(identifier_list skills)
  {
    unit_effect_list effects;
    
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
}

//TODO: check that ranged modifiers by class actually alter values

TEST_CASE("movement of armies")
{
  MapMechanics mechanics = MapMechanics(nullptr);
  unit_list units;
  movement_list expected;


  SECTION("flying") {
    const auto* flying = Data::skill("flying");

    SECTION("single unit") {
      units.push_back(new mock::RaceUnit({ flying }));
      expected += { MovementType::FLYING, MovementType::SWIMMING }; //TODO: is this intended behavior to have swimming not overridden by flying itself?
    }

    SECTION("multiple units") {
      units.push_back(new mock::RaceUnit({ flying }));
      units.push_back(new mock::RaceUnit({ flying }));
      expected += { MovementType::FLYING, MovementType::SWIMMING };
    }

    SECTION("multiple units not all flying") {
      units.push_back(new mock::RaceUnit({ flying }));
      units.push_back(new mock::RaceUnit());
      expected += { };
    }
  }

  SECTION("forester") {
    SECTION("single unit") {
      units.push_back(new mock::RaceUnit({ Data::skill("forester") }));
      expected += { MovementType::FORESTWALK };
    }

    SECTION("multiple units") {
      units.push_back(new mock::RaceUnit({ Data::skill("forester") }));
      units.push_back(new mock::RaceUnit({ Data::skill("forester") }));
      expected += { MovementType::FORESTWALK };
    }

    SECTION("windwalking overrides forester") {
      units.push_back(new mock::RaceUnit({ Data::skill("forester") }));
      units.push_back(new mock::RaceUnit({ Data::skill("windwalking") }));
      expected += { MovementType::FLYING };
    }

    /*SECTION("sailing overrides forester") {
      units.push_back(new mock::RaceUnit({ Data::skill("forester") }));
      units.push_back(new mock::RaceUnit({ Data::skill("sailing") }));
      expected += { MovementType::SAILING };
    }*/
  }

  movement_list movements = mechanics.movementTypeOfArmy(units);
  REQUIRE(movements == expected);

  std::for_each(units.begin(), units.end(), [](auto* unit) { delete unit; });
}

TEST_CASE("scouting") {
  SECTION("scouting sets the range, doesn't add to it")
  {
    mock::RaceUnit unit;
    unit.skills()->add(Data::skill("scouting_3"));

    //TODO: disabled until new management of FIXED properies is done
    //REQUIRE(unit.getProperty(Property::SIGHT) == 3);
  }
}

TEST_CASE("items") {
  SECTION("affix is applied to hero stats") {
    auto hero1 = test::anyHeroUnit();
    auto hero2 = test::anyHeroUnit();

    items::Item* item = new items::Item(items::TypeID::SWORD, 0);
    item->addAffix({ Property::MELEE, 3 });
    hero1->placeItem(0, item);

    test::testModifiers(hero1.get(), hero2.get(), {
      { Property::MELEE, 3}
    });


  }
}

TEST_CASE("magic weapons") 
{
  mock::RaceUnit base;
  mock::RaceUnit magical({ "magical_weapons"_skill });
  mock::RaceUnit mithril({ "mithril_weapons"_skill });
  mock::RaceUnit adamantium({ "adamantium_weapons"_skill });

  SECTION ("to hit") {
    REQUIRE(base.getProperty(Property::TO_HIT) == magical.getProperty(Property::TO_HIT) - 10);
    REQUIRE(base.getProperty(Property::TO_HIT) == mithril.getProperty(Property::TO_HIT) - 10);
    REQUIRE(base.getProperty(Property::TO_HIT) == adamantium.getProperty(Property::TO_HIT) - 10);
  }
}


#pragma mark Unit Abilities

TEST_CASE("Abilities")
{
  SECTION("large shield modifier") {
    const UnitSpec* spec = "barbarian_swordsmen"_unitspec;
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

TEST_CASE("Hero Abilities")
{
  SECTION("Noble") {
    mock::RaceUnit unit;
    //TODO: requires FIXED modifiers to be applied to base property too
    /*REQUIRE(unit.getProperty(Property::GOLD_UPKEEP) > 0);
    unit.skills()->add(Data::skill("noble"));
    REQUIRE(unit.getProperty(Property::GOLD_UPKEEP) == 0);*/
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
  SECTION("modifiers calculation")
  {
    unit_effect_list effects;
    auto modifier = UnitPropertyBonus(Property::MELEE, 2);
    effects.push_back(&modifier);

    value_t result = effects.reduceAsModifier<UnitPropertyBonus>(Property::MELEE, nullptr, 0);
    REQUIRE(result == 2);
  }


  SECTION("flatSize method with CompoundEffect") {
    SECTION("nested once") {
      const auto me1 = new MovementEffect(MovementType::FLYING);
      const auto me2 = new MovementEffect(MovementType::FORESTWALK);
      const auto me3 = new MovementEffect(MovementType::FORESTWALK);
      const auto ce = new CompoundEffect<UnitEffect>({ me1, me2 });

      auto effects = unit_effect_list({ ce, me3 });

      REQUIRE(effects.size() == 2);
      REQUIRE(effects.flatSize() == 3);
    }
  }
  
  SECTION("deep iteration of effect_list") {    
    struct helper {
      static unit_effect_list build(const std::string& encoding) {
        std::stack<unit_effect_list> effects;
        effects.push(unit_effect_list());
        
        for (char c : encoding)
        {
          if (c == '{') effects.push(unit_effect_list());
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
      
      static std::string print(const unit_effect_list& effects)
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
      EffectGroup group = EffectGroup(EffectGroup::Mode::PRIORITY);
      DummyEffect e1 = DummyEffect("first"), e2 = DummyEffect("second");
      e1.setGroup(&group, 0);
      e2.setGroup(&group, 1);

      unit_effect_list effects = unit_effect_list({ &e2, &e1 });
      unit_effect_list actuals = effects.actuals(nullptr);

      REQUIRE(actuals.size() == 1);
      REQUIRE((*actuals.begin())->as<DummyEffect>()->v == "first");
    }

    SECTION("grouping by priority works with compound effects") {
      EffectGroup group = EffectGroup(EffectGroup::Mode::PRIORITY);
      DummyEffect e1 = DummyEffect("first"), e2 = DummyEffect("second");
      auto c1 = CompoundEffect<UnitEffect>(unit_effect_list(&e1)), c2 = CompoundEffect<UnitEffect>(unit_effect_list(&e2));
      c1.setGroup(&group, 0);
      c2.setGroup(&group, 1);

      unit_effect_list effects = unit_effect_list({ &c2, &c1 });
      auto actuals = effects.actuals(nullptr).flatten();

      REQUIRE(effects.flatSize() == 2);
      REQUIRE(actuals.size() == 1);
      REQUIRE((*actuals.begin())->as<DummyEffect>()->v == "first");
    }
  }
}

TEST_CASE("modifiers") {
  SECTION("integral multiplier") {
    const auto l = GENERATE(range(1, 10));
    const auto m = GENERATE(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);

    const UnitModifierValue modifier = UnitModifierValue(UnitModifierValue::Mode::ADDITIVE_PARAMETRIC, m);
    const auto level = mock::Level(l);
    auto unit = mock::RaceUnit();
    unit.setLevel(&level);

    REQUIRE(modifier.transformValue(0, &unit) == l * static_cast<value_t>(m));
  }

  SECTION("fractional multiplier") {
    const auto l = GENERATE(range(1, 10));
    const auto m = GENERATE(1.5f, 2.5f, 3.5f, 4.5f, 5.5f);

    const UnitModifierValue modifier = UnitModifierValue(UnitModifierValue::Mode::ADDITIVE_PARAMETRIC, m);
    const auto level = mock::Level(l);
    auto unit = mock::RaceUnit();
    unit.setLevel(&level);

    REQUIRE(modifier.transformValue(0, &unit) == static_cast<value_t>(l * m));
  }

  SECTION("priority last is applied at the end")
  {
    const UnitModifierValue modifier = UnitModifierValue(UnitModifierValue::Mode::ADDITIVE, 5);
    const UnitModifierValue zeroer = UnitModifierValue(UnitModifierValue::Mode::FIXED, 0, UnitModifierValue::Priority::LAST);

    auto unit = mock::RaceUnit();

    const auto effect1 = UnitPropertyBonus(Property::MELEE, modifier);
    const auto effect2 = UnitPropertyBonus(Property::MELEE, zeroer);

    /* order is swapped to ensure sorting occurs */
    unit_effect_list effects = unit_effect_list({ &effect2, &effect1 });
    effects.sort();

    REQUIRE(effects.reduceAsModifier<UnitPropertyBonus>(Property::MELEE, &unit, 0) == 0);

  }

  SECTION("ranged attack modifiers")
  {
    auto spec = mock::RaceUnitSpec(RangedInfo(Ranged::BULLET, 2, 4));
    auto base = mock::RaceUnit(spec);

    SECTION("only valid ranged type or generic one return correct value")
    {
      REQUIRE(base.getEnumProperty<Ranged>(Property::RANGED_TYPE) == Ranged::BULLET);
      REQUIRE(base.getProperty(Property::RANGED) == 2);
      REQUIRE(base.getProperty(Property::RANGED_MISSILE) == 2);
      REQUIRE(base.getProperty(Property::RANGED_MAGIC) == 0);
      REQUIRE(base.getProperty(Property::RANGED_BOULDER) == 0);
      REQUIRE(base.getProperty(Property::AMMO) == 4);
    }

    SECTION("modifiers on different type of ranged doesn't affect value")
    {
      auto unit = base;

      const UnitModifierValue modifier = UnitModifierValue(UnitModifierValue::Mode::ADDITIVE, 2);
      auto effect = UnitPropertyBonus(Property::RANGED_BOULDER, modifier);
      auto skill = mock::Skill({ &effect });

      unit.skills()->add(&skill);

      auto info = unit.getRangedInfo();
      REQUIRE(info.strength == 2);
      REQUIRE(unit.getProperty(Property::RANGED) == 2);
    }

    SECTION("modifiers on same type of ranged affects value")
    {
      auto unit = base;

      const UnitModifierValue modifier = UnitModifierValue(UnitModifierValue::Mode::ADDITIVE, 3);
      auto effect = UnitPropertyBonus(Property::RANGED_MISSILE, modifier);
      auto skill = mock::Skill({ &effect });

      unit.skills()->add(&skill);

      auto info = unit.getRangedInfo();

      REQUIRE(info.strength == 5);
      REQUIRE(unit.getProperty(Property::RANGED) == 5);
    }
  }

  SECTION("ScalableValue modifiers") {

    SECTION("value_t base") {
      using Value = ScalableValue<value_t>;
      using Modifier = ::Modifier<Value, Unit, ModifierDummyGetter<Unit>>;
      using modifier_list = ::modifier_list<Modifier, Value, Unit>;
      using Mode = Modifier::Mode;
      static constexpr auto Any = Modifier::Priority::ANY;


      SECTION("base additive values convert to sum") {
        std::array<Modifier, 5> modifiers;
        for (value_t i = 0; i < modifiers.size(); ++i)
          modifiers[i] = Modifier(Mode::ADDITIVE, i + 1, Any);

        Value sresult = std::accumulate(modifiers.begin(), modifiers.end(), Value(), 
                                      [](Value p, const Modifier& modifier) { return modifier.transformValue(p, nullptr); });

        value_t result = sresult;

        REQUIRE(result == (1 + 2 + 3 + 4 + 5));
      }

      SECTION("multipliers are added and calculated in the end") {
        modifier_list mods;
        mods += Modifier(Mode::ADDITIVE, 7, Any);
        mods += Modifier(Mode::MULTIPLICATIVE, 70_x, Any); // +70%
        mods += Modifier(Mode::MULTIPLICATIVE, -25_x, Any); // -25%

        value_t result = mods.reduce();
        REQUIRE(result == std::floor(7 * (1.0f + 0.7f - 0.25f)));
      }

      SECTION("single multiplier is applied regardless of order") {
        Modifier base = Modifier(Mode::ADDITIVE, 7, Any);

        SECTION("additive first") {
          Modifier mult = Modifier(Mode::MULTIPLICATIVE, 100_x, Any); // adds +100%
          value_t result = mult.transformValue(base.transformValue(Value(), nullptr), nullptr);
          REQUIRE(result == (7 * 2));
        }

        SECTION("multiplier first") {
          Modifier mult = Modifier(Mode::MULTIPLICATIVE, 100_x, Any); // adds +100%
          value_t result = base.transformValue(mult.transformValue(Value(), nullptr), nullptr);
          REQUIRE(result == (7 * 2));
        }
      }

      SECTION("non integral results should be floored") {
        Modifier base = Modifier(Mode::ADDITIVE, 9, Any);
        Modifier mult = Modifier(Mode::MULTIPLICATIVE, -50_x, Any); // -50%
        value_t result = base.transformValue(mult.transformValue(Value(), nullptr), nullptr);
        REQUIRE(result == 4);
      }
    }

  }
}


#pragma mark Cities

TEST_CASE("city production") {
  auto race = Data::race("beastmen");
  auto game = mock::Game();
  auto player = mock::Player(&game);
  auto city = mock::City(race, Position(10, 10, Plane::ARCANUS), 10000);
  auto mechanics = CityMechanics(&game);

  SECTION("base formula 1/2 per farmer rounded up + 2 (3) per worker")
  {
    //TODO: nerfed because it's time consuming, need to fix release compilation and check again
    value_t farmers = GENERATE(range(1, /*25*/5));
    value_t workers = GENERATE(range(1, /*25*/5));

    city.setFarmers(farmers);
    city.setWorkers(workers);

    value_t production = mechanics.baseProduction(&city);
    
    INFO("with farmers = " << farmers << " and workers " << workers);
    REQUIRE(production == workers*race->baseProduction + static_cast<value_t>(std::ceil(farmers / 2.0f)));
  }
}

TEST_CASE("city magic power") {
  auto race = Data::race("beastmen");
  auto game = mock::Game();
  auto player = mock::Player(&game);
  auto mechanics = CityMechanics(&game);
  
  SECTION("magic fortress") {
    SECTION("magic fortress give 5 power when in myrran plane") {
      auto city = mock::City(race, Position(10, 10, Plane::MYRRAN), 10000);
      city.addBuilding("mage_fortress"_building);
      
      REQUIRE(mechanics.reduceModifiers(&city, CityAttribute::MANA_POWER_OUTPUT) == 5);
    }
    SECTION("magic fortress on arcanus plane gives 0 mana") {
      auto city = mock::City(race, Position(10, 10, Plane::ARCANUS), 10000);
      city.addBuilding("mage_fortress"_building);

      REQUIRE(mechanics.reduceModifiers(&city, CityAttribute::MANA_POWER_OUTPUT) == 0);
    }
  }
}

TEST_CASE("city research output") {
  auto race = Data::race("beastmen");
  auto game = mock::Game();
  auto player = mock::Player(&game);
  auto mechanics = CityMechanics(&game);

  SECTION("library") {
    auto city = mock::City(race, Position(10, 10, Plane::ARCANUS), 10000, { "library"_building });
    REQUIRE(mechanics.reduceModifiers(&city, CityAttribute::RESEARCH_OUTPUT) == 2);
  }

  SECTION("sage's guild") {
    auto city = mock::City(race, Position(10, 10, Plane::ARCANUS), 10000, { "sages_guild"_building });
    REQUIRE(mechanics.reduceModifiers(&city, CityAttribute::RESEARCH_OUTPUT) == 3);
  }

  SECTION("university") {
    auto city = mock::City(race, Position(10, 10, Plane::ARCANUS), 10000, { "university"_building });
    REQUIRE(mechanics.reduceModifiers(&city, CityAttribute::RESEARCH_OUTPUT) == 5);
  }

  SECTION("wizard's guild") {
    auto city = mock::City(race, Position(10, 10, Plane::ARCANUS), 10000, { "wizards_guild"_building });
    REQUIRE(mechanics.reduceModifiers(&city, CityAttribute::RESEARCH_OUTPUT) == 8);
  }

  SECTION("mechanics correctly works")
  {
    auto city = mock::City(race, Position(10, 10, Plane::ARCANUS), 10000, { "wizards_guild"_building, "library"_building });
    REQUIRE(mechanics.computeKnowledge(&city) == 10);
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

TEST_CASE("movement effects") {
  SECTION("check if basic movement finding behavor works") {
    MovementEffect movement(MovementType::FLYING);
    skills::ConcreteSkill skill = skills::ConcreteSkill(skills::Type::NATIVE, { &movement }, skills::SkillVisualInfo());
    SkillSet set = SkillSet({ &skill });
    REQUIRE(set.has(MovementType::FLYING));
  }
  
  SECTION("behave like if no flying is present with disallow") {
    MovementEffect movement(MovementType::FLYING);
    MovementDisallowEffect dmovement(MovementType::FLYING);
    skills::ConcreteSkill skill = skills::ConcreteSkill(skills::Type::NATIVE, { &movement, &dmovement }, skills::SkillVisualInfo());
    SkillSet set = SkillSet({ &skill });
    REQUIRE(!set.has(MovementType::FLYING));
  }
}

TEST_CASE("skill effects groups") {
  WHEN("a skill with a keep greater group") {
    GIVEN("two skill effects, one more powerful") {
      const auto unit = test::anyRaceUnit();
      const unit_effect_list effects = test::effectListWithSkills({"resistance_to_all_2", "resistance_to_all_1"});
      const unit_effect_list actuals = effects.actuals(unit.get());
      
      THEN("only the most powerful is kept") {
        REQUIRE(actuals.size() == 1);
        REQUIRE(*actuals.begin() == *effects.begin());
      }
    
    }
  }
  
  SECTION("unique effect group" ) {
    GIVEN("multiple skill effect with same unique group") {
      const auto unit = test::anyRaceUnit();
      const unit_effect_list effects = test::effectListWithSkills({"healer", "healer", "healer"});
      const unit_effect_list actuals = effects.actuals(unit.get());

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
  unit->health()->healAll();
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

static constexpr value_t TEST_COUNT = 10000;
static constexpr value_t CONFIDENCE = TEST_COUNT * 0.03f;

static constexpr value_t MAX_RESISTANCE = 10;

TEST_CASE("gaze attacks") {
  auto m = combat::CombatMechanics(nullptr);
  m.setLogging(false);

  SECTION("average figures killed on fatal gaze") {
    mock::RaceUnitSpec spec = mock::RaceUnitSpec(1, 1, 5, 10, 10);

    value_t killedFigures = 0;

    for (value_t i = 0; i < TEST_COUNT; ++i)
    {
      unit_figure_value damage = m.computeGazeDamage(damage_amount(true), spec.figures, spec.resistance, 0);
      killedFigures += damage.countPositive();
    }

    value_t expected = TEST_COUNT * std::max(MAX_RESISTANCE - spec.resistance, 0);
    REQUIRE(std::abs(expected - killedFigures) <= CONFIDENCE);
  }

}

TEST_CASE("poison damage") {
  auto m = combat::CombatMechanics(nullptr);

  const auto STRENGTH = GENERATE(0, 1, 3, 5, 8, 10, 15);
  const auto RESISTANCE = GENERATE(0, 1, 3, 5, 8, 10, 15);

  DYNAMIC_SECTION("average damage of " << STRENGTH << " resisted by " << RESISTANCE) {
    damage_value damage = 0;

    for (value_t i = 0; i < TEST_COUNT; ++i)
      damage += m.computePoisonDamage(STRENGTH, RESISTANCE, 0);

    value_t expected = TEST_COUNT * STRENGTH * std::max(MAX_RESISTANCE - RESISTANCE, 0) / 10.0f;

    INFO("actual damage: " << damage << " expected average: " << expected);
    REQUIRE(std::abs(expected - damage) <= CONFIDENCE);
  }


}

TEST_CASE("immunities") {
  
  SECTION("poison attack and poison immunity") {
    auto immunity = "immunity_poison"_skill;
    auto set = SkillSet({ immunity });

    set.isImmuneTo("poison_attack"_effectgroup);
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
