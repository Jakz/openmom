#include "LBXRepository.h"

#include "Buildings.h"
#include "Localization.h"
#include "Help.h"

#include "platform/Platform.h"
#include "common/mystrings.h"

#include <cctype>

using namespace lbx;

size_t Repository::bytesUsed;
size_t Repository::spritesUsed;
LBXFile Repository::data[LBX_COUNT];
std::vector<LBXTerrainSpriteSpecs> Repository::terrainData;

LBXFile& Repository::file(const std::string &name)
{
  auto it = std::find_if(data, data+LBX_COUNT, [&name](const LBXFile& file) { return strings::tolower(file.fileName) == name; });
  
  assert(it < data+LBX_COUNT);
  
  return *it;
}

const LBXFile& Repository::loadLBX(LBXID ident)
{
  if (ident == LBXID::TERRAIN)
    return loadLBXTerrain();
  else if (ident == LBXID::TERRTYPE)
    return loadLBXTerrainMap();
  else if (ident == LBXID::HELP)
    return loadLBXHelp();
  else if (ident == LBXID::FONTS)
    return loadLBXFontsAndPalettes();
  
  
  LBXFile& lbx = file(ident);
  
  file_handle in = getDescriptor(lbx);
  LBX::loadHeader(lbx.info, in);
  
  if (ident == LBXID::SOUNDFX || ident == LBXID::SNDDRV || ident == LBXID::NEWSOUND || ident == LBXID::CMBTSND || ident == LBXID::INTROSFX || ident == LBXID::INTROSND)
    lbx.info.header.type = LBXFileType::SOUND;
  else if (ident == LBXID::MUSIC)
    lbx.info.header.type = LBXFileType::MUSIC;
  
  LOGD("[lbx] caching %zu assets %s", lbx.info.offsets.size(), lbx.fileName.c_str()/*, LBX::getLBXPath(lbx.fileName).c_str()*/);
  
  if (lbx.info.header.type == LBXFileType::GRAPHICS)
  {
    lbx.sprites = new LBXSpriteData*[lbx.size()];
    std::fill(lbx.sprites, lbx.sprites+lbx.size(), nullptr);
  }
  else if (lbx.info.header.type == LBXFileType::DATA_ARRAY)
  {
    lbx.arrays = new LBXArrayData*[lbx.size()];
    std::fill(lbx.arrays, lbx.arrays+lbx.size(), nullptr);
  }
  
  
  in.close();
  
  return lbx;
}

void Repository::loadMultipleLBXSpriteData(LBXID id)
{
  LBXFile& lbx = file(id);
  Path name = getLBXPath(lbx.fileName);
  FILE *in = fopen(name.c_str(), "rb");
  
  size_t count = lbx.size();
  LOGD("[lbx] loading %zu gfx entries from %s", count, lbx.fileName.c_str());
  
  for (size_t i = 0; i < count; ++i)
  {
    LBXSpriteData* spriteData;
    
    if (id != LBXID::TERRAIN)
      spriteData = LBX::scanGfx(lbx.info.header, lbx.info.offsets[i], in);
    else
      spriteData = LBX::scanTerrainGfx(lbx.info.offsets[i], terrainData[i].animated() ? 4 : 1, in);
    
    gfxAllocated(spriteData);
    lbx.sprites[i] = spriteData;
  }
  
  LOGD3("[lbx] lbx cache size: %zu sprites in %zu bytes", spritesUsed, bytesUsed);
  fclose(in);
}

const LBXSpriteData* Repository::loadLBXSpriteData(SpriteInfo info)
{
  LBXFile& lbx = file(info.lbx());
  
  Path name = getLBXPath(lbx.fileName);
  FILE *in = fopen(name.c_str(), "rb");
  
  LOGD("[lbx] loading gfx entry %u from %s", info.index(), file(info.lbx()).fileName.c_str());
  
  LBXSpriteData* spriteData;
  
  if (info.lbx() != LBXID::TERRAIN)
    spriteData = LBX::scanGfx(lbx.info.header, lbx.info.offsets[info.index()], in);
  else
    spriteData = LBX::scanTerrainGfx(lbx.info.offsets[info.index()], terrainData[info.index()].animated() ? 4 : 1, in);
  
  gfxAllocated(spriteData);
  LOGD3("[lbx] lbx cache size: %zu sprites in %zu bytes", spritesUsed, bytesUsed);
  
  lbx.sprites[info.index()] = spriteData;
  
  fclose(in);
  
  return spriteData;
}

const LBXArrayData* Repository::loadLBXArrayData(const LBXFile& lbx, size_t index)
{
  Path name = getLBXPath(lbx.fileName);
  FILE *in = fopen(name.c_str(), "rb");
  
  LBXArrayData* arrayData = LBX::loadArray(lbx.info.offsets[index], in);
  lbx.arrays[index] = arrayData;
  
  fclose(in);
  
  return arrayData;
}

