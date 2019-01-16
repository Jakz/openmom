#include "NewGameView.h"

#include "Data.h"
#include "Font.h"
#include "Buttons.h"
#include "Gfx.h"
#include "ViewManager.h"
#include "Localization.h"

#include "GfxData.h"
#include "UnitSpec.h"
#include "UnitDraw.h"
#include "CommonDraw.h"

#include "game/Game.h"
#include "game/mechanics/SpellMechanics.h"

#include "format.h"

#include "Messages.h"
#include "LocalPlayer.h" //TODO: required for error message, maybe unneeded


bool Textfield::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (key == KeyboardCode::SDL_SCANCODE_ESCAPE)
    onCancel();
  else if (key == KeyboardCode::SDL_SCANCODE_KP_ENTER || key == KeyboardCode::SDL_SCANCODE_RETURN || key == KeyboardCode::SDL_SCANCODE_RETURN2)
    onComplete(text);
  else if (key == KeyboardCode::SDL_SCANCODE_BACKSPACE)
  {
    if (caret > 0)
    {
      text.erase(text.begin()+caret-1);
      --caret;
    }
  }
  else if (key == KeyboardCode::SDL_SCANCODE_DELETE)
  {
    if (caret < text.length())
    {
      text.erase(text.begin()+caret);
    }
  }
  else if (key == KeyboardCode::SDL_SCANCODE_LEFT)
  {
    if (caretEnabled && caret > 0)
      --caret;
  }
  else if (key == KeyboardCode::SDL_SCANCODE_RIGHT)
  {
    if (caretEnabled && caret < text.size())
      ++caret;
  }
  
  return true;
}

bool Textfield::textInput(sdl_text_input data)
{
  if (data[1] == '\0')
  {
    if (isalpha(data[0]))
    {
      text.insert(caret, &data[0]);
      ++caret;
    }
  }
  
  return true;
}

void Textfield::draw()
{
  Fonts::drawString(text, font, position.x, position.y, ALIGN_LEFT);
}




void PlayerNameField::draw()
{
  Textfield::draw();
  
  u16 x, caretWidth;
  u16 y = position.y + font->sh() - 1;
  
  if (caret == text.length())
  {
    x = Fonts::stringWidth(font, text);
    caretWidth = font->charWidth(' ');
  }
  else
  {
    x = Fonts::stringWidth(font, text.substr(0, caret));
    caretWidth = font->charWidth(text[caret]);
  }
  
  if (Gfx::fticks % 4 < 2)
    Gfx::drawLine(Color(158, 125, 101), position.x + x, y, position.x + x + caretWidth + 2, y);
  
  //if (Gfx::fticks % 4 < 2)
  //  Fonts::drawString("_", font, position.x + x, position.y, ALIGN_LEFT);
}



enum sprite_id : sprite_ref
{
  main_bg = LBXI(NEWGAME, 0),
  options_bg = LBXI(NEWGAME, 1),
  wizard_choice_bg = LBXI(NEWGAME, 8),
  portrait_choice_bg = LBXI(NEWGAME, 39),
  name_choice_bg = LBXI(NEWGAME, 40),
  books_choice_bg = LBXI(NEWGAME, 41),
  
  divider = LBXI(NEWGAME, 47),
  
  dark_region_1 = LBXI(NEWGAME, 48),
  dark_region_2 = LBXI(NEWGAME, 49),
  dark_region_3 = LBXI(NEWGAME, 50),
  
  bottom_box_spell_choice = LBXI(NEWGAME, 51),
  
  divider2 = LBXI(NEWGAME, 54),
  races_background = LBXI(NEWGAME, 55),
  
  checkmark = LBXI(NEWGAME, 52)
};

static const Point gameOptionsButtonPositions[] = {
  { 251, 39 },
  { 251, 66 },
  { 251, 93 },
  { 251, 120 }
};

