#include "Common.h"

#include "i18n/Localization.h"
#include "gfx/Gfx.h"
#include "game/spells/Spells.h"

#include "gfx/LBXRepository.h"
#include "gfx/Texture.h"

using namespace std;

static char buffer[512];
void debugprintf(const char* str, ...)
{
  va_list args;
  va_start (args, str);
  vsnprintf (buffer, 512, str, args);
  printf("%s\n", buffer);
}

SpriteInfo::SpriteInfo(TextureID texture, u16 i) : SpriteInfo(texture, i / Texture::get(texture)->cols, i % Texture::get(texture)->cols)
{
	static_assert(sizeof(SpriteInfo) == sizeof(SpriteInfo::data_type), "");
}

const SpriteSheet* SpriteInfo::sheet() const
{
  return isLBX() ? static_cast<const SpriteSheet*>(lbx::Repository::spriteFor(*this)) : Texture::get(texture());
}

u16 SpriteInfo::count() const
{
  assert(isLBX());
  const lbx::LBXSpriteData* data = static_cast<const lbx::LBXSpriteData*>(sheet());
  return data->count;
}

const Palette* SpriteInfo::palette() const
{
  assert(isLBX());
  const lbx::LBXSpriteData* data = static_cast<const lbx::LBXSpriteData*>(sheet());
  return data->palette;
}

Point Point::INVALID = Point(-1,-1);
Point Point::ZERO = Point(0,0);

const Color Color::WHITE = Color(0xFF, 0xFF, 0xFF);
const Color Color::BLACK = Color(0, 0, 0);
const Color Color::NONE = Color(0, 0, 0, 0);