const LBXSpriteData* Repository::loadLBXSpriteTerrainData(SpriteInfo info)
{
  LBXFile& lbx = file(info.lbx());
  file_handle in = getDescriptor(lbx);
  
  LOGD("[lbx] loading gfx entry %u from %s", info.index(), file(info.lbx()).fileName.c_str());
  LBXSpriteData* sprite = LBX::scanTerrainGfx(lbx.info.offsets[info.index()], terrainData[info.index()].animated() ? 4 : 1, in.fd());
  lbx.sprites[info.index()] = sprite;
  gfxAllocated(sprite);
  
  in.close();
  
  return sprite;
}

LBXArrayData* Repository::loadLBXSpriteTerrainMappingData(LBXFile& lbx, size_t i, FILE* in)
{
  LOGD("[lbx] loading terrain mapping %zu from %s", i, lbx.fileName.c_str());
  
  LBXArrayData* array = new LBXArrayData(1, 512);
  
  fseek(in, lbx.info.offsets[i] , SEEK_SET);
  
  u16 v = 0;
  u16 c = 0;
  
  while (c < 255)
  {
    fread(&v, sizeof(u16), 1, in);
    
    /* since shores are stored as a bitmask which starts from north east while implementation assumes that first direction is north
     we need to adjust values
     */
    u16 ai = ((c+1) >> 1) | (((c+1) & 0x01) ? 0x80 : 0x00);
    
    array->set(ai, v);
    ++c;
  }
  
  return array;
}

const LBXFile& Repository::loadLBXFontsAndPalettes()
{
  LBXFile& lbx = file(LBXID::FONTS);
  file_handle in = getDescriptor(lbx);
  
  LBX::loadHeader(lbx.info, in);
  LBX::loadFonts(lbx.info.header, lbx.info.offsets, in.fd());
  LBX::loadPalettes(lbx.info.header, lbx.info.offsets, in.fd());
  
  /*string_list names;
   LBX::scanFileNames(lbx.info, names, in);
   for (size_t i = 0; i < lbx.size(); ++i)
   {
   size_t length = lbx.info.offsets[i+1] - lbx.info.offsets[i];
   fseek(in, lbx.info.offsets[i], SEEK_SET);
   
   std::string outName = string(names[i].folder) + "-" + names[i].name;
   std::replace(outName.begin(), outName.end(), '/', '-');
   
   FILE* out = fopen(outName.c_str(), "wb");
   
   byte* buffer = new byte[length];
   fread(buffer, 1, length, in);
   fwrite(buffer, 1, length, out);
   fclose(out);
   }*/
  
  in.close();
  
  return lbx;
}

const LBXFile& Repository::loadLBXTerrain()
{
  LBXFile& lbx = file(LBXID::TERRAIN);
  
  file_handle in = getDescriptor(lbx);
  in.read(lbx.info.header);
  
  terrainData = LBX::scanTerrainTileInfo(in.fd());
  
  in.close();
  
  lbx.info.header.type = LBXFileType::TILES;
  lbx.info.header.count = terrainData.size();
  
  lbx.info.offsets.reserve(terrainData.size());
  for (u32 i = 0; i < terrainData.size(); ++i)
    lbx.info.offsets.push_back(TERRAIN_BASE + TERRAIN_ENTRY_LENGTH*terrainData[i].index());
  
  LOGD("[lbx] caching %zu assets %s", lbx.info.offsets.size(), lbx.fileName.c_str()/*, LBX::getLBXPath(lbx.fileName).c_str()*/);
  
  lbx.sprites = new LBXSpriteData*[lbx.size()];
  std::fill(lbx.sprites, lbx.sprites+lbx.size(), nullptr);
  
  return lbx;
}

const LBXFile& Repository::loadLBXTerrainMap()
{
  LBXFile& lbx = file(LBXID::TERRTYPE);
  
  constexpr size_t offset[] = { 548+2, 1062 };
  
  file_handle in = getDescriptor(lbx);
  
  in.read(lbx.info.header);
  lbx.info.header.count = 2;
  lbx.info.header.type = LBXFileType::TILES_MAPPING;
  
  lbx.info.offsets.push_back(offset[0]);
  lbx.info.offsets.push_back(offset[1]);
  
  
  lbx.arrays = new LBXArrayData*[2];
  lbx.arrays[0] = loadLBXSpriteTerrainMappingData(lbx, 0, in.fd());
  lbx.arrays[1] = loadLBXSpriteTerrainMappingData(lbx, 1, in.fd());
  
  in.close();
  
  return lbx;
}

