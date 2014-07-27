#ifndef _VIEW_MANAGER_H_
#define _VIEW_MANAGER_H_

#include "EventListener.h"

#include <list>

class Animation;
class View;

enum ViewID
{
  VIEW_MAIN,
  VIEW_SPELLS,
  VIEW_MAGIC,
  VIEW_RESEARCH,
  VIEW_ALCHEMY,
  VIEW_ARMIES,
  VIEW_ARMIES_ITEMS,
  VIEW_ITEM_CRAFT,
  VIEW_CITY,
  VIEW_UNIT,
  VIEW_ARMY,
  VIEW_MESSAGE,
  VIEW_MIRROR,
  VIEW_COMBAT,
  VIEW_PRODUCTION,
  VIEW_OUTPOST,
  VIEW_NEW_GAME,
  
  VIEW_COUNT
};

class ViewManager : public EventListener
{
  private:
    View* views[VIEW_COUNT];
    View* current;
    std::list<View*> overviews;
  
    std::list<Animation*> animations;
    bool animating;
  
  public:
    ViewManager();
  
    void push(Animation* animation)
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
  
  
    static ViewManager gvm;
};

#endif
