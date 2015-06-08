#include "LBX.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
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

class LBXTable : public Fl_Table
{
private:
  int selection;
  
  const LBXHolder& holder(int index) { return LBXRepository::holderForID(static_cast<LBXFileID>(index)); }
  
public:
  LBXTable() : Fl_Table(0,0,300,800), selection(-1)
  {
    rows(extent<decltype(names)>::value);             // how many rows
    row_height_all(20);         // default height of rows
    row_resize(0);              // disable row resizing
    // Cols
    cols(2);             // how many columns
    col_header(1);              // enable column headers (along top)
    col_width_all(150);          // default width of columns
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
        const LBXHolder& lbx = holder(ROW);
        
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

class SpriteTable
{
  
};

int main(int argc, char **argv) {
  LBXRepository::init();
  
  Fl_Window *window = new Fl_Window(1280,800);
  
  new LBXTable();
  
  //Fl_Box *box = new Fl_Box(0,0,300,20,"Hello, World!");
  
  window->end();
  window->show(argc, argv);
  return Fl::run();
}