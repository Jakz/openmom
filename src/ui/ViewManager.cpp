#include "ViewManager.h"

#include "LocalPlayer.h"
#include "Game.h"
#include "Animations.h"
#include "View.h"
#include "SDLHelper.h"
#include "Texture.h"
#include "Gfx.h"

#include "AlchemyView.h"
#include "ArmiesItemsView.h"
#include "ArmiesView.h"
#include "ArmyView.h"
#include "CityView.h"
#include "CombatView.h"
#include "ItemCraftView.h"
#include "MagicView.h"
#include "MainView.h"
#include "MessageView.h"
#include "MirrorView.h"
#include "NewGameView.h"
#include "OutpostView.h"
#include "ProductionView.h"
#include "ResearchView.h"
#include "SpellBookView.h"
#include "UnitDetailView.h"

#include "ConsoleView.h"

using namespace std;
using namespace anims;

ViewManager::ViewManager() : views{nullptr}, animating(false), current(nullptr)
{
  views[VIEW_ALCHEMY] = new AlchemyView(this);
  views[VIEW_ARMIES_ITEMS] = new ArmiesItemsView(this);
  views[VIEW_ARMIES] = new ArmiesView(this);
  views[VIEW_ARMY] = new ArmyView(this);
  views[VIEW_CITY] = new CityView(this);
  views[VIEW_COMBAT] = new CombatView(this);
  views[VIEW_ITEM_CRAFT] = new ItemCraftView(this);
  views[VIEW_MAGIC] = new MagicView(this);
  views[VIEW_MAIN] = new MainView(this);
  views[VIEW_MESSAGE] = new MessageView(this);
  views[VIEW_MIRROR] = new MirrorView(this);
  views[VIEW_NEW_GAME] = new NewGameView(this);
  views[VIEW_OUTPOST] = new OutpostView(this);
  views[VIEW_PRODUCTION] = new ProductionView(this);
  views[VIEW_RESEARCH] = new ResearchView(this);
  views[VIEW_SPELL_BOOK] = new SpellBookView(this);
  views[VIEW_UNIT] = new UnitDetailView(this);
  
  views[VIEW_CONSOLE] = new ConsoleView(this);
}

AlchemyView* ViewManager::alchemyView() { return static_cast<AlchemyView*>(views[VIEW_ALCHEMY]); }
ArmiesItemsView* ViewManager::armiesItemsView() { return static_cast<ArmiesItemsView*>(views[VIEW_ARMIES_ITEMS]); }
ArmiesView* ViewManager::armiesView() { return static_cast<ArmiesView*>(views[VIEW_ARMIES]); }
ArmyView* ViewManager::armyView() { return static_cast<ArmyView*>(views[VIEW_ARMY]); }
CityView* ViewManager::cityView() { return static_cast<CityView*>(views[VIEW_CITY]); }
CombatView* ViewManager::combatView() { return static_cast<CombatView*>(views[VIEW_COMBAT]); }
ItemCraftView* ViewManager::itemCraftView() { return static_cast<ItemCraftView*>(views[VIEW_ITEM_CRAFT]); }
MagicView* ViewManager::magicView() { return static_cast<MagicView*>(views[VIEW_MAGIC]); }
MainView* ViewManager::mainView() { return static_cast<MainView*>(views[VIEW_MAIN]); }
MessageView* ViewManager::messageView() { return static_cast<MessageView*>(views[VIEW_MESSAGE]); }
MirrorView* ViewManager::mirrorView() { return static_cast<MirrorView*>(views[VIEW_MIRROR]); }
NewGameView* ViewManager::newGameView() { return static_cast<NewGameView*>(views[VIEW_NEW_GAME]); }
OutpostView* ViewManager::outpostView() { return static_cast<OutpostView*>(views[VIEW_OUTPOST]); }
ProductionView* ViewManager::productionView() { return static_cast<ProductionView*>(views[VIEW_PRODUCTION]); }
ResearchView* ViewManager::researchView() { return static_cast<ResearchView*>(views[VIEW_RESEARCH]); }
SpellBookView* ViewManager::spellBookView() { return static_cast<SpellBookView*>(views[VIEW_SPELL_BOOK]); }
UnitDetailView* ViewManager::unitDetailView() { return static_cast<UnitDetailView*>(views[VIEW_UNIT]); }

ConsoleView* ViewManager::consoleView() { return static_cast<ConsoleView*>(views[VIEW_CONSOLE]); }


void ViewManager::switchView(ViewID type)
{
  if (current)
    current->doDeactivate();
  current = views[type];
  current->doActivate(LocalGame::i->currentPlayer());
}

void ViewManager::switchOverview(ViewID type)
{
  View* overview = views[type];
  overview->doActivate(LocalGame::i->currentPlayer());
  overviews.push_back(overview);
}

void ViewManager::closeOverview()
{
  overviews.back()->doDeactivate();
  overviews.pop_back();
}

void ViewManager::draw()
{
  auto it = animations.begin();
  auto cend = animations.end();
  
  while (it != cend)
  {
    Animation* a = it->get();
    if (a->hasFinished())
    {
      
      if (a->next())
      {
        a->reset();
        animations.push_back(std::move(a->next()));
      }
      
      it = animations.erase(it);
    }
    else
      ++it;
  }
  
  if (animations.empty())
    animating = false;
  
  current->doDraw();
  
  if (LocalGame::i->currentPlayer()->hasMessage() && !isThereOverview())
    switchOverview(VIEW_MESSAGE);
  
  if (isThereOverview())
  {
    Gfx::draw(TextureID::DARKNER, 0, 0);

    for (View *v : overviews)
      v->doDraw();
  }
  
  for (const auto& a : animations)
    a->step();
  
  current->drawPost();
}

void ViewManager::mouseClicked(u16 x, u16 y, MouseButton b)
{
  if (animating)
    (*animations.begin())->mouseClicked(x, y, b);
  else
    respondingView()->doMouseClicked(x, y, b);
}

void ViewManager::mouseDragged(u16 x, u16 y, MouseButton b)
{
  if (animating)
    (*animations.begin())->mouseDragged(x, y, b);
  else
    respondingView()->doMouseDragged(x, y, b);
}

void ViewManager::mouseMoved(u16 x, u16 y, MouseButton b)
{
  if (animating)
    (*animations.begin())->mouseMoved(x, y, b);
  else
    respondingView()->doMouseMoved(x, y, b);
}

void ViewManager::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (animating)
    (*animations.begin())->mouseReleased(x, y, b);
  else
    respondingView()->doMouseReleased(x, y, b);
}

void ViewManager::mousePressed(u16 x, u16 y, MouseButton b)
{
  if (animating)
    (*animations.begin())->mousePressed(x, y, b);
  else
    respondingView()->doMousePressed(x, y, b);
}

void ViewManager::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (animating)
    (*animations.begin())->keyPressed(key, kkey, mod);
  else
    respondingView()->doKeyPressed(key, kkey, mod);
}

void ViewManager::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (animating)
    (*animations.begin())->keyReleased(key, kkey, mod);
  else
    respondingView()->doKeyReleased(key, kkey, mod);
}

void ViewManager::setPlayer(LocalPlayer* player)
{
  std::for_each(views, views+VIEW_COUNT, [player] (View* view) { view->setPlayer(player); });
}


//CityView* ViewManager::cityView() { return static_cast<CityView*>(views[VIEW_CITY]); }
