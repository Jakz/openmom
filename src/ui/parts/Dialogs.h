#pragma once

#include <string>
#include "Common.h"

namespace dialogs
{
  void drawErrorDialog(const std::string& text);
  
  class InfoMenu
  {
  public:
    class Delegate
    {
    public:
      virtual int buttonCount() = 0;
      virtual void buttonClicked(int index) = 0;
      virtual void drawButton(int index, bool hovered, const Rect& bounds) = 0;
    };
  
  private:
    Delegate* _delegate;
    Point b;
    Size s;
    int buttonWidth;
    int hovered;
    
  public:
    InfoMenu(Delegate* delegate, u16 buttonWidth);
    
    void draw() const;
    
    Point buttonBase() const;
    int buttonHeight() const;
    int hoveredButton(const Point& p);
    Delegate* delegate() const { return _delegate; }
    
    void setPosition(const Point& position) { this->b = position; }
    void setHovered(int hovered) { this->hovered = hovered; }
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
