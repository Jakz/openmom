#include "Help.h"

using namespace help;

std::vector<Paragraph> help::Data::data;
std::unordered_map<std::string, const Paragraph*> help::Data::helpMapping;
