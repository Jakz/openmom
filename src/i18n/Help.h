#pragma once

#include "common/Common.h"

#include <string>
#include <unordered_map>

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
    static std::unordered_map<std::string, const Paragraph*> helpMapping;
    
  public:
    static const Paragraph* at(size_t i) { return &data[i]; }
    static const Paragraph* get(const std::string& i) { auto it = helpMapping.find(i); return it != helpMapping.end() ? it->second : nullptr; }
    
    friend class lbx::Repository;
  };
}