const LBXFile& Repository::loadLBXHelp()
{
  static_assert(sizeof(LBXHelpEntry) == 1048, "");
  constexpr size_t offset = 37069 + 4;
  constexpr size_t count = 806;
  
  LBXFile& lbx = file(LBXID::HELP);
  
  file_handle in = getDescriptor(lbx);
  
  in.read(lbx.info.header);
  lbx.info.offsets.resize(lbx.size());
  in.read(&lbx.info.offsets[0], sizeof(LBXOffset), 3);
  
  lbx.info.header.count = 2;
  lbx.info.offsets.pop_back();
  lbx.info.header.type = LBXFileType::GRAPHICS;
  lbx.sprites = new LBXSpriteData*[lbx.size()];
  std::fill(lbx.sprites, lbx.sprites+lbx.size(), nullptr);
  
  in.seek(offset, SEEK_SET);
  
  size_t i = 0;
  
  auto& dest = help::Data::data;
  
  dest.resize(count);
  
  for (i = 0; i < 806; ++i)
  {
    size_t off = in.tell();
    
    LBXHelpEntry entry;
    in.read(entry);
    
    for (size_t j = 0; entry.lbxName[j] != '\0'; ++j)
      if (entry.lbxName[j] == '.')
      {
        entry.lbxName[j] = '\0';
        break;
      }
      else entry.lbxName[j] = std::tolower(entry.lbxName[j]);
    
    for (size_t j = 0; entry.text[j] != '\0'; ++j)
      if (entry.text[j] == 0x14)
        entry.text[j] = '\n';
    
    auto& current = dest[i];
    
    if (!entry.hasGfx())
    {
      current = help::Paragraph(entry.title, entry.text);
    }
    else
    {
      current = help::Paragraph(entry.title, entry.text, SpriteInfo(file(entry.lbxName).ident, entry.lbxIndex));
    }
    
    if (entry.type == -1)
      current.next = &dest[i+1];
    else if (entry.type != 0)
      current.next = &dest[entry.type];
    else
      current.next = nullptr;
    
    //printf("%3zu %s %c\n%s\n\n", i, current.title.c_str(), current.next ? 'y' : 'n', current.text.c_str());
    
    
    /*printf("%3zu %08zu: %s", i, off, entry.title);
     
     if (entry.hasGfx())
     printf(" %s(%u)", entry.lbxName, entry.lbxIndex);
     
     printf("   %d", entry.type);
     printf(" %s", entry.text);
     
     printf("\n");*/
  }
  
  
  std::unordered_map<const help::Paragraph*, const help::Paragraph*> depMap;
  for (const auto& p : help::Data::data)
    if (p.next)
      depMap[p.next] = &p;
  
  size_t counter = 0;
  for (const auto& p : help::Data::data)
    if (depMap.find(&p) == depMap.end())
    {
      help::Data::lbxHelpMapping[p.title] = &p;
      ++counter;
    }
  
  LOGD("[lbx] read %zu help entries to %zu elements", i, counter);

  
  /*auto out = file_handle("help.txt", file_mode::WRITING);
  
  std::unordered_map<const help::Paragraph*, const help::Paragraph*> depMap;
  for (const auto& p : help::Data::data)
    if (p.next)
      depMap[p.next] = &p;
  
  size_t idx = 0;
  for (const auto& p : help::Data::data)
  {
    if (depMap.find(&p) == depMap.end())
    {
      const help::Paragraph* cp = &p;
      
      while (cp)
      {
        std::string f = fmt::format("{}: {}\n{}\n\n", idx, cp->title, cp->text);
        out.write(f.c_str(), 1, f.length());
        cp = cp->next;
      }
      
      out.write("\n\n\n", 1, 1);
    }
    ++idx;
  }
  
  out.close();*/
  
  
  
  in.close();
  
  return lbx;
}

void Repository::gfxAllocated(const LBXSpriteData* data)
{
  bytesUsed += data->memoryUsedInBytes();
  ++spritesUsed;
}

void Repository::gfxDeallocated(const LBXSpriteData* data)
{
  bytesUsed -= data->memoryUsedInBytes();
  --spritesUsed;
}

