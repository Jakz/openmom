#ifndef _VIEW_MANAGER_H_
#define _VIEW_MANAGER_H_

#include "Common.h"
#include "EventListener.h"

#include <list>

namespace anims { class Animation; }

class View;
class AlchemyView;
class ArmiesItemsView;
class ArmiesView;
class ArmyView;
class CityView;
class CombatView;
class ItemCraftView;
class MagicView;
class MainView;
class MessageView;
class MirrorView;
class NewGameView;
class OutpostView;
class ProductionView;
class ResearchView;
class SpellBookView;
class UnitDetailView;

class ViewManager : public EventListener
{
  private:
    View* views[VIEW_COUNT];
    View* current;
    std::list<View*> overviews;
  
    std::list<anims::Animation*> animations;
    bool animating;
  
  public:
    ViewManager();
  
    void push(anims::Animation* animation)
    {
      animating = true;
      animations.push_back(animation);
    }
  
    void switchView(ViewID type);
    void switchOverview(ViewID type);
    void closeOverview();
  
    void draw();
  
    void mousePressed(u16 x, u16 y, MouseButton b) override;
    void mouseClicked(u16 x, u16 y, MouseButton b) override;
    void mouseReleased(u16 x, u16 y, MouseButton b) override;
    void mouseMoved(u16 x, u16 y, MouseButton b) override;
    void mouseDragged(u16 x, u16 y, MouseButton b) override;
    
    void keyPressed(KeyboardKey key, KeyboardMod mod) override;
    void keyReleased(KeyboardKey key, KeyboardMod mod) override;
  
    inline bool isThereOverview() { return !overviews.empty(); }
    inline View* firstOverview() { return isThereOverview() ? overviews.front() : nullptr; }
    inline View* respondingView() { return isThereOverview() ? overviews.front() : current; }
  
  AlchemyView* alchemyView();
  ArmiesItemsView* armiesItemsView();
  ArmiesView* armiesView();
  ArmyView* armyView();
  CityView* cityView();
  CombatView* combatView();
  ItemCraftView* itemCraftView();
  MagicView* magicView();
  MainView* mainView();
  MessageView* messageView();
  MirrorView* mirrorView();
  NewGameView* newGameView();
  OutpostView* outpostView();
  ProductionView* productionView();
  ResearchView* researchView();
  SpellBookView* spellBookView();
  UnitDetailView* unitDetailView();
  
    static ViewManager gvm;
};

#endif
