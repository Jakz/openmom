#pragma once

#include <string>
#include "Common.h"

namespace dialogs
{
  void drawErrorDialog(const std::string& text);
    
  class InfoMenu
  {
  private:
    ScreenCoord b;
    u16 w, h;
    
  public:
    InfoMenu(u16 x, u16 y, u16 w, u16 h) : b(x,y), w(w), h(h) { }
    
    void draw();
  };
}
