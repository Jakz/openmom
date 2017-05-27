#pragma once

#include <string>
#include "Common.h"

namespace dialogs
{
  void drawErrorDialog(const std::string& text);
    
  class InfoMenu
  {
  private:
    Point b;
    Size s;
    int buttonWidth;
    int rows;
    int hovered;
    
  public:
    InfoMenu(const Point& position, u16 rows, u16 buttonWidth);
    
    void draw();
    
    Point buttonBase() const;
    int buttonHeight() const;
    
    class Delegate
    {
      virtual void buttonClicked(int index) = 0;
      virtual const std::string& nameForButton(int index) = 0;
    };
  };
  
  class ItemDetailDialog
  {
  private:
    Point b;
    
  public:
    ItemDetailDialog(Point base) : b(base) { }
    void draw();
    
    
  };
}
