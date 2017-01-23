#include "CombatUtils.h"

using namespace combat;

constexpr const s16 CombatCoord::DIRS[12][2];
constexpr const u16 CombatCoord::DIRS_LENGTH;

CombatCoord CombatCoord::neighbour(Dir facing) const
{
  const s16* offsets = dirs(facing, this->y % 2 == 0);
  return CombatCoord(this->x + offsets[0], this->y + offsets[1]);
}
