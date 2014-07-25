#include "WorldGenerator.h"

#include "Util.h"

AtlasMap::AtlasMap(s32 width, s32 height, char defaultValue) :
  W(width), H(height), mapGrid(new char[width*height])
{
  memset(mapGrid, defaultValue, width*height);
  // TODO: dealloc on destruction
}

void AtlasMap::initialize(s32 w, s32 h, s32 continents, s32 continentSize, s32 islands, IslandType islandType, CoastWaters coastType, bool wrapX, bool wrapY, CoastMode coastMode)
{
  W = w;
  H = h;
  this->wrapX = wrapX;
  this->wrapY = wrapY;
  ISLAND_SIZE = continentSize;
  coastlineMode = coastMode;

  placeContinents(continents);
  cellCountRadius(continentSize, 0.8f + 0.04f * continentSize, 'G', 'w', 'G', 1);
  cellCount4(ISLAND_SIZE, 0.3f, 'G', 'w', 'G');
  placeIslands(islands, islandType);
  bays(continentSize);
  coastalCorrection(coastType);
}


s32 AtlasMap::adjacentType(s32 x, s32 y, char t)
{
  int i = 0;
  if (get(downX(x),y) == t) ++i;
  if (get(x,downY(y)) == t) ++i;
  if (get(x,upY(y)) == t) ++i;
  if (get(upX(x),y) == t) ++i;
  return i;
}

s32 AtlasMap::surroundingType(s32 x, s32 y, char t, s32 d)
{
  int i = 0;
  
  if (get(downX(x),downY(y)) == t) i++;
  if (get(downX(x),y) == t) i++;
  if (get(downX(x),upY(y)) == t) i++;
  
  if (get(x,downY(y)) == t) i++;
  if (get(x,upY(y)) == t) i++;
  
  if (get(upX(x),downY(y)) == t) i++;
  if (get(upX(x),y) == t) i++;
  if (get(upX(x),upY(y)) == t) i++;
  
  return i;
}

void AtlasMap::placeContinents(s32 count)
{
  s32 m;
  for (int i = 0; i < count; i++) {
    s32 j = (s32)(Util::randomIntUpTo(W) - 0.5f);
    int k = 0;
    for (m = 0; k == 0; m++) {
      s32 n = (s32)(Util::randomIntUpTo(H) - 0.5f);
      k = n;
      if (equatorDistance(n) <= 0.7f + 0.25f * Util::rand() - 0.02f * ISLAND_SIZE) continue; k = 0;
    }
    m = (s32)std::ceil(6.0f * Util::rand()); // 0-6 inclusive but seed has 9 elements TODO
    placeContinentSeed(j, k, m);
  }
}

