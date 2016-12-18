//
//  AlchemyView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "LbxView.h"

#include "Gfx.h"
#include "Font.h"

#include "ViewManager.h"
#include "Buttons.h"

using namespace lbx;

const u16 MAX_PER_PAGE = 23;

LBXView::LBXView(ViewManager* gvm) : View(gvm), selectedLBX(-1), lbxOffset(0), contentOffset(0), selectedContent(-1)
{
  buttons.resize(4);
  buttons[0] = TristateButton::build("prev lbx", 5, 5+23*8, TextureID::ARMIES_ARROWS, 0)->setAction([this](){  });
  buttons[1] = TristateButton::build("next lbx", 15, 5+23*8, TextureID::ARMIES_ARROWS, 1)->setAction([this](){  });
  
  buttons[2] = TristateButton::build("prev content", 50, 5+23*8, TextureID::ARMIES_ARROWS, 0)->setAction([this](){ --contentOffset; updateContentButtons(); });
  buttons[3] = TristateButton::build("next nextcontent", 60, 5+23*8, TextureID::ARMIES_ARROWS, 1)->setAction([this](){ ++contentOffset; updateContentButtons(); });
  
  for (auto &b : buttons) b->hide();
  
  hasNextContent = false;
  hasPrevContent = false;
  hasNextFile = LBX_COUNT > MAX_PER_PAGE;
  hasPrevFile = 0;
  
  //offsets.resize(files.size());
  //headers.resize(files.size());
}

void LBXView::draw()
{
  if (selectedContent != -1)
  {
    LBXSpriteData* s = Repository::data[selectedLBX].sprites[selectedContent];
    
    for (int i = 0; i < s->count; ++i)
    {
      Gfx::draw(s, 0, i*s->height, i);
    }
  }
  
  
  for (int i = 0; i < LBX_COUNT; ++i)
    Fonts::drawString(Repository::data[i].fileName, selectedLBX == i+lbxOffset ? FontFaces::Small::REDW : FontFaces::Small::WHITE, 5, 5+i*8, ALIGN_LEFT);
  
  if (selectedLBX != -1)
  {
    std::map<LBXID, string_list>::iterator it = filesForLBX.find(static_cast<LBXID>(selectedLBX));
    
    if (it != filesForLBX.end())
    {
      for (int i = 0; i < MAX_PER_PAGE; ++i)
      {
        if (i + contentOffset < it->second.size())
        {
          auto &el = it->second[i+contentOffset];
          Fonts::drawString(std::string(el.folder)+"/"+el.name, selectedContent == i+contentOffset ? FontFaces::Small::REDW : FontFaces::Small::WHITE, 50, 5+i*8, ALIGN_LEFT);
        }
      }
    }
  }
  
  
}

void LBXView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (x >= 0 && y > 5 && y < HEIGHT)
  {
    x -= 5;
    y -= 5;
    
    y /= 8;
    
    if (y < MAX_PER_PAGE)
    {
      if (y+lbxOffset < LBX_COUNT && x < Fonts::stringWidth(FontFaces::Small::WHITE, Repository::data[y].fileName) + 10)
      {
        selectedLBX = y+lbxOffset;
        selectedContent = -1;
        selectLBX();
      }
      else if (y+contentOffset < filesForLBX[static_cast<LBXID>(selectedLBX)].size() && x > 50 && x <= 100)
      {
        selectedContent = y+contentOffset;
        selectGFX();
      }
    }
  }
}

void LBXView::updateContentButtons()
{
  buttons[3]->showIf(contentOffset + MAX_PER_PAGE < filesForLBX[static_cast<LBXID>(selectedLBX)].size());
  buttons[2]->showIf(contentOffset > 0);
}

void LBXView::selectLBX()
{
  if (Repository::shouldAllocateLBX(static_cast<LBXID>(selectedLBX)))
  {
    Repository::loadLBX(static_cast<LBXID>(selectedLBX));
    
    LBXFile &lbx = Repository::data[selectedLBX];
    string_list fileNames;
    
    std::string name = LBX::getLBXPath(lbx.fileName);
    FILE *in = fopen(name.c_str(),"rb");
    LBX::scanFileNames(lbx.info, fileNames, in);
    fclose(in);
    filesForLBX[static_cast<LBXID>(selectedLBX)] = fileNames;
  }
  
  updateContentButtons();
}

void LBXView::selectGFX()
{
  SpriteInfo info = SpriteInfo(static_cast<LBXID>(selectedLBX), selectedContent);
  
  if (Repository::shouldAllocateSprite(info))
    Repository::loadLBXSpriteData(info);
}