NewGameView::NewGameView(ViewManager * gvm) : ViewWithQueue(gvm), info({nullptr, "", school_value_map(0)}), fonts({FontPalette()})
{
  nameField.setFace(new fonts::MediumBoldFont({158, 125, 101}));
  nameField.setOnCancel([this](){ switchToPhase(isPremadeWizard ? Phase::WIZARD_CHOICE : Phase::PORTRIT_CHOICE); });
  nameField.setOnComplete([this](const std::string& name) {
    info.name = name;
    switchToPhase(isPremadeWizard ? Phase::RACE_CHOICE : Phase::BOOKS_CHOICE);
  });
  
  const Color single = {113, 85, 69};
  
  fonts.darkBoldFont = new fonts::MediumBoldFont({52,40,28}, {166,134,105}); // TODO: fix last color?
  fonts.brightBoldFont = new fonts::MediumBoldFont({166,134,105}, {52,40,28});
  fonts.darkSerifFont = fonts::SerifFont::of({52,40,28}, {166, 134, 105});
  
  fonts.tinyBright = fonts::TinyFont::of({190, 154, 117}, single, {56, 31, 27});
  fonts.tinyInactive = fonts::TinyFont::of({97, 73, 60}, single, {56, 31, 27});
  fonts.tinyGold = fonts::TinyFont::of({239, 166, 35}, {97, 73, 60}, {56, 31, 27});
  fonts.tinyRetortList = fonts::TinyFont::of({239, 166, 35}, {142, 97, 36}, {52, 40, 28});
  
  /* take from main palette directly? it should be 13th */
  fonts.schoolFonts.set(School::LIFE, FontPalette::ofSolidWithLowShadow({219, 219, 219}, {0, 0, 0}));
  fonts.schoolFonts.set(School::DEATH, FontPalette::ofSolidWithLowShadow(Gfx::mainPalette->get(114), {0, 0, 0}));

  fonts.schoolFonts.set(School::NATURE, FontPalette::ofSolidWithLowShadow({0, 190, 0}, {0, 0, 0}));
  
  /* TODO: shadow is incorrect because edge shadow is transparent but these make it same color as low shadow */
  fonts.brightMedium = FontPalette::ofSolidWithLowShadow({ 194, 154, 118 }, single, { 10, 10, 10 });
  fonts.hoverMedium = FontPalette::ofSolidWithLowShadow({ 255, 202, 100 }, {89, 65, 52}, { 10, 10, 10 });
  fonts.inactiveMedium = FontPalette::ofSolidWithLowShadow({ 97, 73, 60 }, single, { 10, 10, 10 });

  fonts.racesTitle = FontPalette::ofSolidWithLowShadow({207, 138, 23}, {0, 0, 0});
}

void NewGameView::activate()
{
  info.portrait = Data::wizard("kali"); //TODO nullptr
  info.books = school_value_map({{CHAOS, 3}, {LIFE, 4}, {SORCERY, 0},  {ARCANE, 0}, {DEATH, 0}, {NATURE, 5}}); //TODO school_value_map(0);
  info.name = "Jack"; // "";
  /*info.retorts.insert(Data::retort("archmage")); // ...
  info.retorts.insert(Data::retort("conjurer"));
  info.retorts.insert(Data::retort("node_mastery"));
  info.retorts.insert(Data::retort("charismatic"));*/
  isPremadeWizard = false; // TODO true
  
  switchToPhase(Phase::RACE_CHOICE);
  
  nameField.setPosition(Point(194,35));
  nameField.setText("Lo Pan");
  
  availablePicks = 11;
}

void NewGameView::errorMessage(const std::string& message)
{
  player->send(new msgs::Error(message));
}

void NewGameView::drawRetortList()
{
  if (!info.retorts.empty())
  {
    size_t i = 0;
    std::string ss;
  
    /* we must use original list because this is the order shown in game */
    // TODO: order now always correspond to the one of original game
    const auto& retorts = Data::values<const Retort*>();
    for (auto it = retorts.obegin(); it != retorts.oend(); ++it)
    {
      const Retort* retort = *it;
      
      if (info.retorts.find(retort) != info.retorts.end())
      {
        if (i == 0)
          ss = i18n::s(retort->i18n) + '.';
        else if (i == 1)
          ss = i18n::s(retort->i18n) + " and " + ss;
        else
          ss = i18n::s(retort->i18n) + ", " + ss;
        
        ++i;
      }
    }
  
    // TODO: ugly hack to increase vertical spacing between lines, we should
    // understand if this font is used with multiple spacings or this is the most
    // used one
    Fonts::setFace(fonts.tinyRetortList, fonts.tinyRetortList->ver+1, fonts.tinyRetortList->hor);
    Fonts::drawStringBounded(ss, 12, 180, 145, ALIGN_LEFT);
  }
}

