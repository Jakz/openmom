//
//  mystrings.h
//  OpenMoM
//
//  Created by Jack on 5/7/17.
//  Copyright (c) 2014 Jack. All rights reserved.
//
#pragma once

#include <string>
#include <vector>
#include <cstdint>

class strings
{
public:
  static std::string groupDigits(uint32_t value);
  static std::string toupper(const std::string &text);
  static std::string tolower(const std::string& text);
  static bool compareInsensitive(const std::string& s1, const std::string& s2) { return tolower(s1) == tolower(s2); }
  
  static const std::string join(const std::vector<std::string>& tokens, int32_t s, int32_t e);
  
  static void split(std::string string, std::vector<std::string>& tokens, const std::string& delims);
  static void split(std::string string, std::vector<std::string>& tokens, char delim);
};
