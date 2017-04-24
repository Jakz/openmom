//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "IntroView.h"

#include "Gfx.h"
#include "ViewManager.h"

IntroView::IntroView(ViewManager* gvm) : View(gvm), currentFrame(0)
{

}

void IntroView::activate()
{

}

void IntroView::draw()
{
  Gfx::drawAnimated(LSI(INTRO, 3), 0, 0, 0, 2);
}