u32 NewGameView::countPicks() const
{
  u32 picksFromRetort = std::accumulate(info.retorts.begin(), info.retorts.end(), 0u, [](u32 value, const Retort* retort) { return value + retort->cost; });
  u32 picksFromBooks = std::accumulate(info.books.begin(), info.books.end(), 0u, [](u32 value, const u32& entry) { return value + entry; });
  
  return picksFromRetort + picksFromBooks;
}

void NewGameView::booksPicked(School school, u16 amount)
{
  int dx = amount - info.books[school];
  
  /* we're removing books, alwasys do it then check for broken retorts */
  if (dx < 0)
  {
    info.books.set(school, amount);
    //TODO: check for broken retorts and remove them + error message
  }
  else if (dx > 0)
  {
    int freePicks = availablePicks - countPicks();
    /* if we're allowed to add books, if all picks are used then message */
    if (freePicks > 0)
    {
      s16 maxIncrement = std::min(freePicks, dx);
      info.books.set(school, info.books[school] + maxIncrement);
    }
    else
    {
      errorMessage("You have already made all your picks");
    }
  }
  
  bookPhaseOkButton->activateIf(freePicks() == 0);
}

//TODO: use value from ValueMechanics
static constexpr s32 MAX_RETORTS = 6;

void NewGameView::retortToggled(const Retort* retort)
{
  //TODO: localize (this will be a problem for plurals )
  static const std::string no_picks_available = "You have already made all your picks";
  static const std::string no_enough_picks_available = "You don't have enough picks left to make this selection.\nYou need %d picks";
  static const std::string no_enough_books_school_prefix = "To select %s you need:   ";
  static const std::string no_enough_books_any_school = "%d pick%s in any %sRealm%s of Magic";
  static const std::string no_enough_books_specific_school = "%d pick%s in %s Magic";
  static const std::string too_many_retorts = "You may not select more than 6 special abilities";

  
  if (info.retorts.find(retort) != info.retorts.end())
    info.retorts.erase(retort);
  else if (info.retorts.size() < MAX_RETORTS && retort->canBePicked(freePicks(), info.books))
    info.retorts.insert(retort);
  else if (info.retorts.size() >= MAX_RETORTS)
    errorMessage(too_many_retorts);
  else if (freePicks() == 0)
    errorMessage(no_picks_available);
  else
  {
    /* generate error message for requirements */
    if (freePicks() < retort->cost)
      errorMessage(fmt::sprintf(no_enough_picks_available, retort->cost));
    else if (!retort->requirements.empty())
    {
      const Retort::Requirement& req = retort->requirements[0];
      std::string prefix = fmt::sprintf(no_enough_books_school_prefix, i18n::c(retort->i18n));
      
      if (req.type == Retort::Requirement::Type::ANY_SCHOOL_AT_LEAST)
      {
        errorMessage(
          prefix +
          fmt::sprintf(no_enough_books_any_school,
            req.count,
            req.count > 1 ? "s" : "",
            req.times > 1 ? std::to_string(req.times) + " " : "",
            req.times> 1 ? "s" : "")
        );
        
        assert(retort->requirements.size() == 1);
      }
      else if (req.type == Retort::Requirement::Type::SPECIFIC_SCHOOL_AT_LEAST)
      {
        std::string suffix = "";
        for (const auto& r : retort->requirements)
        {
          assert(r.type == Retort::Requirement::Type::SPECIFIC_SCHOOL_AT_LEAST);
          if (!suffix.empty()) suffix += ", ";
          suffix += fmt::sprintf(no_enough_books_specific_school, r.count, r.count > 1 ? "s" : "", "Chaos"); // TODO: use localized school name
        }
        
        errorMessage(prefix + suffix);
      }
    }
  }
  
  bookPhaseOkButton->activateIf(freePicks() == 0);
}

