#include "LBX.h"

#include "format.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Table.H>
#include <FL/FL_Check_Button.h>
#include <FL/fl_draw.H>

#include <string>
#include <functional>
#include <unordered_map>
#include <thread>
#include <array>

#include "Gfx.h"

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

std::unordered_map<LBXID, u32, enum_hash> usedSpritesCount;
std::unordered_set<u32> usedSprites;
bool isUsed(LBXID id, u32 index) { return usedSprites.find((static_cast<u32>(id) << 16) | index) != usedSprites.end(); }
bool isTotallyUsed(LBXID id) { return usedSpritesCount[id] == Repository::holderForID(id).size(); }
void toggleUsed(LBXID id, u32 index, bool value)
{
  auto key = (static_cast<u32>(id) << 16) | index;
  if (value)
  {
    bool wasPresent = usedSprites.find(key) != usedSprites.end();
    
    if (!wasPresent)
    {
      usedSprites.insert(key);
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
      
      Color pixel = !palette ? Gfx::PALETTE[index] : palette->get(index);
      u8 r = GET_RED(pixel), g = GET_GREEN(pixel), b = GET_BLUE(pixel);
      
      for (size_t ix = 0; ix < S; ++ix)
        for (size_t iy = 0; iy < S; ++iy)
        {
          u8* base = &tdata[(x*S+ix)*3 + (y*S+iy)*cw*3*S];
          base[0] = r;
          base[1] = g;
          base[2] = b;
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

class MyWindow : public Fl_Double_Window
{
private:
  const LBXSpriteData* sprite;
  const LBXArrayData* array;
  u8 mask = 0;

public:
  MyWindow() : Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "LBX Explorer 0.6"), sprite(nullptr), array(nullptr)
  {
    Fl::add_timeout(0.10, Timer_CB, (void*)this);

  }

  void setData(const LBXArrayData* array) { this->array = array; this->sprite = nullptr; }
  void setData(const LBXSpriteData* sprite) { this->sprite = sprite; this->array = nullptr; }
  
  void draw() override
  {
    Fl_Double_Window::draw();
    
    //fl_color(FL_BLACK);
    
    if (currentLBX && currentLBX->info.header.type == LBXFileType::TILES_MAPPING)
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
        int sx = FIRST_TABLE_WIDTH+2+SECOND_TABLE_WIDTH+2+50;
        int sy = 50;
        
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
              Color pixel = p == 0 ? Gfx::PALETTE[i] : sprite->palette->get(i);
              u8 r = GET_RED(pixel), g = GET_GREEN(pixel), b = GET_BLUE(pixel);
              
              for (size_t x = 0; x < PW; ++x)
              {
                for (size_t y = 0; y < 3; ++y)
                {
                  base[0+x*3+y*(PW*3)] = r;
                  base[1+x*3+y*(PW*3)] = g;
                  base[2+x*3+y*(PW*3)] = b;
                }
              }
            }
            
            fl_draw_image(palette, FIRST_TABLE_WIDTH+2+SECOND_TABLE_WIDTH+2+16*p, 0, PW, 768);
            
            delete[] palette;
          }
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

class SpriteTable : public Fl_Table
{
private:
  int selection;
  
public:
  SpriteTable() : Fl_Table(FIRST_TABLE_WIDTH,0,SECOND_TABLE_WIDTH,WINDOW_HEIGHT), selection(-1), rowHeight(ROW_HEIGHT)
  {
    rows(0);             // how many rows
    row_height_all(rowHeight);         // default height of rows
    row_resize(0);              // disable row resizing
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
        toggleUsed(currentLBX->ident, callback_row(), Fl::event_button() == FL_LEFT_MOUSE);
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
          if (isUsed(currentLBX->ident, ROW))
            bgcol = fl_rgb_color(200, 255, 200);
          else
            bgcol = FL_WHITE;
        }
        
        if (COL == 3 && type == LBXFileType::TILES)
        {
          const auto& terrainInfo = lbx::Repository::terrainInfo()[ROW];
          const Color c = Gfx::PALETTE[terrainInfo.minimapColor];
          bgcol = fl_rgb_color(GET_RED(c), GET_GREEN(c), GET_BLUE(c));
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
            fl_draw(fmt::sprintf("%s/%s",entry.folder, entry.name).c_str(), X, Y, W, H, FL_ALIGN_LEFT);
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
            
            if (sprite && COL == 2)
              fl_draw(fmt::sprintf("%ux%u (%u)", sprite->width, sprite->height, sprite->count).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
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
        LBX::scanFileNames(entry.info, assetNames[entry.ident], LBX::getDescriptor(entry));
        
      currentLBX = &holder(selection);
      
      if (currentLBX->info.header.type == LBXFileType::TILES || currentLBX->info.header.type == LBXFileType::GRAPHICS)
        tableSprites->row_height_all(20*2+2);
      else
        tableSprites->row_height_all(ROW_HEIGHT);

      
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
          fl_draw(to_string(lbx.info.header.type).c_str(), X, Y, W, H, FL_ALIGN_CENTER);
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
  FILE* in = fopen("status.dat", "rb");
  if (in)
  {
    fseek(in, 0, SEEK_END);
    long amount = ftell(in) / sizeof(decltype(usedSprites)::value_type);
    fseek(in, 0, SEEK_SET);
    u32* data = new u32[amount];
    fread(data, sizeof(decltype(usedSprites)::value_type), amount, in);
    for (size_t i = 0; i < amount; ++i)
    {
      usedSprites.insert(data[i]);
      ++usedSpritesCount[static_cast<LBXID>(data[i] >> 16)];
    }
    delete [] data;
  }
  fclose(in);
}

void saveStatus()
{
  FILE* in = fopen("status.dat", "wb");
  if (in)
  {
    fseek(in, 0, SEEK_END);
    long amount = usedSprites.size();
    u32* data = new u32[amount];
    auto it = usedSprites.begin();
    for (size_t i = 0; i < amount; ++i)
    {
      data[i] = *it;
      ++it;
    }
    fwrite(data, sizeof(decltype(usedSprites)::value_type), amount, in);
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