void Repository::init()
{
  bytesUsed = 0;
  spritesUsed = 0;
  
  static const char* names[] = {
    "armylist",
    "backgrnd",
    "book",
    "builddat",
    "buildesc",
    "chriver",
    "cityname",
    "cityscap",
    "citywall",
    "cmbdesrc",
    "cmbdesrt",
    "cmbgrasc",
    "cmbgrass",
    "cmbmagic",
    "cmbmounc",
    "cmbmount",
    "cmbtcity",
    "cmbtfx",
    "cmbtsnd",
    "cmbtundc",
    "cmbtundr",
    "cmbtwall",
    "combat",
    "compix",
    "conquest",
    "desc",
    "desert",
    "diplomac",
    "diplomsg",
    "eventmsg",
    "events",
    "figures1",
    "figures2",
    "figures3",
    "figures4",
    "figures5",
    "figures6",
    "figures7",
    "figures8",
    "figures9",
    "figure10",
    "figure11",
    "figure12",
    "figure13",
    "figure14",
    "figure15",
    "figure16",
    "fonts",
    "halofam",
    "help",
    "herodata",
    "hire",
    "hlpentry",
    "intro",
    "introsfx",
    "introsnd",
    "itemdata",
    "itemisc",
    "itempow",
    "items",
    "lilwiz",
    "listdat",
    "load",
    "lose",
    "magic",
    "main",
    "mainscrn",
    "mapback",
    "message",
    "monster",
    "moodwiz",
    "music",
    "names",
    "newgame",
    "newsound",
    "portrait",
    "reload",
    "resource",
    "scroll",
    "snddrv",
    "soundfx",
    "specfx",
    "special",
    "special2",
    "spelldat",
    "spellose",
    "spells",
    "spellscr",
    "splmastr",
    "terrain",
    "terrstat",
    "terrtype",
    "tundra",
    "units1",
    "units2",
    "unitview",
    "vortex",
    "wallrise",
    "win",
    "wizards",
    "wizlab"
  };
  
  for (int i = 0; i < LBX_COUNT; ++i)
    data[i] = LBXFile(static_cast<LBXID>(i), names[i]);
  
  loadLBXHelp();
}



void Repository::loadBuildDesc()
{
  
  {
    FileInfo info;
    file_handle in = file_handle(getLBXPath("buildesc"), file_mode::READING);
    LBX::loadHeader(info, in);
    
    std::vector<LBX::TextFiller> inserters = {
      LBX::TextFiller(0, [](u16 index, std::string& str) {
        static const I18 buildings[] = {
          I18::BUILDING_DESC_TRADE_GOODS,
          I18::BUILDING_DESC_HOUSING,
          I18::BUILDING_DESC_BARRACKS,
          I18::BUILDING_DESC_ARMORY,
          I18::BUILDING_DESC_FIGHTERS_GUILD,
          I18::BUILDING_DESC_ARMORERS_GUILD,
          I18::BUILDING_DESC_WAR_COLLEGE,
          I18::BUILDING_DESC_SMITHY,
          I18::BUILDING_DESC_STABLE,
          I18::BUILDING_DESC_ANIMISTS_GUILD,
          I18::BUILDING_DESC_FANTASTIC_STABLE,
          I18::BUILDING_DESC_SHIP_WRIGHTS_GUILD,
          I18::BUILDING_DESC_SHIP_YARD,
          I18::BUILDING_DESC_MARITIME_GUILD,
          I18::BUILDING_DESC_SAWMILL,
          I18::BUILDING_DESC_LIBRARY,
          I18::BUILDING_DESC_SAGES_GUILD,
          I18::BUILDING_DESC_ORACLE,
          I18::BUILDING_DESC_ALCHEMISTS_GUILD,
          I18::BUILDING_DESC_UNIVERSITY,
          I18::BUILDING_DESC_WIZARDS_GUILD,
          I18::BUILDING_DESC_SHRINE,
          I18::BUILDING_DESC_TEMPLE,
          I18::BUILDING_DESC_PARTHENON,
          I18::BUILDING_DESC_CATHEDRAL,
          I18::BUILDING_DESC_MARKETPLACE,
          I18::BUILDING_DESC_BANK,
          I18::BUILDING_DESC_MERCHANTS_GUILD,
          I18::BUILDING_DESC_GRANARY,
          I18::BUILDING_DESC_FARMERS_MARKET,
          I18::BUILDING_DESC_FORESTERS_GUILD,
          I18::BUILDING_DESC_BUILDERS_HALL,
          I18::BUILDING_DESC_MECHANICIANS_GUILD,
          I18::BUILDING_DESC_MINERS_GUILD,
          I18::BUILDING_DESC_CITY_WALLS
        };
        
        if (index > 0)
          i18n::getData()[buildings[index-1]] = str;
      })
    };
    
    LBX::loadArrayFile(info, inserters, in);
    
  }
}

Path Repository::getLBXPath(const std::string& name)
{
  const auto p = Platform::instance();
  
  return p->findFile(p->getResourcePath() + "/data/lbx", name + ".lbx");
}

file_handle Repository::getDescriptor(const LBXFile& lbx)
{
  Path name = getLBXPath(lbx.fileName);
  return file_handle(name, file_mode::READING);
}
