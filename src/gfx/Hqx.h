/*
 * Copyright (C) 2003 Maxim Stepin ( maxst@hiend3d.com )
 *
 * Copyright (C) 2010 Cameron Zemek ( grom@zeminvaders.net)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __HQX_H_
#define __HQX_H_

#include <stdint.h>

#if defined( __GNUC__ )
#ifdef __MINGW32__
#define HQX_CALLCONV __stdcall
#else
#define HQX_CALLCONV
#endif
#else
#define HQX_CALLCONV
#endif

#if defined(_WIN32)
#ifdef DLL_EXPORT
#define HQX_API __declspec(dllexport)
#else
#define HQX_API __declspec(dllimport)
#endif
#else
#define HQX_API
#endif

HQX_API void HQX_CALLCONV hqxInit(void);
HQX_API void HQX_CALLCONV hq2x_32( uint32_t * src, uint32_t * dest, int width, int height );
HQX_API void HQX_CALLCONV hq3x_32( uint32_t * src, uint32_t * dest, int width, int height );
HQX_API void HQX_CALLCONV hq4x_32( uint32_t * src, uint32_t * dest, int width, int height );

HQX_API void HQX_CALLCONV hq2x_32_rb( uint32_t * src, uint32_t src_rowBytes, uint32_t * dest, uint32_t dest_rowBytes, int width, int height );
HQX_API void HQX_CALLCONV hq3x_32_rb( uint32_t * src, uint32_t src_rowBytes, uint32_t * dest, uint32_t dest_rowBytes, int width, int height );
HQX_API void HQX_CALLCONV hq4x_32_rb( uint32_t * src, uint32_t src_rowBytes, uint32_t * dest, uint32_t dest_rowBytes, int width, int height );

// BEGIN COMMON

/*
 * Copyright (C) 2003 Maxim Stepin ( maxst@hiend3d.com )
 *
 * Copyright (C) 2010 Cameron Zemek ( grom@zeminvaders.net)
 * Copyright (C) 2011 Francois Gannaz <mytskine@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __HQX_COMMON_H_
#define __HQX_COMMON_H_

#include <stdlib.h>
#include <stdint.h>

#define MASK_2     0x0000FF00
#define MASK_13    0x00FF00FF
#define MASK_RGB   0x00FFFFFF
#define MASK_ALPHA 0xFF000000

#define Ymask 0x00FF0000
#define Umask 0x0000FF00
#define Vmask 0x000000FF
#define trY   0x00300000
#define trU   0x00000700
#define trV   0x00000006

/* RGB to YUV lookup table */
extern uint32_t RGBtoYUV[16777216];

static inline uint32_t rgb_to_yuv(uint32_t c)
{
  // Mask against MASK_RGB to discard the alpha channel
  //return RGBtoYUV[MASK_RGB & c];
  
  /* TODO: possibile fix? */
  if ((c & 0xFF000000) == 0)
    return 0xFF008080;
  
  int r = (c >> 16) & 0xFF;
  int g = (c >> 8) & 0xFF;
  int b = c & 0xFF;
  int y = (int)(0.299*r + 0.587*g + 0.114*b);
  int u = (int)(-0.169*r - 0.331*g + 0.5*b) + 128;
  int v = (int)(0.5*r - 0.419*g - 0.081*b) + 128;
  int cc = (y << 16) + (u << 8) + v;
  //map.put(w, c);
  return cc;
}

/* Test if there is difference in color */
static inline int yuv_diff(uint32_t yuv1, uint32_t yuv2) {
  return (( abs((yuv1 & Ymask) - (yuv2 & Ymask)) > trY ) ||
          ( abs((yuv1 & Umask) - (yuv2 & Umask)) > trU ) ||
          ( abs((yuv1 & Vmask) - (yuv2 & Vmask)) > trV ) );
}

static inline int Diff(uint32_t c1, uint32_t c2)
{
  return yuv_diff(rgb_to_yuv(c1), rgb_to_yuv(c2));
}