const Spell* NewGameView::spellForSlot(SpellRarity rarity, School school, s32 slot)
{
  static std::array<Spell*, 10> spells = { { nullptr } };

  const char* spellNames[] = { "True Sight", "Plane Shift", "Resurrection", "Dispel Evil", "Planar Seal", "Unicorns", "Raise Dead", "Planar Travel", "Heavenly Light", "Prayer" };
  
  if (!spells[0])
    for (int i = 0; i < 10; ++i)
      spells[i] = new Spell(I18::SPELL_HEALING, SpellType::UNIT_SKILL, SpellRarity::COMMON, SpellKind::UNIT_SPELL, SpellDuration::PERMANENT, School::LIFE, Target::FRIENDLY_UNIT, ManaInfo(0,0,0,0,0,0));

  return spells[slot];
}

void NewGameView::spellToggled(const Spell* spell)
{
  std::set<const Spell*>& picks = info.spells[spell->school][spell->rarity];
  auto& scd = spellChoiceData;
  
  auto& available = scd.spellChoicePicks[spell->school][spell->rarity];
  auto total = scd.spellChoiceTotals[spell->school][spell->rarity];
  
  auto it = picks.find(spell);
  
  if (it == picks.end())
  {
    if (available == 0 && total > 1)
    {
      if (total > 1)
      {
        errorMessage("You have no picks left in this area, to deselect click on a selected item"); //TODO: localize
        return;
      }
      else
        picks.clear();
    }

    picks.insert(spell);
    --available;
  }
  else
  {
    picks.erase(it);
    ++available;
  }
  
  s32 leftPicks = std::accumulate(scd.shownRarities.begin(), scd.shownRarities.end(), 0, [spell, &scd] (s32 v, const optional<SpellRarity>& rarity) {
    return !rarity.isPresent() ? v : (v + scd.spellChoicePicks[spell->school][rarity]);
  });
  bookPhaseOkButton->activateIf(leftPicks == 0);
}

