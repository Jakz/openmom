#include "ViewManager.h"

#include "LocalPlayer.h"
#include "Game.h"
#include "Animations.h"
#include "View.h"
#include "SDL.h"
#include "Texture.h"
#include "Gfx.h"

#include "MainView.h"
#include "NewGameView.h"

using namespace std;

ViewManager ViewManager::gvm;

ViewManager::ViewManager() : views{nullptr}, animating(false)
{
  views[VIEW_MAIN] = new MainView();
  
  views[VIEW_NEW_GAME] = new NewGameView();
}

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
  overviews.push_front(overview);
}

void ViewManager::closeOverview()
{
  overviews.front()->doDeactivate();
  overviews.pop_front();
}

void ViewManager::draw()
{
  list<Animation*>::iterator it = animations.begin();
  
  while (it != animations.end())
  {
    Animation* a = *it;
    if (a->hasFinished(SDL::ticks))
    {
      it = animations.erase(it);
      
      if (a->next())
      {
        a->reset(SDL::ticks);
        push(a->next());
      }
      
      delete a;
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
    Gfx::draw(DARKNER, 0, 0);
    
    for (View *v : overviews)
      v->doDraw();
  }
  
  for (Animation *a : animations)
    a->step();
  
  current->drawPost();
}

void ViewManager::mouseClicked(u16 x, u16 y, MouseButton b)
{
  if (!animating)
    respondingView()->doMouseClicked(x, y, b);
}

void ViewManager::mouseDragged(u16 x, u16 y, MouseButton b)
{
  if (!animating)
    respondingView()->doMouseDragged(x, y, b);
}

void ViewManager::mouseMoved(u16 x, u16 y, MouseButton b)
{
  if (!animating)
    respondingView()->doMouseMoved(x, y, b);
}

void ViewManager::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (!animating)
    respondingView()->doMouseReleased(x, y, b);
}

void ViewManager::mousePressed(u16 x, u16 y, MouseButton b)
{
  if (!animating)
    respondingView()->doMousePressed(x, y, b);
}

void ViewManager::keyPressed(KeyboardKey key, KeyboardMod mod)
{
  if (!animating)
    respondingView()->doKeyPressed(key, mod);
}

void ViewManager::keyReleased(KeyboardKey key, KeyboardMod mod)
{
  if (!animating)
    respondingView()->doKeyReleased(key, mod);
}