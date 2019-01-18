#pragma once

#include "common/Common.h"

#include <vector>
#include <memory>

template<typename T, typename S> class DrawElement;

/* this is a simple auto-sorting drawing queue which can be used
   to arrange drawing of generic objects
 */
template<typename T, typename S = std::less<std::unique_ptr<T>>>
class DrawQueue
{
private:
  bool _dirty;
  std::vector<std::unique_ptr<T>> _elements;

public:
  DrawQueue()
  {
    static_assert(std::is_base_of<DrawElement<T,S>, T>::value, "Template type must be subclass of DrawElement");
  }
  
  using iterator = typename decltype(_elements)::iterator;
  using const_iterator = typename decltype(_elements)::const_iterator;
  
  iterator begin() { return _elements.begin(); }
  iterator end() { return _elements.end(); }
  const_iterator begin() const { return _elements.begin(); }
  const_iterator end() const { return _elements.end(); }
  
  template<typename U> void erase(U s, U e) { _elements.erase(s, e); }
  
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
    element->setQueue(this);
    _elements.push_back(std::unique_ptr<T>(element));
    _dirty = true;
  }
  
  void draw()
  {
    if (_dirty)
      sort();
    
    for (const auto& element : _elements)
      if (element->isVisible())
        element->draw();
  }
  
  void setDirty() { _dirty = true; }

  using type = T;
};

template<typename T, typename S = std::less<std::unique_ptr<T>>>
class DrawElement
{
private:
  bool visible;
  DrawQueue<T,S>* queue;
  
protected:
  DrawElement() : queue(nullptr), visible(true) { }
  
public:
  void setQueue(DrawQueue<T,S>* queue) { this->queue = queue; }
  void setDirty() const { queue->setDirty(); }
  
  bool isVisible() const { return visible; }
  void setVisible(bool visible) { this->visible = visible; }
};