void NewGameView::switchToPhase(Phase phase)
{
  const auto* face = fonts.darkBoldFont;
  const u16 baseX[] = { 169, 245 };
  const u16 baseY = 27;
  const u16 baseButtonSprite = 9;
  const u16 deltaY = 22;
  
  buttons.clear();
  bookPhaseOkButton = nullptr;
  
  switch (phase)
  {
    case Phase::PORTRIT_CHOICE:
    case Phase::WIZARD_CHOICE:
    {
      isPremadeWizard = true;
      info.portrait = nullptr;
      info.books = school_value_map(0);
      break;
    }
    default: break;
  }
  
  switch (phase)
  {
    case Phase::GAME_OPTIONS:
    {
      static const char* names[] = { "difficulty", "opponents", "land size", "magic" };
      static const u16 spriteIndex[] = { 4, 5, 6, 7 };
      static const SettingGroupID groups[] = { SETTING_DIFFICULTY, SETTING_OPPONENTS, SETTING_LAND_SIZE, SETTING_MAGIC_POWER };
      
      for (size_t i = 0; i < 4; ++i)
      {
        const auto button = addButton(Button::buildOffsetted(names[i], gameOptionsButtonPositions[i].x, gameOptionsButtonPositions[i].y, LSI(NEWGAME, spriteIndex[i])));
        
        button->setAction([this, i, face, button]() {
          settings.group(groups[i]).next();
          button->setTextInfo(TextInfo(settings.group(groups[i]).currentValue(), face));

        });
        
        button->setTextInfo(TextInfo(settings.group(groups[i]).currentValue(), face));
      }
      
      addButton(Button::buildBistate("Cancel", 171, 179, LSI(NEWGAME, 3)))->setAction([this](){ gvm->switchView(VIEW_START); });
      addButton(Button::buildBistate("Ok", 252, 179, LSI(NEWGAME, 2)))->setAction([this](){ switchToPhase(Phase::WIZARD_CHOICE); });
      
      break;
    }
      
    case Phase::WIZARD_CHOICE:
    {
      const auto& wizards = Data::values<const Wizard*>();
      
      //TODO: labels are not correctly centered on buttons, verify buildOffsetted
      
      auto it = wizards.obegin();
      for (size_t i = 0; i < wizards.size(); ++i, ++it)
      {
        const Wizard* wizard = *it;
        const WizardGfxSpec& gfx = GfxData::wizardGfx(wizard);
        
        const auto button = addButton(Button::buildOffsetted(i18n::s(gfx.name), baseX[i/7], baseY + deltaY*(i%7), LSI(NEWGAME, baseButtonSprite+i)));
        button->setTextInfo(TextInfo(i18n::s(gfx.name), face));
        button->setOnEnterAction([this,wizard]() {
          info.portrait = wizard;
          info.books = wizard->defaultBooks;
          info.retorts = wizard->defaultRetorts;
          info.name = i18n::s(GfxData::wizardGfx(wizard).name);
        });
      }

      const auto customButton = addButton(Button::buildOffsetted("custom", baseX[1], baseY+deltaY*7, LSI(NEWGAME, 23)));
      customButton->setTextInfo(TextInfo("Custom", face));
      customButton->setAction([this]() { switchToPhase(Phase::PORTRIT_CHOICE); });
      customButton->setOnEnterAction([this]() {
        info.portrait = nullptr;
        info.books = school_value_map(0);
        info.retorts.clear();
        info.name = "";
      });
      
      break;
    }
      
    case Phase::PORTRIT_CHOICE:
    {
      const auto& wizards = Data::values<const Wizard*>();
      
      auto it = wizards.obegin();
      for (size_t i = 0; i < wizards.size(); ++i, ++it)
      {
        const Wizard* wizard = *it;
        const WizardGfxSpec& gfx = GfxData::wizardGfx(wizard);
        
        const auto button = addButton(Button::buildOffsetted(i18n::s(gfx.name), baseX[i/7], baseY + deltaY*(i%7), LSI(NEWGAME, baseButtonSprite+i)));
        button->setTextInfo(TextInfo(i18n::s(gfx.name), face));
        button->setOnEnterAction([this,wizard]() { info.portrait = wizard; });
      }
      
      break;
    }
      
    case Phase::NAME_CHOICE:
    {
      
      break;
    }
      
    case Phase::BOOKS_CHOICE:
    {
      //TODO: background
      bookPhaseOkButton = addButton(Button::buildOffsetted("books_choice_ok", 252, 182, LSI(NEWGAME, 42), LSI(NEWGAME, 43)));
      bookPhaseOkButton->deactivate();
      
      bookPhaseOkButton->setAction([this] () {
        /* if any school has > 1 books we must switch to spell choice */
        if (std::any_of(info.books.begin(), info.books.end(), [] (auto c) { return c > 1; }))
        {
          switchToPhase(Phase::SPELLS_CHOICE);
        }
      });
      
      break;
    }
      
    case Phase::SPELLS_CHOICE:
    {
      //TODO: background
      bookPhaseOkButton = addButton(Button::buildOffsetted("spells_choice_ok", 252, 182, LSI(NEWGAME, 42), LSI(NEWGAME, 43)));
      bookPhaseOkButton->deactivate();
      
      for (const School school : Data::schoolsWithoutArcane())
      {
        auto&& guaranteed = g->spellMechanics.guaranteedSpells(school, info.books[school]);
        spellChoiceData.spellChoiceTotals[school] = guaranteed;
        spellChoiceData.spellChoicePicks[school] = guaranteed;
      }

      for (School school : Data::schoolsWithoutArcane())
      {
        if (info.books[school] > 1)
        {
          spellChoiceData.currentSchool = school;
          
          auto count = spellChoiceData.spellChoiceTotals[school];
          
          /* TODO: this should be enhanced to be generic, for now it just has two cases
             which are the one currently possible in the game */
          
          if (count[SpellRarity::COMMON] == 10)
          {
            spellChoiceData.shownRarities[0] = SpellRarity::UNCOMMON;
            spellChoiceData.shownRarities[1] = SpellRarity::RARE;
          }
          else
          {
            spellChoiceData.shownRarities[0] = SpellRarity::COMMON;
            spellChoiceData.shownRarities[1] = optional<SpellRarity>();
          }
          
          break;
        }
      }
    }
      
    case Phase::RACE_CHOICE:
    {
      /* cache races names for race choice */
      /* algorithm is generic although there are just two planes */
      if (sortedRaces[Plane::ARCANUS].empty())
      {
        const auto& races = Data::values<const Race*>();
        
        /* split by plane */
        for (const auto& race : races)
          sortedRaces[race.second->startingPlane].push_back(race.second);

        /* sort alphabetically */
        for (auto& races : sortedRaces)
        {
          std::sort(races.begin(), races.end(), [](const Race* r1, const Race* r2) {
            auto i1 = GfxData::raceGfxSpec(r1).name;
            auto i2 = GfxData::raceGfxSpec(r2).name;
            return i18n::s(i1) < i18n::s(i2);
          });
        }
      }
      
      break;
    }
  }
  
  this->phase = phase;
}

