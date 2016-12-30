//
//  ResearchView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _RESEARCH_VIEW_H_
#define _RESEARCH_VIEW_H_

#include "View.h"

class ViewManager;
class BookView;

class ResearchView : public View
{
private:
  enum
  {
    PREV_PAGE,
    NEXT_PAGE,
    
    BUTTON_COUNT
  };
  
  void draw() override;
  void drawPost() override { }
  
  void indexClicked(u16 index);
  
  BookView *book;
  
public:
  ResearchView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};

#endif
