#pragma once

#include "Common.h"

#include <string>

namespace lbx {
  class Repository;
}

namespace help
{
  struct Paragraph
  {
    std::string title;
    std::string text;
    optional<SpriteInfo> icon;
    const Paragraph* next;
    
    Paragraph() = default;
    Paragraph(const char* title, const char* text) : title(title), text(text), next(nullptr) { }
    Paragraph(const char* title, const char* text, SpriteInfo icon): title(title), text(text), icon(icon), next(nullptr) { }
  };
  
  struct Entry
  {
    const Paragraph* paragraph;
    
    Entry(const Paragraph* paragraph = nullptr) : paragraph(paragraph) { }
  };
  
  class Data
  {
  private:
    static std::vector<Paragraph> data;
    
  public:
    static const Paragraph* at(size_t i) { return &data[i]; }
    
    friend class lbx::Repository;
  };
}

