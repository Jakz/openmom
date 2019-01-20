#pragma once

#include "common/Common.h"

#include <string>
#include <unordered_map>

namespace lbx {
  class Repository;
}

class yaml;

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
    static std::unordered_map<std::string, const Paragraph*> mapping;

    static std::vector<Paragraph> data;
    static std::unordered_map<std::string, const Paragraph*> lbxHelpMapping;
    
  public:
    static const Paragraph* at(size_t i) { return &data[i]; }
    static const Paragraph* get(const std::string& i) { auto it = mapping.find(i); return it != mapping.end() ? it->second : nullptr; }
    
    static const Paragraph* getFromLBX(const std::string& title)
    {
      auto it = lbxHelpMapping.find(title);
      assert(it != lbxHelpMapping.end());
      return it->second;
    }
    
    friend class lbx::Repository;
    friend class ::yaml;
  };
  
  struct Ref
  {
    const Paragraph* paragraph;
  };
}