struct SpellChoice
{
  static constexpr s32 X[] = { 174, 248, 316 };
  static constexpr s32 ROWS = 5;
  static constexpr s32 ROW_HEIGHT = 7;
  static constexpr s32 BASE_Y = 38;
  static constexpr s32 DELTA_Y = 51;
};

constexpr s32 SpellChoice::X[];

void NewGameView::draw()
{
  Gfx::draw(main_bg, 0, 0);
  const Palette* palette = LBXU(main_bg).palette();

  /*Gfx::drawGrayScale(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30);
  Gfx::drawGlow(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30, CHAOS);*/
    
  switch (phase)
  {
    case Phase::GAME_OPTIONS:
    {
      Gfx::draw(main_bg, 0, 0);
      //TODO: draw buttons bg
      //const Color buttonBG = Color(12,12,12);
      Gfx::draw(options_bg, palette, 165, 0);
      //Gfx::fillRect(0, 0, 100, 20, buttonBG);
      
      break;
    }
      
    case Phase::WIZARD_CHOICE:
    {
      Gfx::draw(main_bg, 0, 0);
      Fonts::drawString("Select Wizard", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(wizard_choice_bg, palette, 165, 17);
      break;
    }
      
    case Phase::PORTRIT_CHOICE:
    {
      Gfx::draw(main_bg, 0, 0);
      Fonts::drawString("Select Picture", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(portrait_choice_bg, palette, 165, 17);
      break;
    }
      
    case Phase::NAME_CHOICE:
    {
      Gfx::draw(main_bg, 0, 0);
      Fonts::drawString("Wizard's Name", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(name_choice_bg, palette, 165+16, 17);
      nameField.draw();
      break;
    }
      
    case Phase::BOOKS_CHOICE:
    {
      Gfx::draw(books_choice_bg, palette, 0, 0);
      Fonts::drawString(std::to_string(availablePicks - countPicks())+ " picks", fonts.brightBoldFont, 221, 184, ALIGN_CENTER);
      
      /* draw spellbooks */
      for (size_t i = 0; i < SCHOOL_NO_ARCANE_COUNT; ++i)
      {
        School school = CommonDraw::schools[i];
        CommonDraw::drawSpellBooks(school, info.books[school], Point(197, 49 + 26*i));
      }
      
      /* draw retort list */
      {
        const static u16 X[] = { 170, 209, 259 };
        const static u16 Y = 4;
        const static u16 H = 6, RH = 7;
        
        const auto& retorts = Data::values<const Retort*>();
        auto it = retorts.obegin();
        for (u16 i = 0; i < retorts.size(); ++i, ++it)
        {
          const Retort* retort = *it;
          
          bool canBePicked = info.retorts.size() < MAX_RETORTS && retort->canBePicked(availablePicks - countPicks(), info.books);
          const FontSpriteSheet* font = canBePicked ? fonts.tinyBright : fonts.tinyInactive;
          
          Point pos = Point(X[i / H], Y + (i % H) * RH);
          
          if (info.retorts.find(retort) != info.retorts.end())
          {
            Gfx::draw(checkmark, pos.delta(-5, 2));
            font = fonts.tinyGold;
          }
          
          Fonts::drawString(i18n::s(retort->i18n), font, pos.x, pos.y, ALIGN_LEFT);
        }
      }

      break;
    }
      
    case Phase::SPELLS_CHOICE:
    {
      const School school = spellChoiceData.currentSchool;
      
      const auto& scd = spellChoiceData;
      const auto& schoolGfx = GfxData::schoolGfxSpec(school);
      
      const fonts::SerifFont titleFont(&fonts.schoolFonts[school]);
      const fonts::MediumBoldFont headerFont(&fonts.schoolFonts[school]);
      
      Fonts::drawString(fmt::sprintf("Select %s Spells", i18n::c(schoolGfx.name)), &titleFont, 241, 5, ALIGN_CENTER); //TODO: localize

      /* top divider */
      Gfx::draw(divider, palette, {181, 18});
      /* bottom frame - no black border but in original game there is, why? */
      Gfx::draw(bottom_box_spell_choice, palette, {196, 180});

      static const spell_rarity_map<std::string> raritiesNames = {{ SpellRarity::COMMON, "Common" }, { SpellRarity::UNCOMMON, "Uncommon" }, { SpellRarity::RARE, "Rare" }, { SpellRarity::VERY_RARE, "Very Rare" }};

      for (s32 i = 0; i < scd.shownRarities.size(); ++i)
      {
        if (scd.shownRarities[i].isPresent())
        {
          const SpellRarity rarity = scd.shownRarities[i];
          auto guaranteed = scd.spellChoiceTotals[school][rarity];

          Fonts::drawString(fmt::sprintf("%s: %d", raritiesNames[rarity], guaranteed), &headerFont, 166, 37 - 12 + 51*i, ALIGN_LEFT);
          
          //TODO: draw correct dark region
          Gfx::draw(SpriteInfo(dark_region_1).relative(i), palette, Point(167, 37 + 51*i));
          
          for (s32 s = 0; s < 10; ++s)
          {
            const Spell* spell = spellForSlot(rarity, school, s);
            const bool isPicked = info.spells[school][rarity].find(spell) != info.spells[school][rarity].end();
            
            const auto* font = isPicked ? fonts.tinyGold : fonts.tinyBright;
            
            Point p = Point(SpellChoice::X[(s / SpellChoice::ROWS)], SpellChoice::BASE_Y + SpellChoice::DELTA_Y*i + (s % SpellChoice::ROWS) * SpellChoice::ROW_HEIGHT);
            Fonts::drawString(i18n::s(spell->name), font, p.x, p.y, ALIGN_LEFT);
            
            if (isPicked)
              Gfx::draw(checkmark, p.delta(-5, 2));

          }
        }
      }

      /* TODO: code already used in spellToggled */
      s32 leftPicks = std::accumulate(scd.shownRarities.begin(), scd.shownRarities.end(), 0, [school, &scd] (s32 v, const optional<SpellRarity>& rarity) {
        return !rarity.isPresent() ? v : (v + scd.spellChoicePicks[school][rarity]);
      });
      
      Fonts::drawString(std::to_string(leftPicks)+ " picks", fonts.brightBoldFont, 221, 184, ALIGN_CENTER);

      break;
    }
      
    case Phase::RACE_CHOICE:
    {
      Fonts::drawString("Select Race", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(divider2, palette, 164, 18);
      Gfx::draw(races_background, palette, 208, 34);
      
      const fonts::MediumFont activeFont(&fonts.brightMedium);
      const fonts::MediumFont inactiveFont(&fonts.inactiveMedium);


      /* draw races list */
      int y = 37;
      for (const Race* race : sortedRaces[Plane::ARCANUS])
      {
        Fonts::drawString(i18n::s(GfxData::raceGfxSpec(race).name), &activeFont, 219, y, ALIGN_LEFT);
        y += 10;
      }
      
      y = 146;
      const auto& myrranFont = info.retorts.find(Data::retort("myrran")) == info.retorts.end() ? inactiveFont : activeFont;
      for (const Race* race : sortedRaces[Plane::MYRRAN])
      {
        Fonts::drawString(i18n::s(GfxData::raceGfxSpec(race).name), &myrranFont, 219, y, ALIGN_LEFT);
        y += 10;
      }
      
      arcanusRacesGrid = clickable_grid(210, 36, 60, 10, sortedRaces[Plane::ARCANUS].size(), 1);
      arcanusRacesGrid.forEachCell([] (coord_t x, coord_t y, coord_t w, coord_t h) {
        Gfx::rect(x, y, w, h, { 255, 0, 0});
      });
      
      myrranRacesGrid = clickable_grid(210, 145, 60, 10, sortedRaces[Plane::MYRRAN].size(), 1);
      myrranRacesGrid.forEachCell([] (coord_t x, coord_t y, coord_t w, coord_t h) {
        Gfx::rect(x, y, w, h, { 255, 0, 0});
      });

      break;
    }
  }
  
  if (info.portrait)
  {
    Gfx::draw(GfxData::wizardGfx(info.portrait).portraitLarge, 24, 10);
    Fonts::drawString(info.name, fonts.darkSerifFont, 76, 118, ALIGN_CENTER);
  }
  
  drawRetortList();
  
  CommonDraw::drawSpellBooks(info.books, Point(36, 135), false);
}

bool NewGameView::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (phase == Phase::NAME_CHOICE)
    nameField.keyReleased(key, kkey, mod);
  
  if (key == KeyboardCode::SDL_SCANCODE_ESCAPE)
  {
    switch (phase)
    {
      case Phase::PORTRIT_CHOICE: switchToPhase(Phase::WIZARD_CHOICE); break;
      
      case Phase::SPELLS_CHOICE: switchToPhase(Phase::BOOKS_CHOICE); break;
    }

    return true;
  }
  
  return false;
}

bool NewGameView::textInput(sdl_text_input data)
{
  if (phase == Phase::NAME_CHOICE)
  {
    nameField.textInput(data);
    return true;
  }
  
  return false;
}

struct BooksChoice
{
  static constexpr u16 BX = 189, BY = 44;
  static constexpr u16 BW = 8, BH = 26, VH = 22;
  static constexpr u16 MX = 13, MY = 5;
  
  static bool isInside(s32 x, s32 y)
  {
    int rx = (x - BX) / BW;
    int ry = (y - BY) / BH;
    return rx >= 0 && rx <= MX && ry >= 0 && ry < MY;
  }
  
  static std::pair<School, s32> get(s32 x, s32 y)
  {
    int rx = (x - BX) / BW;
    int ry = (y - BY) / BH;
    
    if (rx >= 0 && rx <= MX && ry >= 0 && ry < MY)
    {
      if (((y - BY) % BH) > (BH - VH))
      {
        School school = Data::schoolsWithoutArcane()[ry];
        return std::make_pair(school, rx);
      }
    }
    
    return std::make_pair(School::NO_SCHOOL, 0);
  }
};


bool NewGameView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (phase == Phase::BOOKS_CHOICE)
  {
    /* books selection grid */
    {
      auto selection = BooksChoice::get(x, y);
      
      if (selection.first != School::NO_SCHOOL)
        booksPicked(selection.first, selection.second);
    }
    
    /* retort selection grid */
    {
      const static u16 X[] = { 170, 209, 259 };
      const static u16 Y = 4;
      const static u16 H = 6, RH = 7;
      
      if (x >= X[0] - 5 && y >= Y && y < Y + H*RH)
      {
        int v = (y - Y) / RH;
        int r = x < 209 ? 0 : (x < 259 ? 1 : 2);
        
        int index = v + r*H;
        
        const Retort* retort = Data::values<const Retort*>()[index];
        retortToggled(retort);
      }
    }
  }
  else if (phase == Phase::SPELLS_CHOICE)
  {
    /* spells choice */
    const auto& scd = spellChoiceData;
    const auto& sr = scd.shownRarities;
    
    for (s32 i = 0; i < sr.size(); ++i)
    {
      if (sr[i].isPresent())
      {
        const SpellRarity rarity = sr[i];
        const s32 baseY = SpellChoice::BASE_Y + i * SpellChoice::DELTA_Y;
        
        if (x >= SpellChoice::X[0] && x <= SpellChoice::X[2])
        {
          if (y >= baseY && y < baseY + SpellChoice::ROW_HEIGHT * SpellChoice::ROWS)
          {
            const s32 c = x > SpellChoice::X[1] ? 1 : 0;
            const s32 r = (y - baseY) / SpellChoice::ROW_HEIGHT;
            const s32 i = r + c * SpellChoice::ROWS;
            
            const Spell* spell = spellForSlot(rarity, scd.currentSchool, i);
            spellToggled(spell);
          }
        }
      }
    }
  }
  
  
  return true;
}

bool NewGameView::mouseDragged(u16 x, u16 y, MouseButton b)
{
  switch (phase)
  {
    case Phase::BOOKS_CHOICE:
    {
      if (BooksChoice::get(x, y).first != School::NO_SCHOOL)
        mouseReleased(x,y,b);
    }
      
    default:
      ;
  }
  
  return true;
};
