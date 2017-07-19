#include "LBXRepository.h"

#include "format.h"
#include "Platform.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_draw.H>

#include <string>
#include <functional>
#include <unordered_map>
#include <map>
#include <thread>
#include <array>

#include "Gfx.h"
#include "Font.h"

#define MAX_ROWS 100
#define MAX_COLS 100

using namespace std;
using namespace lbx;

constexpr int FIRST_TABLE_WIDTH = 200;
constexpr int NUMERIC_COLUMN_WIDTH = 40;
constexpr int SECOND_TABLE_WIDTH = 340;
constexpr int WINDOW_HEIGHT = 800;
constexpr int WINDOW_WIDTH = 1024;
constexpr int ROW_HEIGHT = 12;

static const int DIRECTION_DX = 70;
static const int DIRECTION_MULTIPLIERS[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
static const int DIRECTION_BASE[2] = { FIRST_TABLE_WIDTH+SECOND_TABLE_WIDTH+100, 100 };

class MyWindow;
class LBXTable;
class MyCheckbox;
class SpriteTable;
class PreviewWidget;
class MyCombobox;

MyWindow *mywindow = nullptr;
LBXTable *tableLbx;
MyCheckbox *animatedCheckbox;
MyCheckbox* defaultPaletteCheckbox;
MyCheckbox* doubleScaleCheckbox;
MyCheckbox* myrranCheckbox;
MyCheckbox* directionBoxes[8];
MyCombobox* terrainTypeCB;

Fl_Table *tableSprites;

struct LBXTerrainInfo
{
  const char* group;
  int offset;
  int arrayIndex;
};

static const std::array<LBXTerrainInfo, 5> terrainInfo = {{
  { "shore-0", 0, 0 },
  { "desert-0", -2 + 0x124, 0 },
  { "tundra-0", -2 + 0x25A, 0 },
  { "mountains-1", 0, 1 },
  { "hills-1", -0x103 + 0x113, 1 }
}};

const char* terrainTypeForIndex(u16 index)
{
  if (index == 0x00)
    return "ocean";
  else if (index == 0x01)
    return "grasslands-buggy";
  else if (index <= 0xA1)
    return "shore-join";
  else if (index == 0xA2)
    return "grasslands";
  else if (index == 0xA3)
    return "forest";
  else if (index == 0xA4)
    return "mountain";
  else if (index == 0xA5)
    return "desert";
  else if (index == 0xA6)
    return "swamp";
  else if (index == 0xA7)
    return "tundra";
  else if (index == 0xA8)
    return "node-sorcery";
  else if (index == 0xA9)
    return "node-nature";
  else if (index == 0xAA)
    return "node-chaos";
  else if (index == 0xAB)
    return "hills";
  else if (index == 0xAC || index == 0xAD)
    return "grasslands";
  else if (index <= 0xB0)
    return "desert";
  else if (index <= 0xB2)
    return "swamp";
  else if (index == 0xB3)
    return "volcano";
  else if (index == 0xB4)
    return "grasslands";
  else if (index <= 0xB6)
    return "tundra";
  else if (index <= 0xB8)
    return "forest";
  else if (index <= 0xC4)
    return "river-shore";
  else if (index <= 0xC8)
    return "shore-lake";
  else if (index <= 0xE8)
    return "shore-ocean";
  else if (index <= 0x102)
    return "river-shore";
  else if (index <= 0x112)
    return "mountains-join";
  else if (index <= 0x123)
    return "hills-join";
  else if (index <= 0x1C3)
    return "desert-join";
  else if (index <= 0x258)
    return "shore";
  else if (index <= 0x259)
    return "ocean";
  else if (index <= 0x261)
    return "tundra-buggy";
  else if (index <= 0x2f9)
    return "tundra-join";
  else
    assert(false);
}

enum class state : u32
{
  none = 0, used = 1, ignored = 2
};

std::unordered_map<LBXID, u32, enum_hash> usedSpritesCount;
std::map<u32, state> usedSprites;
state isUsed(LBXID id, u32 index) { auto it = usedSprites.find((static_cast<u32>(id) << 16) | index); return it != usedSprites.end() ? it->second : state::none; }
bool isTotallyUsed(LBXID id) { return usedSpritesCount[id] == Repository::holderForID(id).size(); }

void toggleUsed(LBXID id, u32 index, bool value, state s = state::used)
{
  auto key = (static_cast<u32>(id) << 16) | index;
  if (value)
  {
    bool wasPresent = usedSprites.find(key) != usedSprites.end();
    
    if (!wasPresent)
    {
      usedSprites[key] = s;
      ++usedSpritesCount[id];
    }
  }
  else
  {
    bool wasPresent = usedSprites.find(key) != usedSprites.end();
    if (wasPresent)
    {
      usedSprites.erase(key);
      --usedSpritesCount[id];
    }
  }
}

void switchUsed(LBXID id, u32 index, state s = state::used)
{
  auto key = (static_cast<u32>(id) << 16) | index;
  bool wasPresent = usedSprites.find(key) != usedSprites.end();
  toggleUsed(id, index, !wasPresent, s);
}

size_t ticks = 0;

namespace std
{
  template<>
  struct hash<LBXID>
  {
    size_t operator()(const LBXID& entry) const
    {
      return static_cast<size_t>(entry);
    }
  };
}

unordered_map<LBXID, vector<LBXFileName>> assetNames;

PreviewWidget* preview = nullptr;

const LBXFile* currentLBX = nullptr;


class MyCombobox : public Fl_Choice
{
private:
  
  
public:
  MyCombobox(const char* label, int x, int y, int w, int h) : Fl_Choice(x, y, w, h, label)
  {
    for (size_t i = 0; i < terrainInfo.size(); ++i)
      add(terrainInfo[i].group);
    callback(mycallback);
    value(0);
  }
  
  int handle(int event) override
  {
    return Fl_Choice::handle(event);
  }
  
  void mycallback()
  {
    this->index = this->value();
  }
  
  static void mycallback(Fl_Widget* w, void* data)
  {
    static_cast<MyCombobox*>(w)->mycallback();
  }
  
  int index = 0;
};

class MyCheckbox : public Fl_Check_Button
{

public:
  MyCheckbox(const char* label, int x, int y, int w = 100, int h = 20) : Fl_Check_Button(x, y, w, h, label) { }
  
  int handle(int event) override
  {
    if (event == FL_PUSH)
    {
      isToggled = !isToggled;
    }
    
    return Fl_Check_Button::handle(event);
  }
  
  bool isToggled = false;
};

void drawSprite(const u8* data, int w, int h, int& sx, int& sy, int S, const Palette* palette, bool autoClip = true, int cw = std::numeric_limits<int>::max(), int ch = std::numeric_limits<int>::max())
{
  cw = std::min(w, cw);
  ch = std::min(h, ch);
  
  u8* tdata = new u8[cw*ch*3 * S*S];
  
  for (size_t x = 0; x < cw; ++x)
  {
    for (size_t y = 0; y < ch; ++y)
    {
      u8 index = data[x + y*w];
      
      Color pixel = !palette ? Gfx::mainPalette->get(index) : palette->get(index);
      
      for (size_t ix = 0; ix < S; ++ix)
        for (size_t iy = 0; iy < S; ++iy)
        {
          u8* base = &tdata[(x*S+ix)*3 + (y*S+iy)*cw*3*S];
          base[0] = pixel.r;
          base[1] = pixel.g;
          base[2] = pixel.b;
        }
    }
  }
  
  if (sy+h*S >= WINDOW_HEIGHT)
  {
    if (autoClip)
    {
      sy = 50;
      sx += w*S+2;
    }
    else
    {
      delete [] tdata;
      return;
    }
  }
  
  fl_draw_image(tdata, sx, sy, cw*S, ch*S);
  
  sy += h*S+2;
  
  delete [] tdata;
}

#pragma mark Main Window
class MyWindow : public Fl_Double_Window
{
private:
  const LBXSpriteData* sprite;
  const LBXArrayData* array;
  u8 mask = 0;
  
  int sx = FIRST_TABLE_WIDTH+2+SECOND_TABLE_WIDTH+2+50;
  int sy = 50;
  
  int hx = -1;
  int hy = -1;
  u8 hi = 0;

public:
  MyWindow() : Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "LBX Explorer 0.6"), sprite(nullptr), array(nullptr)
  {
    Fl::add_timeout(0.10, Timer_CB, (void*)this);

  }
  
  int handle(int event) override
  {
    if (event == FL_ENTER)
      return 1;
    else if (event == FL_MOVE)
    {
      hx = -1;
      hy = -1;
      
      int x = Fl::event_x() - sx;
      int y = Fl::event_y() - sy;
      
      if (x >= 0 && y >= 0)
      {
        
        if (sprite)
        {
          int S = doubleScaleCheckbox->isToggled ? 4 : 2;

          bool isAnimated = animatedCheckbox->isToggled;
          const size_t spriteIndex = isAnimated ? ((ticks/5)%sprite->count) : 0;
          
          if (x < S*sprite->width && y < S*sprite->height)
          {
            x /= S;
            y /= S;
            
            hx = x;
            hy = y;
            hi = sprite->data[spriteIndex][x + y*sprite->width];
          }
        }
      }
    }
    
    return Fl_Double_Window::handle(event);
  }

  void setData(const LBXArrayData* array) { this->array = array; this->sprite = nullptr; }
  void setData(const LBXSpriteData* sprite) { this->sprite = sprite; this->array = nullptr; }
  
  int drawFont(const char* name, const FontData* font, int BASEY)
  {
    const int BASEX = FIRST_TABLE_WIDTH + SECOND_TABLE_WIDTH + 10;
    const int S = doubleScaleCheckbox->isToggled ? 4 : 2;

    fl_draw(name, BASEX, BASEY);
    
    const int SPACING = 1;
    const int GLYPHS_PER_LINE = (WINDOW_WIDTH - BASEX) / ((font->w()+SPACING)*S);
    int ry = BASEY;
    
    u8 maxColor = 0;
    u8* fdata = font->dataAt(0,0);
    for (int xx = 0; xx < font->tw()*font->h(); ++xx)
      maxColor = std::max(maxColor, fdata[xx]);
    
    const Palette* palette;
    
    if (defaultPaletteCheckbox->isToggled)
    {
      color_array colors;
      colors.push_back({0,0,0});
      colors.push_back({255,0,0});
      colors.push_back({0,255,0});
      colors.push_back({0,0,0});
      colors.push_back({0,0,255});
      colors.push_back({255,255,0});
      colors.push_back({0,255,255});
      colors.push_back({255,255,255});
      colors.push_back({255,128,0});
      colors.push_back({0,255,128});
      colors.push_back({255,0,128});
      palette = new IndexedPalette(colors);
    }
    else
    {
      color_array colors;
      for (int i = 0; i < maxColor+1; ++i)
      {
        u8 c = i == 0 ? 0 : ((196 / (maxColor+1)) * i + (256-196));
        colors.push_back({ c, c, c });
      }
      
      palette = new IndexedPalette(colors);
    }
    

    
    u8* buffer = new u8[font->w()*font->h()];
    
    for (int i = 0; i < GLYPH_COUNT; ++i)
    {
      u8* data = font->dataAt(0, i);
      
      for (size_t xx = 0; xx < font->w(); ++xx)
        for (size_t yy = 0; yy < font->h(); ++yy)
          buffer[xx + yy*font->w()] = data[xx + yy*font->tw()];
      
      int sx = BASEX + (i % GLYPHS_PER_LINE)*(font->w()+1)*S;
      int sy = BASEY + 8 + (i / GLYPHS_PER_LINE)*(font->h()+1)*S;
      drawSprite(buffer, font->w(), font->h(), sx, sy, S, palette);
      ry = sy;
    }
    
    delete [] buffer;
    delete palette;
    
    return ry;
  }
  
  void draw() override
  {
    Fl_Double_Window::draw();
    
    //fl_color(FL_BLACK);
    
    if (currentLBX && currentLBX->info.header.type == LBXFileType::FONTS)
    {
      const int DX = 15;
      int ry = drawFont("Tiny", FontData::fonts[FONT_TINY], 12);
      ry = drawFont("Small", FontData::fonts[FONT_SMALL], ry + DX);
      ry = drawFont("Medium", FontData::fonts[FONT_MEDIUM], ry + DX);
      ry = drawFont("Medium Thick", FontData::fonts[FONT_MEDIUM_THICK], ry + DX);
      ry = drawFont("Serif", FontData::fonts[FONT_SERIF], ry + DX);


    }
    else if (currentLBX && currentLBX->info.header.type == LBXFileType::TILES_MAPPING)
    {
      u8 mask = 0;
      
      for (int i = 0; i < 8; ++i)
      {
        if (directionBoxes[i]->isToggled)
          mask |= (1<<i);
      }
      
      if (mask != 0)
      {
        // shore, desert, hills
        int selectedBatch = terrainTypeCB->index;

        
        u16 index = currentLBX->arrays[terrainInfo[selectedBatch].arrayIndex]->get<u16>(mask);
        
        fl_draw(fmt::sprintf("Mask: %u", mask).c_str(), FIRST_TABLE_WIDTH+SECOND_TABLE_WIDTH+250, DIRECTION_BASE[1] - DIRECTION_DX);
        fl_draw(fmt::sprintf("Tile Index: %03X", index).c_str(), FIRST_TABLE_WIDTH+SECOND_TABLE_WIDTH+250, DIRECTION_BASE[1] - DIRECTION_DX + 20);

        index = index + terrainInfo[selectedBatch].offset;
        
        fl_draw(fmt::sprintf("Computed Index: %03X", index).c_str(), FIRST_TABLE_WIDTH+SECOND_TABLE_WIDTH+250, DIRECTION_BASE[1] - DIRECTION_DX + 40);

        if (myrranCheckbox->isToggled)
          index += 0x2FA;
        
        const LBXFile& file = Repository::holderForID(LBXID::TERRAIN);
        
        if (!file.sprites[index])
          Repository::loadLBXSpriteTerrainData(LSI(TERRAIN, index));

        const int S = 4;
        int bx = DIRECTION_BASE[0] - (20*S)/2 + 10;
        int by = DIRECTION_BASE[1] - (18*S)/2 + 7;
        drawSprite(file.sprites[index]->data[0], 20, 18, bx, by, S, nullptr);
      }
      
      
    }
    else
    {
    
      bool isAnimated = animatedCheckbox->isToggled;

      if (sprite)
      {
        int sx = this->sx;
        int sy = this->sy;
        
        for (int i = 0; i < sprite->count && (!isAnimated || i == 0); ++i)
        {
          // draw sprite
          
          int S = doubleScaleCheckbox->isToggled ? 4 : 2;
          const size_t spriteIndex = isAnimated ? ((ticks/5)%sprite->count) : i;

          drawSprite(sprite->data[spriteIndex], sprite->width, sprite->height, sx, sy, S, defaultPaletteCheckbox->isToggled ? nullptr : sprite->palette);

          // draw palette
          
          const int PW = 10;
          
          for (int p = 0; p < 2; ++p)
          {
            u8 *palette = new u8[3*768*PW];
            
            for (size_t i = 0; i < 256; ++i)
            {
              u8* base = palette+(PW*3*3)*i;
              Color pixel = p == 0 ? Gfx::mainPalette->get(i) : sprite->palette->get(i);
              
              for (size_t x = 0; x < PW; ++x)
              {
                for (size_t y = 0; y < 3; ++y)
                {
                  base[0+x*3+y*(PW*3)] = pixel.r;
                  base[1+x*3+y*(PW*3)] = pixel.g;
                  base[2+x*3+y*(PW*3)] = pixel.b;
                }
              }
            }
            
            fl_draw_image(palette, FIRST_TABLE_WIDTH+2+SECOND_TABLE_WIDTH+2+16*p, 5, PW, 768);
            
            delete[] palette;
          }
        }
        
        if (hx != -1)
        {
          Color pixel = !sprite->palette ? Gfx::mainPalette->get(hi) : sprite->palette->get(hi);

          fl_draw(fmt::sprintf("(%d,%d) = %d = RGB(%d,%d,%d) = #%02X%02X%02X", hx, hy, hi, pixel.r, pixel.g, pixel.b, pixel.r, pixel.b, pixel.g).c_str(), this->sx, 30);
          fl_draw("<", FIRST_TABLE_WIDTH+2+SECOND_TABLE_WIDTH+2+16*2, hi*3+10);
        }
      }
      else if (array != nullptr)
      {
        fl_font(FL_HELVETICA, 10);
        
        int y = 12, x = FIRST_TABLE_WIDTH + SECOND_TABLE_WIDTH + 5;
        double maxw = 0.0;
        for (int i = 0; i < array->count; ++i)
        {
          string str = to_string(i) + ": ";
          str.reserve(array->size);
          
          for (size_t kk = 0; kk < array->size; ++kk)
          {
            u8 u = array->data[i][kk];
            if (u == 0x00)
              continue;
            else if (u == '\n')
              str += "<\\n>";
            else if (u <= 128)
              str += (char)u;
            else
              str += fmt::sprintf("<%02X>", u);
          }
          
          maxw = std::max(maxw, fl_width(str.c_str()));
          
          fl_draw(str.c_str(), x, y);

          if (y > WINDOW_HEIGHT - 40)
          {
            x += maxw + 10;
            y = 12;
            
            if (x > WINDOW_WIDTH)
              break;
          }
          else
            y += 12;
        }
      }
    }
  }
  
  static void Timer_CB(void *userdata) {
    MyWindow *o = (MyWindow*)userdata;
    o->redraw();
    Fl::repeat_timeout(0.10, Timer_CB, userdata);
    ++ticks;
  }
};

