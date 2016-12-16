#include "LBX.h"

#include "format.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>

#include <string>
#include <functional>
#include <unordered_map>

#include "Gfx.h"

#define MAX_ROWS 100
#define MAX_COLS 100

using namespace std;
using namespace lbx;

constexpr int FIRST_TABLE_WIDTH = 250;
constexpr int SECOND_TABLE_WIDTH = 320;
constexpr int WINDOW_HEIGHT = 800;
constexpr int WINDOW_WIDTH = 1920;

class MyWindow;
class LBXTable;
class SpriteTable;
class PreviewWidget;

MyWindow *mywindow = nullptr;
LBXTable *tableLbx;
Fl_Table *tableSprites;

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

class MyWindow : public Fl_Double_Window
{
private:
  const LBXSpriteData* sprite;

public:
  MyWindow() : Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "LBX Explorer 0.1"), sprite(nullptr)
  {
    Fl::add_timeout(0.25, Timer_CB, (void*)this);

  }

  void setData(const LBXSpriteData* sprite) { this->sprite = sprite; }
  
  void draw() override
  {
    Fl_Double_Window::draw();
    
    //fl_color(FL_BLACK);

    if (sprite)
    {
      int sx = FIRST_TABLE_WIDTH+2+SECOND_TABLE_WIDTH+2+50;
      int sy = 50;
      
      for (int i = 0; i < sprite->count; ++i)
      {
        const int w = sprite->width, h = sprite->height;
        constexpr int S = 2;
        
        //printf("DRAW %d %d\n", w, h);
        
        u8* tdata = new u8[w*h*3 * S*S];
        
        for (size_t x = 0; x < w; ++x)
        {
          for (size_t y = 0; y < h; ++y)
          {
            u8 index = sprite->data[i][x + y*w];
            
            Color pixel;
            
            /*constexpr int DELTA = 256 - 32;
            if (index < DELTA)*/
            {
              pixel = sprite->palette->get(index);
            }
            /*else
            {
              int delta = 32 - (index - DELTA);
              pixel = RGB((7 + delta*2) << 2, (4 + (delta/2)) << 2, (2 + (delta/2)) << 2);
            }*/
            
            u8 r = GET_RED(pixel), g = GET_GREEN(pixel), b = GET_BLUE(pixel);
            
            tdata[x*S*3 + y*w*3*S*S] = r; tdata[x*S*3 + 1 + y*w*3*S*S] = g; tdata[x*S*3 + 2 + y*w*3*S*S] = b;
            tdata[(x*S+1)*3 + y*w*3*S*S] = r; tdata[(x*S+1)*3 + 1 + y*w*3*S*S] = g; tdata[(x*S+1)*3 + 2 + y*w*3*S*S] = b;
            
            tdata[x*S*3 + y*w*3*S*S + w*3*S] = r; tdata[x*S*3 + 1 + y*w*3*S*S + w*3*S ] = g; tdata[x*S*3 + 2 + y*w*3*S*S + w*3*S] = b;
            tdata[(x*S+1)*3 + y*w*3*S*S + w*3*S] = r; tdata[(x*S+1)*3 + 1 + y*w*3*S*S + w*3*S] = g; tdata[(x*S+1)*3 + 2 + y*w*3*S*S + w*3*S] = b;
            
            //tdata[(i*S+w*S)*3] = r; tdata[(i*S+w*S)*3 + 1] = g; tdata[(i*S+w*S)*3 + 2] = b;
          }
        }
        
        if (sy+h*S >= WINDOW_HEIGHT)
        {
          sy = 50;
          sx += w*S+2;
        }
  
        fl_draw_image(tdata, sx, sy, w*S, h*S);
        
        sy += h*S+2;
        

        
        delete [] tdata;
      }
      
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
  
  static void Timer_CB(void *userdata) {
    MyWindow *o = (MyWindow*)userdata;
    o->redraw();
    Fl::repeat_timeout(0.25, Timer_CB, userdata);
  }
};

class LBXTable : public Fl_Table
{
private:
  int selection;
  
  const LBXFile& holder(int index) { return Repository::holderForID(static_cast<LBXID>(index)); }
  
public:
  LBXTable() : Fl_Table(0,0,FIRST_TABLE_WIDTH,800), selection(-1)
  {
    rows(LBX_COUNT);             // how many rows
    row_height_all(20);         // default height of rows
    row_resize(0);              // disable row resizing
    // Cols
    cols(2);             // how many columns
    col_header(1);              // enable column headers (along top)
    col_width_all(FIRST_TABLE_WIDTH/2);          // default width of columns
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
      {
        Repository::loadLBX(entry.ident);
        LBX::scanFileNames(entry.header, entry.offsets, assetNames[entry.ident], LBX::getDescriptor(entry));
      }
      
      currentLBX = &holder(selection);
      
      
      
      tableSprites->rows(currentLBX->size());
      tableSprites->redraw();
      tableSprites->row_height_all(20);
      
      redraw();
    }
  }
  
  static void mycallback(Fl_Widget* w, void* data)
  {
    static_cast<LBXTable*>(data)->mycallback();
  }
  
  /*int handle(int event) override
  {
    if (event == FL_RELEASE)
    {
      
    }
    
    return Fl_Widget::handle(event);
  }*/
  
  void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0)
  {
    switch (context)
    {
      case CONTEXT_STARTPAGE:
        fl_font(FL_HELVETICA, 10);
        return;
      case CONTEXT_COL_HEADER:
        DrawHeader(COL == 0 ? "Archive" : "Entries",X,Y,W,H);
        return;
      case CONTEXT_ROW_HEADER:
        return;
      case CONTEXT_CELL:
      {
        const LBXFile& lbx = holder(ROW);
        
        int fgcol = selection == ROW ? FL_RED : FL_BLACK;
        int bgcol = FL_WHITE;
        fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, bgcol);
        fl_color(fgcol);
        
        if (COL == 0)
          fl_draw(lbx.fileName.c_str(), X,Y,W,H, FL_ALIGN_CENTER);
        else
        {
          if (lbx.sprites == nullptr)
            fl_draw("NOT LOADED", X,Y,W,H, FL_ALIGN_CENTER);
          else
            fl_draw((to_string(lbx.offsets.size())+" entries").c_str(), X,Y,W,H, FL_ALIGN_CENTER);


        }
        return;
      }
      default:
        return;
    }
  }
};


