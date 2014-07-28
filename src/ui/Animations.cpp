//
//  Animations.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Animations.h"

#include "Gfx.h"

void BlinkAnimation::step()
{
  u8 alpha = 0;
  float pos = position();
  

  
  alpha = (u8) (pos < 0.5f ? pos*2*maxAlpha : (1.0f - ((pos - 0.5f)*2))*maxAlpha);
  
  printf("%f POS %d\n", pos, alpha);
  
  Gfx::alphaBlend(rect, (alpha << 24) | (color & 0x00FFFFFF));
}
