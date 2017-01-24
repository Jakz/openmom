#pragma once

#include "Common.h"

#include <vector>

template<typename T, typename S>
class DrawQueue
{
private:
  bool _dirty;
  std::vector<std::unique_ptr<T>> _elements;
  
public:
  
  void sort()
  {
    std::sort(_elements.begin(), _elements.end(), S());
    _dirty = false;
  }
  
  void sortIfNeeded()
  {
    if (_dirty)
      sort();
  }
  
  void clear()
  {
    _elements.clear();
    _dirty = false;
  }
  
  void add(T* element)
  {
    _elements.push_back(std::unique_ptr<T>(element));
    _dirty = true;
  }
  
  void draw()
  {
    if (_dirty)
      sort();
    
    for (const auto& element : _elements)
      element->draw();
  }
  
  void setDirty() { _dirty = true; }
};