/* Interpolate functions */
static inline uint32_t Interpolate_2(uint32_t c1, int w1, uint32_t c2, int w2, int s)
{
  if (c1 == c2) {
    return c1;
  }
  return
  (((((c1 & MASK_ALPHA) >> 24) * w1 + ((c2 & MASK_ALPHA) >> 24) * w2) << (24-s)) & MASK_ALPHA) +
  ((((c1 & MASK_2) * w1 + (c2 & MASK_2) * w2) >> s) & MASK_2)	+
  ((((c1 & MASK_13) * w1 + (c2 & MASK_13) * w2) >> s) & MASK_13);
}

static inline uint32_t Interpolate_3(uint32_t c1, int w1, uint32_t c2, int w2, uint32_t c3, int w3, int s)
{
  return
  (((((c1 & MASK_ALPHA) >> 24) * w1 + ((c2 & MASK_ALPHA) >> 24) * w2 + ((c3 & MASK_ALPHA) >> 24) * w3) << (24-s)) & MASK_ALPHA) +
  ((((c1 & MASK_2) * w1 + (c2 & MASK_2) * w2 + (c3 & MASK_2) * w3) >> s) & MASK_2) +
  ((((c1 & MASK_13) * w1 + (c2 & MASK_13) * w2 + (c3 & MASK_13) * w3) >> s) & MASK_13);
}

static inline void Interp1(uint32_t * pc, uint32_t c1, uint32_t c2)
{
  //*pc = (c1*3+c2) >> 2;
  *pc = Interpolate_2(c1, 3, c2, 1, 2);
}

static inline void Interp2(uint32_t * pc, uint32_t c1, uint32_t c2, uint32_t c3)
{
  //*pc = (c1*2+c2+c3) >> 2;
  *pc = Interpolate_3(c1, 2, c2, 1, c3, 1, 2);
}

static inline void Interp3(uint32_t * pc, uint32_t c1, uint32_t c2)
{
  //*pc = (c1*7+c2)/8;
  *pc = Interpolate_2(c1, 7, c2, 1, 3);
}

static inline void Interp4(uint32_t * pc, uint32_t c1, uint32_t c2, uint32_t c3)
{
  //*pc = (c1*2+(c2+c3)*7)/16;
  *pc = Interpolate_3(c1, 2, c2, 7, c3, 7, 4);
}

static inline void Interp5(uint32_t * pc, uint32_t c1, uint32_t c2)
{
  //*pc = (c1+c2) >> 1;
  *pc = Interpolate_2(c1, 1, c2, 1, 1);
}

static inline void Interp6(uint32_t * pc, uint32_t c1, uint32_t c2, uint32_t c3)
{
  //*pc = (c1*5+c2*2+c3)/8;
  *pc = Interpolate_3(c1, 5, c2, 2, c3, 1, 3);
}

static inline void Interp7(uint32_t * pc, uint32_t c1, uint32_t c2, uint32_t c3)
{
  //*pc = (c1*6+c2+c3)/8;
  *pc = Interpolate_3(c1, 6, c2, 1, c3, 1, 3);
}

static inline void Interp8(uint32_t * pc, uint32_t c1, uint32_t c2)
{
  //*pc = (c1*5+c2*3)/8;
  *pc = Interpolate_2(c1, 5, c2, 3, 3);
}

static inline void Interp9(uint32_t * pc, uint32_t c1, uint32_t c2, uint32_t c3)
{
  //*pc = (c1*2+(c2+c3)*3)/8;
  *pc = Interpolate_3(c1, 2, c2, 3, c3, 3, 3);
}

static inline void Interp10(uint32_t * pc, uint32_t c1, uint32_t c2, uint32_t c3)
{
  //*pc = (c1*14+c2+c3)/16;
  *pc = Interpolate_3(c1, 14, c2, 1, c3, 1, 4);
}

#endif

// END COMMON

#endif
