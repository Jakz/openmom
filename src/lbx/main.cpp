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

#define MAX_ROWS 100
#define MAX_COLS 100

using namespace std;

std::string names[] = {
  "armylist",
  "backgrnd",
  "units1",
  "units2",
  "mainscrn",
  "main",
  "specfx"
};

constexpr int FIRST_TABLE_WIDTH = 150;
constexpr int SECOND_TABLE_WIDTH = 320;
constexpr int WINDOW_HEIGHT = 800;
constexpr int WINDOW_WIDTH = 1440;

class MyWindow;
class LBXTable;
class SpriteTable;
class PreviewWidget;

MyWindow *mywindow = nullptr;
LBXTable *tableLbx;
Fl_Table *tableSprites;

PreviewWidget* preview = nullptr;

const LBXFile* currentLBX = nullptr;

class MyWindow : public Fl_Double_Window
{
private:
  const LBXSpriteData* sprite;

public:
  MyWindow() : Fl_Double_Window(WINDOW_WIDTH, WINDOW_HEIGHT), sprite(nullptr)
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
      int sx = 302+322+50;
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
            Color pixel = sprite->palette->get(sprite->data[i][x + y*w]);
            u8 r = GET_RED(pixel), g = GET_GREEN(pixel), b = GET_BLUE(pixel);
            
            tdata[x*S*3 + y*w*3*S*S] = r; tdata[x*S*3 + 1 + y*w*3*S*S] = g; tdata[x*S*3 + 2 + y*w*3*S*S] = b;
            tdata[(x*S+1)*3 + y*w*3*S*S] = r; tdata[(x*S+1)*3 + 1 + y*w*3*S*S] = g; tdata[(x*S+1)*3 + 2 + y*w*3*S*S] = b;
            
            tdata[x*S*3 + y*w*3*S*S + w*3*S] = r; tdata[x*S*3 + 1 + y*w*3*S*S + w*3*S ] = g; tdata[x*S*3 + 2 + y*w*3*S*S + w*3*S] = b;
            tdata[(x*S+1)*3 + y*w*3*S*S + w*3*S] = r; tdata[(x*S+1)*3 + 1 + y*w*3*S*S + w*3*S] = g; tdata[(x*S+1)*3 + 2 + y*w*3*S*S + w*3*S] = b;
            
            //tdata[(i*S+w*S)*3] = r; tdata[(i*S+w*S)*3 + 1] = g; tdata[(i*S+w*S)*3 + 2] = b;
          }
        }
        
        fl_draw_image(tdata, sx, sy+(h+2)*S*i, w*S, h*S);
        
        delete [] tdata;
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
  
  const LBXFile& holder(int index) { return LBXRepository::holderForID(static_cast<LBXFileID>(index)); }
  
public:
  LBXTable() : Fl_Table(0,0,FIRST_TABLE_WIDTH,800), selection(-1)
  {
    rows(extent<decltype(names)>::value);             // how many rows
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
      
      if (holder(selection).sprites == nullptr)
        LBXRepository::loadLBX(static_cast<LBXFileID>(selection));
      
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
        LBXRepository::loadLBXSpriteData(LBXSpriteInfo(currentLBX->ident, selection));
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
        static const char* columnNames[] = {"Sprite Name", "Size", "#", "Palette?"};
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
        fl_color(fgcol);
        
        LBXSpriteData* sprite = currentLBX->sprites[ROW];
        
        if (sprite)
        {
          if (COL == 1)
            fl_draw(fmt::sprintf("%ux%u", sprite->width, sprite->height).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
          else if (COL == 2)
            fl_draw(fmt::sprintf("%u", sprite->count).c_str(), X,Y,W,H, FL_ALIGN_CENTER);
          
          //window->redraw();
        }
        else
        {
          if (COL == 0)
            fl_draw("NOT LOADED", X, Y, W, H, FL_ALIGN_CENTER);
        }


        return;
      }
      default:
        return;
    }
  }

};

int main(int argc, char **argv) {
  LBXRepository::init();
  
  Fl::visual(FL_RGB);
  
  mywindow = new MyWindow();
  
  tableLbx = new LBXTable();
  tableSprites = new SpriteTable();
  
  mywindow->end();
  mywindow->show(argc, argv);
  return Fl::run();
}