#pragma mark Entries Table
class SpriteTable : public Fl_Table
{
private:
  int selection;
  
public:
  SpriteTable() : Fl_Table(FIRST_TABLE_WIDTH,0,SECOND_TABLE_WIDTH,WINDOW_HEIGHT), selection(-1), rowHeight(ROW_HEIGHT)
  {
    rows(0);             // how many rows
    row_height_all(rowHeight);         // default height of rows
    //row_resize(0);              // disable row resizing
    // Cols
    cols(5);             // how many columns
    col_header(1);              // enable column headers (along top)
    col_width_all(SECOND_TABLE_WIDTH/4);          // default width of columns
    col_width(0, 30);
    col_width(1, 140);
    col_width(3, 20);
    col_width(4, 60);
    when(FL_WHEN_RELEASE);
    callback(mycallback, this);
    end();
  }
  
  void DrawHeader(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
    fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
    fl_color(FL_BLACK);
    fl_draw(s,X,Y,W,H, FL_ALIGN_CENTER);
    fl_pop_clip();
  }
  
  void mycallback()
  {
    if (callback_context() == CONTEXT_CELL)
    {
      if (callback_col() == 4)
      {
        if (Fl::event() == FL_PUSH)
        {
          switchUsed(currentLBX->ident, callback_row(), Fl::event_button() == FL_LEFT_MOUSE ? state::used : state::ignored);
        }
        return;
      }
      
      selection = callback_row();
      
      if (currentLBX->info.header.type == LBXFileType::GRAPHICS)
      {
        if (!currentLBX->sprites[selection])
          Repository::loadLBXSpriteData(SpriteInfo(currentLBX->ident, selection));
        
        mywindow->setData(currentLBX->sprites[selection]);
      }
      else if (currentLBX->info.header.type == LBXFileType::DATA_ARRAY)
      {
        if (!currentLBX->arrays[selection])
          Repository::loadLBXArrayData(*currentLBX, selection);
        
        mywindow->setData(currentLBX->arrays[selection]);
      }
      else if (currentLBX->info.header.type == LBXFileType::TILES)
      {
        if (!currentLBX->sprites[selection])
          Repository::loadLBXSpriteTerrainData(SpriteInfo(currentLBX->ident, selection));
        
        mywindow->setData(currentLBX->sprites[selection]);
      }
      else if (currentLBX->info.header.type == LBXFileType::TILES_MAPPING)
      {
        mywindow->setData(currentLBX->arrays[selection]);
      }
      
      
      
      redraw();
    }
  }
  