void AtlasMap::placeContinentSeed(s32 x,s32 y,s32 t) // TODO: optimizable by placing all data in static array
{
  static const s32 LENGTH = 14;
  
  s32* data[2];
  
  switch (t)
  {
    case 1:
    {
      static s32 ar0[] = { 2, 3, 2, 3, 0, 1, 2, -1, 0, 1, -2, -1, -2, -1 };
      static s32 ar1[] = { -2, -2, -1, -1, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    case 2:
    {
      static s32 ar0[] = { 2, -2, -1, 1, 2, -2, -1, 0, 1, -2, -1, 0, -1, 0 };
      static s32 ar1[] = { -2, -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 2, 2 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    case 3:
    {
      static s32 ar0[] = { -1, 0, 1, 2, -2, -1, 2, 3, -2, 2, 3, -2, -2, -1 };
      static s32 ar1[] = { -2, -2, -2, -2, -1, -1, -1, -1, 0, 0, 0, 1, 2, 2 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    case 4:
    {
      static s32 ar0[] = { -1, 0, 1, -1, 0, 1, 2, -1, 0, 1, 2, 0, 1, 2 };
      static s32 ar1[] = { -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    case 5:
    {
      static s32 ar0[] = { -1, 0, -2, -1, 0, 1, -2, 1, 2, -2, 2, 3 };
      static s32 ar1[] = { -1, -1, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    case 6:
    {
      static s32 ar0[] = { 1, -1, 0, 1, 2, 3, 0, 2, -1, 0, 1, 2, 3, 1 };
      static s32 ar1[] = { -1, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 3 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    case 7:
    {
      static s32 ar0[] = { 0, 1, 2, 3, 0, 1, 2, 3, -2, -1, 0, 1, 1, 1 };
      static s32 ar1[] = { -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 3 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    case 8:
    {
      static s32 ar0[] = { -2, 1, 2, 3, -1, 2, 3, 1, -1, 0, 1, 2, -1, 0 };
      static s32 ar1[] = { -2, -2, -2, -2, -1, -1, -1, 1, 2, 2, 2, 2, 3, 3 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
    default:
    {
      static s32 ar0[] = { 1, 2, -1, 0, 1, -1, 0, -2, -1, 0, 1, -2, 1, -2 };
      static s32 ar1[] = { -2, -2, -1, -1, -1, 0, 0, 1, 1, 1, 1, 2, 2, 3 };
      data[0] = ar0; data[1] = ar1;
      break;
    }
  }
  
  for (int i = 0; i < LENGTH; ++i)
    set(upX(x, data[0][i]), upY(y, data[1][i]), 'G');
}

void AtlasMap::placeIslands(s32 count, IslandType type)
{
  if (type == IslandType::RANDOM) type = static_cast<IslandType>(Util::randomIntInclusive(1, 3));
  
  switch (type) {
    case IslandType::MIXED: mixedIslands(count); break;
    case IslandType::ARCHIPELAGO: archipelago(25*count, true); break;
    case IslandType::LONELY: lonelyIslands(count); break;
    default: break;
  }
}

void AtlasMap::lonelyIslands(s32 count)
{
  for (int i = 0; i < count; ++i)
  {
    s32 j = Util::randomIntUpTo(W); // TODO this order shouldn't work since it gives ( [0:1]*W ) - 0.5f
    s32 k = Util::randomIntUpTo(H);
    set(j, k, 'G');
  }
  cellCount4(3 * ISLAND_SIZE, 0.3f, 'G', 'w', 'G');
}

void AtlasMap::archipelago(s32 count, bool flag)
{
  s32 i = 25;
  s32 j = 7;
  s32 k = count / i;
  s32 m = count % i;
  
  if (flag) cellCount4(3 * ISLAND_SIZE, 0.3f, 'G', 'w', 'G');
  
  s32 n, i1, i2, i3, i4, i5, i6;
  for (i6 = 0; i6 < k; i6++)
  {
    n = Util::randomIntUpTo(W);
    i1 = Util::randomIntUpTo(H);
    
    for (int i7 = 0; i7 < i; i7++) {
      i2 = (s32)std::floor(Util::rand(2*j) - j);
      i3 = (s32)std::floor(Util::rand(2*j) - j);
      if (i2 <= 0) i4 = downX(n, -1 * i2); else
        i4 = upX(n, i2);
      if (i3 <= 0) i5 = downY(i1, -1 * i3); else
        i5 = upY(i1, i3);
      set(i4, i5, 'G');
    }
    
  }
  
  n = Util::randomIntUpTo(W);
  i1 = Util::randomIntUpTo(H);
  
  if (m > 0)
  {
    for (i6 = 0; i6 < m; i6++)
    {
      i2 = (s32)std::floor(Util::rand(2*j) - j);
      i3 = (s32)std::floor(Util::rand(2*j) - j);
      if (i2 <= 0) i4 = downX(n, -1 * i2); else
        i4 = upX(n, i2);
      if (i3 <= 0) i5 = downY(i1, -1 * i3); else
        i5 = upY(i1, i3);
      set(i4, i5, 'G');
    }
  }
}

void AtlasMap::mixedIslands(s32 count)
{
  float d = Util::rand();
  
  int i = (int)(d * count);
  int j = count - i;
  
  lonelyIslands(j);
  archipelago(10 * i, false);
}

void AtlasMap::coastalCorrection(CoastWaters type)
{
  cellCountRadius(ISLAND_SIZE, 0.7f, 'w', 'g', 'w', 1);
  cellCountRadius(1, 1.0f, 'G', 'w', 'c', 1);

  if (type == CoastWaters::RANDOM) type = static_cast<CoastWaters>(Util::randomIntInclusive(1, 6));
  
  switch (type)
  {
    case CoastWaters::VARIED: { cellCountRadius(3, 0.08f, 'c', 'w', 'c', 1); cellCountRadius(3, 0.12f, 'w', 'c', 'w', 1); break; }
    case CoastWaters::VERY_CALM: cellCountRadius(5, 0.16f, 'c', 'w', 'c', 1); break;
    case CoastWaters::CALM: cellCountRadius(4, 0.08f, 'c', 'w', 'c', 1); break;
    case CoastWaters::NORMAL: break;
    case CoastWaters::WILD: cellCountRadius(4, 0.05f, 'w', 'c', 'w', 1);
    case CoastWaters::VERY_WILD: cellCountRadius(5, 0.2f, 'w', 'c', 'w', 1);
    default: break;
  }
}

void AtlasMap::cellCount4(s32 count, float chance, char t1, char t2, char t3)
{
  for (int i = 0; i < count; ++i)
  {
    char* tmp = new char[W*H];
    memcpy(tmp, mapGrid, W*H);
    
    for (int i = 0; i < W; ++i)
    {
      for (int j = 0; j < H; ++j)
      {
        if (get(i,j) == t2 && Util::chance(chance) && adjacentType(i, j, t1) > 0)
          tmp[i+j*W] = t3;
      }
    }
    
    memcpy(mapGrid, tmp, W*H);
    delete [] tmp;
  }
}

void AtlasMap::cellCountRadius(s32 count, float chance, char t1, char t2, char t3, s32 radius)
{
  for (int i = 0; i < count; ++i)
  {
    char* tmp = new char[W*H];
    memcpy(tmp, mapGrid, W*H);

    for (int i = 0; i < W; ++i)
    {
      for (int j = 0; j < H; ++j)
      {
        if (get(i,j) != t2 || Util::chance(1.0f-chance) || surroundingType(i, j, t1, radius) <= 0) continue;
        else
          tmp[i+j*W] = t3;
      }
    }

    memcpy(mapGrid, tmp, W*H);
    delete [] tmp;
  }
}

void AtlasMap::bays(s32 count)
{
  float d1 = 0.0f;
  float d2 = 0.0f;
  float d3 = 0.9f;
  s32 i = 3, j = 0;
  
  if (coastlineMode == CoastMode::RANDOM) coastlineMode = static_cast<CoastMode>(Util::randomIntInclusive(1, 3));
  
  if (coastlineMode == CoastMode::IRREGULAR) { d1 = 0.1f; d2 = 0.6f; }
  if (coastlineMode == CoastMode::CHAOTIC) { d1 = 0.6f; d2 = 0.32f;
    count = (s32)std::ceil(count * 1.33f);
    d3 = 0.4f; i = 5;
  }
  
  char* tmp = new char[W*H];
  memcpy(tmp, mapGrid, W*H);
  s32 k;
  for (j = 0; j < W; j++) {
    for (k = 0; k < H; k++) {
      if ((get(j,k) != 'G') || (surroundingType(j, k, 'w', 1) <= 0) || (surroundingType(j, k, 'w', 1) >= 6) || (Util::chance(1.0f-d1)))
        continue;
     
      tmp[j + k*W] = 'b';
    }
    
    memcpy(mapGrid, tmp, W*H);
  }
  
  for (j = 0; j < count; j++)
  {
    memcpy(tmp, mapGrid, W*H);
    for (k = 0; k < W; k++) {
      for (int m = 0; m < H; m++) {
        if ((get(k,m) != 'G') || (surroundingType(k, m, 'b', 1) <= 0) || (surroundingType(k, m, 'b', 1) >= i) || (Util::chance(1.0f-d2)))
          continue;
      
        tmp[k + m*W] = 'b';
      }
    }
    memcpy(mapGrid, tmp, W*H);
  }

  for (j = 0; j < W; j++) {
    for (k = 0; k < H; k++) {
      if (tmp[j+k*W] == 'b') {
        tmp[j+k*W] = 'w';
      }
      
      if ((tmp[j+k*W] != 'G') || (surroundingType(j, k, 'w', 1) + surroundingType(j, k, 'b', 1) <= 5) || (Util::chance(1.0f-d3))) {
        continue;
      }
      tmp[j+k*W] = 'w';
    }
    
    memcpy(mapGrid, tmp, W*H);
  }
  
  delete [] tmp;
  
  if (coastlineMode == CoastMode::IRREGULAR) removeIsles();
}

void AtlasMap::removeIsles()
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < W; j++)
      for (int k = 0; k < H; k++)
      {
        if (get(j,k) != 'G' || surroundingType(j, k, 'w', 1) <= 5 || Util::chance(0.4f))
          continue;
        set(j,k,'w');
      }
}


float AtlasMap::equatorDistance(s32 p)
{
  float d1 = p - 0.5f * H;
  float d2 = std::abs(d1);
  float d3 = d2 / (0.5f * H);
  return d3;
}