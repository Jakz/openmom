#include "common/Util.h"

std::mt19937 Math::rng(std::mt19937::default_seed);
std::uniform_real_distribution<float> Math::real(0.0f,1.0f);
std::uniform_int_distribution<> Math::integer(0, std::numeric_limits<int>::max());



const std::array<PositionOffset, 8> Util::DIRS = {{{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}}};
const std::array<PositionOffset, 4> Util::ODIRS = {{{0,-1},{1,0},{0,1},{-1,0}}};