  static void mycallback(Fl_Widget* w, void* data)
  {
    static_cast<SpriteTable*>(data)->mycallback();
  }
  
  int rowHeight;
  
  
  void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0)
  {
    switch (context)
    {
      case CONTEXT_STARTPAGE:
        fl_font(FL_HELVETICA, 10);
        return;
      case CONTEXT_COL_HEADER:
      {
        static const char* columnNames[] = {"#", "Name", "Spec", "P", "U"};
        DrawHeader(columnNames[COL], X, Y, W, H);
        return;
      }
      case CONTEXT_ROW_HEADER:
        return;
      case CONTEXT_CELL:
      {
        int fgcol = selection == ROW ? FL_RED : FL_BLACK;
        int bgcol = FL_WHITE;
        
        LBXFileType type = currentLBX->info.header.type;
        bool isLoaded = currentLBX->isLoaded();
        
        if (!isLoaded)
          fgcol = fl_rgb_color(180, 180, 180);
        else
        {
          state s = isUsed(currentLBX->ident, ROW);
          
          if (s == state::used)
            bgcol = fl_rgb_color(200, 255, 200);
          else if (s == state::ignored)
            bgcol = fl_rgb_color(255, 255, 220);
          else
            bgcol = FL_WHITE;
        }
        
        if (COL == 3 && type == LBXFileType::TILES)
        {
          const auto& terrainInfo = lbx::Repository::terrainInfo()[ROW];
          const Color c = Gfx::mainPalette->get(terrainInfo.minimapColor);
          bgcol = fl_rgb_color(c.r,c.g,c.b);
          fgcol = FL_WHITE;
        }
        
        fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, bgcol);
        fl_color(fgcol);
        
        if (COL == 0)
        {
          if (type == LBXFileType::TILES)
            fl_draw(fmt::sprintf("%03X", ROW%0x2FA).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
          else
            fl_draw(fmt::sprintf("%u", ROW).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
        }
        else if (COL == 1)
        {
          if (type != LBXFileType::TILES)
          {
            const LBXFileName& entry = assetNames[currentLBX->ident][ROW];
            if (ROW+1 < currentLBX->info.offsets.size())
              fl_draw(fmt::sprintf("%6Xh %d bytes\n%s/%s",currentLBX->info.offsets[ROW], currentLBX->info.offsets[ROW+1]-currentLBX->info.offsets[ROW],entry.folder, entry.name).c_str(), X, Y, W, H, FL_ALIGN_LEFT);
          }
          else
          {
            const auto& terrainInfo = lbx::Repository::terrainInfo()[ROW];
            const char* plane = ROW <= 0x2F9 ? "arcanus" : "myrran";
            
            fl_draw(fmt::sprintf("%s/%s (%03X)", plane, terrainTypeForIndex(ROW%0x2FA), terrainInfo.index()).c_str(), X, Y, W, H, FL_ALIGN_LEFT);
          }
          
        }
        else if (COL == 4)
        {
          if (type == LBXFileType::TILES || type == LBXFileType::GRAPHICS)
          {
            LBXSpriteData* sprite = currentLBX->sprites[ROW];
            
            if (!sprite)
            {
              if (type == LBXFileType::TILES)
                Repository::loadLBXSpriteTerrainData(SpriteInfo(currentLBX->ident, ROW));
              else if (type == LBXFileType::GRAPHICS)
                Repository::loadLBXSpriteData(SpriteInfo(currentLBX->ident, ROW));
            }
            
            if (sprite)
            {
              int sx = X+1, sy = Y+1;
              
              const Palette* palette = (defaultPaletteCheckbox->isToggled || type == LBXFileType::TILES) ? nullptr : sprite->palette;
              
              drawSprite(sprite->data[0], sprite->width, sprite->height, sx, sy, 2, palette, false, 20, 18);
            }
          }
        }
        else
        {
          if (type == LBXFileType::GRAPHICS || type == LBXFileType::TILES)
          {
            LBXSpriteData* sprite = currentLBX->sprites[ROW];
            
            // fl_draw(fmt::sprintf("%ux%u (%u)", sprite->width, sprite->height, sprite->count).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
            
            if (sprite)
            {
              //assert((sprite->unknown[0] == 0) && (sprite->unknown[2] == 0) && (sprite->unknown[3] == 0));
              //assert(sprite->unknown[4] > 0 || sprite->count == 1);
            }

            if (sprite && COL == 2)
              fl_draw(fmt::sprintf("%ux%u (%u)\n%d %d", sprite->width, sprite->height, sprite->count, sprite->unknown2, sprite->unknown3).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
            else if (sprite && COL == 3 && sprite->hasCustomPalette && type == LBXFileType::GRAPHICS)
              fl_draw("Y", X,Y,W,H, FL_ALIGN_CENTER);
            else if (COL == 3 && type == LBXFileType::TILES)
            {
              const auto& terrainInfo = lbx::Repository::terrainInfo()[ROW];
              fl_draw(fmt::sprintf("%02X", terrainInfo.minimapColor).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
            }
          }
          else if (type == LBXFileType::DATA_ARRAY)
          {
            LBXArrayData* array = currentLBX->arrays[ROW];
            
            if (array)
            {
              if (COL == 2)
                fl_draw(fmt::sprintf("%u x %u bytes", array->count, array->size).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
            }
          }
          else if (type == LBXFileType::TILES_MAPPING)
          {
            if (COL == 2)
              fl_draw(fmt::sprintf("256 x 2 bytes").c_str(), X,Y,W,H, FL_ALIGN_CENTER);
          }
        }
        
        return;
      }
      default:
        return;
    }
  }
  
};


#pragma mark LBX Table
class LBXTable : public Fl_Table
{
private:
  int selection;
  
  const LBXFile& holder(int index) { return Repository::holderForID(static_cast<LBXID>(index)); }
  
public:
  LBXTable() : Fl_Table(0,0,FIRST_TABLE_WIDTH,WINDOW_HEIGHT), selection(-1)
  {
    rows(LBX_COUNT);             // how many rows
    row_height_all(ROW_HEIGHT);         // default height of rows
    row_resize(0);              // disable row resizing
    cols(4);             // how many columns
    col_header(1);              // enable column headers (along top)
    col_width(0, 90);          // default width of columns
    col_width(1, 30);
    col_width(2, 30);
    col_width(3, 40);
    when(FL_WHEN_RELEASE);
    callback(mycallback, this);
    end();                      // end the Fl_Table group
  }
  
  void DrawHeader(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
    fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
    fl_color(FL_BLACK);
    fl_draw(s,X,Y,W,H, FL_ALIGN_CENTER);
    fl_pop_clip();
  }
  
  void mycallback()
  {
    if (callback_context() == CONTEXT_CELL)
    {
      selection = callback_row();
      
      const auto& entry = holder(selection);
      
      if (entry.sprites == nullptr)
        Repository::loadLBX(entry.ident);
      
      if (assetNames.find(entry.ident) == assetNames.end())
        LBX::scanFileNames(entry.info, assetNames[entry.ident], Repository::getDescriptor(entry).fd());
        
      currentLBX = &holder(selection);
      
      if (currentLBX->info.header.type == LBXFileType::TILES || currentLBX->info.header.type == LBXFileType::GRAPHICS)
        tableSprites->row_height_all(20*2+2);
      else
        tableSprites->row_height_all(20*2+2);

      
      if (currentLBX->info.header.type == LBXFileType::TILES_MAPPING)
      {
        for (size_t i = 0; i < 8; ++i)
          directionBoxes[i]->show();
        terrainTypeCB->show();
        myrranCheckbox->show();
        
      }
      else
      {
        for (size_t i = 0; i < 8; ++i)
          directionBoxes[i]->hide();
        terrainTypeCB->hide();
        myrranCheckbox->hide();
      }
      
      tableSprites->rows(currentLBX->size());
      tableSprites->redraw();
      
      redraw();
    }
  }
  
  static void mycallback(Fl_Widget* w, void* data)
  {
    static_cast<LBXTable*>(data)->mycallback();
  }
  
  void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0)
  {
    switch (context)
    {
      case CONTEXT_STARTPAGE:
        fl_font(FL_HELVETICA, 10);
        return;
      case CONTEXT_COL_HEADER:
      {
        const char* title = "Archive";
        if (COL == 1) title = "#";
        else if (COL == 2) title = "T";
        else if (COL == 3) title = "%";
        DrawHeader(title,X,Y,W,H);
        return;
      }
      case CONTEXT_ROW_HEADER:
        return;
      case CONTEXT_CELL:
      {
        const LBXFile& lbx = holder(ROW);
        
        int fgcol = selection == ROW ? FL_RED : FL_BLACK;
        int bgcol = isTotallyUsed(static_cast<LBXID>(ROW)) ? fl_rgb_color(200, 255, 200) : FL_WHITE;
        
        fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, bgcol);
        fl_color(fgcol);
        
        if (COL == 0)
          fl_draw((lbx.fileName).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
        else if (COL == 1)
        {
          if (lbx.sprites == nullptr)
            fl_draw("N/A", X,Y,W,H, FL_ALIGN_CENTER);
          else
            fl_draw(to_string(lbx.size()).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
        }
        else if (COL == 2)
        {
          const char* typeName = nullptr;
          switch (lbx.info.header.type)
          {
            case LBXFileType::GRAPHICS: typeName = "GFX"; break;
            case LBXFileType::DATA_ARRAY: typeName = "DATA"; break;
            case LBXFileType::TILES: typeName = "TGFX"; break;
            case LBXFileType::TILES_MAPPING: typeName = "TMAP"; break;
            case LBXFileType::SOUND: typeName = "SFX"; break;
            case LBXFileType::MUSIC: typeName = "MFX"; break;
            case LBXFileType::HELP: typeName = "HLP"; break;
            case LBXFileType::FONTS: typeName = "FNT"; break;
          }
          
          fl_draw(typeName, X, Y, W, H, FL_ALIGN_CENTER);
        }
        else if (COL == 3)
        {
          if (lbx.info.header.type == LBXFileType::GRAPHICS)
          {
            float percent = usedSpritesCount.find(lbx.ident)->second / (float)lbx.size();
            fl_draw(fmt::sprintf("%d%%", (int)(percent*100)).c_str(), X, Y, W, H, FL_ALIGN_CENTER);
          }
        }
        return;
      }
      default:
        return;
    }
  }
};



void loadStatus()
{
  std::string path = std::string(LBX_STATUS_PATH) + "/status.dat"; //Platform::instance()->getResourcePath() + "/data/status.dat";
  
  FILE* in = fopen(path.c_str(), "rb");
  if (in)
  {
    fseek(in, 0, SEEK_END);
    long amount = (ftell(in) / sizeof(u32)) / 2;
    fseek(in, 0, SEEK_SET);
    u32* data = new u32[amount*2];
    fread(data, sizeof(u32), amount*2, in);
    for (size_t i = 0; i < amount; ++i)
    {
      usedSprites[data[2*i]] = (state)data[2*i+1];
      ++usedSpritesCount[static_cast<LBXID>(data[2*i] >> 16)];
    }
    delete [] data;
  }
  fclose(in);
}

void saveStatus()
{
  std::string path = std::string(LBX_STATUS_PATH) + "/status.dat";//Platform::instance()->getResourcePath() + "/data/status.dat";
  
  FILE* in = fopen(path.c_str(), "wb");
  if (in)
  {
    fseek(in, 0, SEEK_END);
    long amount = usedSprites.size();
    u32* data = new u32[amount*2];
    auto it = usedSprites.begin();
    for (size_t i = 0; i < amount; ++i)
    {
      data[i*2] = it->first;
      data[i*2+1] = (u32)it->second;
      ++it;
    }
    fwrite(data, sizeof(u32), amount*2, in);
    delete [] data;
  }
  fclose(in);
}

int main(int argc, char **argv) {

  Repository::init();
  
  loadStatus();
  
  for (size_t i = 0; i < LBX_COUNT; ++i)
    Repository::loadLBX(static_cast<LBXID>(i));
  
  Fl::visual(FL_RGB);
    
  mywindow = new MyWindow();
  
  tableLbx = new LBXTable();
  tableSprites = new SpriteTable();
  doubleScaleCheckbox = new MyCheckbox("double scale", FIRST_TABLE_WIDTH+SECOND_TABLE_WIDTH+5, WINDOW_HEIGHT - 20);
  animatedCheckbox = new MyCheckbox("animated", FIRST_TABLE_WIDTH+SECOND_TABLE_WIDTH+5 + 100, WINDOW_HEIGHT - 20);
  defaultPaletteCheckbox = new MyCheckbox("default palette", FIRST_TABLE_WIDTH+SECOND_TABLE_WIDTH+5 + 200, WINDOW_HEIGHT - 20);
  
  static const char* dirNames[] = {"N","NW","W","SW","S","SE","E","NE"};
  
  for (int i = 0; i < 8; ++i)
  {
    directionBoxes[i] = new MyCheckbox(dirNames[i], DIRECTION_BASE[0] + DIRECTION_MULTIPLIERS[i][0]*DIRECTION_DX, DIRECTION_BASE[1] + DIRECTION_MULTIPLIERS[i][1]*DIRECTION_DX, 20, 20);
    directionBoxes[i]->hide();
  }
  
  myrranCheckbox = new MyCheckbox("Myrran", DIRECTION_BASE[0] - DIRECTION_MULTIPLIERS[5][0]*DIRECTION_DX, DIRECTION_BASE[1] + DIRECTION_MULTIPLIERS[5][1]*DIRECTION_DX + 50);
  terrainTypeCB = new MyCombobox("Type", DIRECTION_BASE[0] - DIRECTION_MULTIPLIERS[5][0]*DIRECTION_DX, DIRECTION_BASE[1] + DIRECTION_MULTIPLIERS[5][1]*DIRECTION_DX + 70, 100, 20);
  
  myrranCheckbox->hide();
  terrainTypeCB->hide();
  
  mywindow->end();
  mywindow->show(argc, argv);
  
  int result = Fl::run();
  
  saveStatus();

  return result;
}