class SpriteTable : public Fl_Table
{
private:
  int selection;
  
public:
  SpriteTable() : Fl_Table(FIRST_TABLE_WIDTH,0,SECOND_TABLE_WIDTH,800), selection(-1)
  {
    rows(0);             // how many rows
    row_height_all(20);         // default height of rows
    row_resize(0);              // disable row resizing
    // Cols
    cols(4);             // how many columns
    col_header(1);              // enable column headers (along top)
    col_width_all(SECOND_TABLE_WIDTH/4);          // default width of columns
    col_width(0, 30);
    col_width(1, 140);
    col_width(3, 20);
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
      selection = callback_row();
      
      if (!currentLBX->sprites[selection])
      {
        Repository::loadLBXSpriteData(LBXSpriteInfo(currentLBX->ident, selection));
      }
      
      mywindow->setData(currentLBX->sprites[selection]);
      
      redraw();
    }
  }
  
  static void mycallback(Fl_Widget* w, void* data)
  {
    static_cast<SpriteTable*>(data)->mycallback();
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
        static const char* columnNames[] = {"#", "Sprite Name", "Spec", "P"};
        DrawHeader(columnNames[COL], X, Y, W, H);
        return;
      }
      case CONTEXT_ROW_HEADER:
        return;
      case CONTEXT_CELL:
      {
        int fgcol = selection == ROW ? FL_RED : FL_BLACK;
        int bgcol = FL_WHITE;
        fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, bgcol);
        
        LBXSpriteData* sprite = currentLBX->sprites[ROW];
        
        if (!sprite)
          fgcol = fl_rgb_color(180, 180, 180);
        
        fl_color(fgcol);

        
        if (COL == 0)
          fl_draw(fmt::sprintf("%u", ROW).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
        else if (COL == 1)
        {
          const LBXFileName& entry = assetNames[currentLBX->ident][ROW];
          fl_draw(fmt::sprintf("%s/%s",entry.folder, entry.name).c_str(), X, Y, W, H, FL_ALIGN_LEFT);
        }
        else
        {
          if (sprite)
          {
            if (COL == 2)
              fl_draw(fmt::sprintf("%ux%u (%u)", sprite->width, sprite->height, sprite->count).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
            
            //window->redraw();
          }
          else
          {
  
          }
        }

        return;
      }
      default:
        return;
    }
  }

};

int main(int argc, char **argv) {

  Repository::init();
  
  Fl::visual(FL_RGB);
    
  mywindow = new MyWindow();
  
  tableLbx = new LBXTable();
  tableSprites = new SpriteTable();
  
  mywindow->end();
  mywindow->show(argc, argv);
  return Fl::run();
}
