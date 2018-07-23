//
//  mystrings.h
//  OpenMoM
//
//  Created by Jack on 5/7/17.
//  Copyright (c) 2014 Jack. All rights reserved.
//
#include "mystrings.h"

#include <numeric>

using namespace std;

string strings::tolower(const std::string &text)
{
  string lname;
  lname.resize(text.size());
  std::transform(text.begin(), text.end(), lname.begin(), ::tolower);
  return lname;
}

std::string strings::groupDigits(u32 value)
{
  constexpr char separator = ',';
  
  //TODO: utf8 support?
  std::string str = std::to_string(value);
  
  assert(str.length() > 0);
  size_t separators = (str.length() - 1) / 3;
  
  std::string result = std::string(str.length() + separators, separator);
  
  for (size_t i = 0; i < str.length(); ++i)
  {
    result[(result.length() - 1) - (i + (i/3))] = str[str.length()-1-i];
  }
  
  return result;
}

const string strings::join(const vector<string>& tokens, s16 s, s16 e)
{
  assert(e <= tokens.size() - 1 && s >= 0 && s <= e);
  
  if (tokens.size() == 1)
    return tokens[0];
  
  size_t finalLength = std::accumulate(tokens.begin()+s, tokens.end()-(tokens.size()-e-1), 0, [](size_t v, const std::string& token) { return v + token.length(); });
  string result;
  result.reserve(finalLength + (e - s));
  
  for (int i = s; i <= e; ++i)
  {
    if (i > s)
      result += ' ';
    result += tokens[i];
  }
  
  return result;
}

void strings::split(string s, vector<std::string>& tokens, s8 delim)
{
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delim)) != std::string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(token);
    s.erase(0, pos + 1);
  }
  tokens.push_back(s);
}
