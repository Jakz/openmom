//
//  mystrings.h
//  OpenMoM
//
//  Created by Jack on 5/7/17.
//  Copyright (c) 2014 Jack. All rights reserved.
//
#pragma once

#include "common/Common.h"

#include <string>
#include <vector>

class strings
{
public:
  static std::string groupDigits(u32 value);
  static std::string tolower(const std::string& text);
  static bool compareInsensitive(const std::string& s1, const std::string& s2) { return tolower(s1) == tolower(s2); }
  
  static const std::string join(const std::vector<const std::string>& tokens, s16 s, s16 e);
  static void split(std::string string, std::vector<const std::string>& tokens, s8 delim);
};
