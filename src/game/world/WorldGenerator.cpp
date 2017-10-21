#include "WorldGenerator.h"

#include "common/Util.h"

using namespace std;

AtlasMap::AtlasMap(s32 width, s32 height, char defaultValue) :
  W(width), H(height), mapGrid(new char[width*height])
{
  memset(mapGrid, defaultValue, width*height);
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
  s32 LENGTH = 14;
  
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
      LENGTH = 12;
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
  {
    s32 xx = upX(x, data[0][i]);
    s32 yy = upY(y, data[1][i]);
    set(xx, yy, 'G');
  }
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
    s32 j = Util::randi(W); // TODO this order shouldn't work since it gives ( [0:1]*W ) - 0.5f
    s32 k = Util::randi(H);
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

#pragma mark WorldGenerator

void WorldGenerator::count(Plane plane)
{
  fill(begin(quantities), end(quantities), 0);
  totalTiles = 0;
  
  for (int x = 0; x < world->width(); ++x)
    for (int y = 0; y < world->height(); ++y)
    {
      TileType type = world->get(x, y, plane)->type;
      s32 oldValue = quantities[type];
      quantities.set(type, oldValue+1);
      ++totalTiles;
    }
}

void WorldGenerator::makeAreas(Plane plane, TileType type,  float perc, float expandO, float expandT, int limit)
{
  count(plane);
  
  s32 placed = 0;
  s32 itotal = static_cast<s32>(perc*quantities[TileType::GRASS]);
  
  // TODO: break after some time if stuck
  
  for (s32 i = 0; placed < itotal; ++i)
  {
    s32 gx = Util::randomIntUpTo(W);
    s32 gy = Util::randomIntUpTo(H);
    
    if (gx > 0 && gx < W && gy > 0 && gy < H && world->get(gx, gy, plane)->type == TileType::GRASS && (limit == 0 || (gy < limit && gy > H-limit)) )
    {
      world->set(type, gx, gy, plane);
      
      for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1;++y)
          if (gx+x >= 0 && gx+x < W && gy+y >= 0 && gy+y < H && std::abs(x) - std::abs(y) != 0 && (limit == 0 || (gy+y > limit && gy+y > H-limit)))
            if (Util::chance(expandO) && world->get(gx+x, gy+y, plane)->type == TileType::GRASS)
            {
              
              world->set(type, gx+x, gy+y, plane);
							
              if (y != 0)
              {
                Tile* t = world->get(gx+x-1, gy+y, plane);
                if (Util::chance(expandT) && t && t->type == TileType::GRASS)
                  world->set(type, gx+x-1, gy+y, plane);
                
                t = world->get(gx+x+1, gy+y, plane);
                if (Util::chance(expandT) && t && t->type == TileType::GRASS)
                  world->set(type, gx+x+1, gy+y, plane);
              }
              else if (x != 0)
              {
                Tile* t = world->get(gx+x, gy+y-1, plane);
                if (Util::chance(expandT) && t && t->type == TileType::GRASS)
                  world->set(type, gx+x, gy+y-1, plane);
                
                t = world->get(gx+x, gy+y+1, plane);
                if (Util::chance(expandT) && t && t->type == TileType::GRASS)
                  world->set(type, gx+x, gy+y+1, plane);
              }
            }		
      ++placed;
    }
  }
}

void WorldGenerator::makeChains(Plane plane, TileType type,  float perc, float branchProb, int length)
{
  count(plane);
  s32 placed = 0;
  s32 itotal = (int)(perc*quantities[TileType::GRASS]);
  
  for (s32 i = 0; placed < itotal; ++i)
  {
    int gx = Util::randomIntUpTo(W);
    int gy = Util::randomIntUpTo(H);
    
    if (world->get(gx, gy, plane)->type == TileType::GRASS)
    {
      world->set(type, gx, gy, plane);
      
      int l = Util::randomIntUpTo(length);
      PositionOffset d;
      
      for (int k = 0; k < l; ++k)
      {
        if (k == 0 ||  Util::chance(branchProb))
          d = Util::ODIRS[Util::randomIntUpTo(4)];
        int rx = gx+d.x;
        int ry = gy+d.y;
        while (!(rx > 0 && rx < W && ry > 0 && ry < H))
        {
          d = Util::ODIRS[Util::randomIntUpTo(4)];
          rx = gx+d.x;
          ry = gy+d.y;
        }
        
        if (world->get(rx, ry, plane)->type == TileType::GRASS)
          world->set(type, rx, ry, plane);
        
        gx = rx;
        gy = ry;
      }
      
      ++placed;
    }
  }
}

void WorldGenerator::makeSpots(Plane plane, TileType type, float perc)
{
  count(plane);
  s32 placed = 0;
  s32 itotal = (int)(perc*quantities[TileType::GRASS]);
  
  for (s32 i = 0; placed < itotal; ++i)
  {
    s32 gx = Util::randomIntUpTo(W);
    s32 gy = Util::randomIntUpTo(H);
    
    if (gx > 0 && gx < W && gy > 0 && gy < H && world->get(gx, gy, plane)->type == TileType::GRASS )
    {
      world->set(type, gx, gy, plane);
      ++placed;
    }
  }
}

void WorldGenerator::makeTundraEdges(Plane plane)
{
  s32 l[] = {1,1}, c[] = {0,0};
  
  for (int x = 0; x < W; ++x)
  {
    world->set(TileType::TUNDRA, x, 0, plane);
    world->set(TileType::TUNDRA, x, H-1, plane);
    
    if (l[0] == 2)
      world->set(TileType::TUNDRA, x, 1, plane);
    if (l[1] == 2)
      world->set(TileType::TUNDRA, x, H-2, plane);
    
    for (int k = 0; k < 2; ++k)
    {
      if (Util::chance(c[k]/10.0f))
      {
        l[k] = l[k] == 1 ? 2 : 1;
        c[k] = 1;
      }
      else
        ++c[k];
    }
    
    /*for (int k = 0; k < 2; ++k)
     {
     if (l[k] == 1)
     world->set(TileType::TUNDRA, x, y[k][0], Plane.ARCANUS);
     else
     {
     world->set(TileType::TUNDRA, x, y[k][0], Plane.ARCANUS);
     world->set(TileType::TUNDRA, x, y[k][1], Plane.ARCANUS);
     }
     l[k] = Util::chance(0.5f) ? 1 : 2;
     }*/
  }
}

void WorldGenerator::makeNodes(Plane plane)
{
  s32 total = plane == ARCANUS ? 16 : 14;
  s32 minDistance = 10, counter = 1;
  s32 cur = 0;
  while (cur < total)
  {
    s32 gx = Util::randomIntUpTo(W);
    s32 gy = Util::randomIntUpTo(H);
    
    if (gx > 0 && gx < W && gy > 0 && gy < H && world->get(gx, gy, plane)->type == TileType::GRASS)
    {
      // check if there are nodes within minimal distance
      bool allowed = true;
      for (s32 x = -minDistance; x <= minDistance; ++x)
        for (s32 y = -minDistance; y <= minDistance; ++y)
        {
          Tile* t = world->get(gx+x, gy+y, plane);
          if (t && t->node())
            allowed = false;
        }
      
      // if we spent at least 500 tries looking for a tile at valid distance reduce the maximum, probably we must be less restrictive
      if (counter % 500 == 0 && !allowed && minDistance > 1)
        --minDistance;
      else if (counter % 50000 == 0 && !allowed && minDistance > 0)
        --minDistance;
      
      if (!allowed)
      {
        ++counter;
        continue;
      }
      
      /* count tiles around the location to decide the type of the node */
      int sea = 0, forest = 0, mountain = 0;
      for (int x = -2; x <= 2; ++x)
        for (int y = -2; y <= 2; ++y)
        {
          Tile* t = world->get(gx+x, gy+y, plane);
          if (t)
            switch (t->type) {
              case TileType::OCEAN:
                ++sea;
                break;
              case TileType::FOREST:
              case TileType::SWAMP:
                ++forest;
                break;
              case TileType::HILL:
              case TileType::MOUNTAIN:
                ++mountain;
                break;
              default: break;
            }
        }
      
      School schools[] = {SORCERY,NATURE,CHAOS};
      School type = School::NO_SCHOOL;
      
      if (sea == forest && mountain == forest) type = schools[Util::randomIntUpTo(3)];
      else if (sea >= forest && sea >= mountain) type = SORCERY;
      else if (forest >= sea && forest >= mountain) type = NATURE;
      else if (mountain >= sea && mountain >= forest) type = CHAOS;
      
      world->get(gx,gy,plane)->placePlace(mapMech->generateManaNode(static_cast<World*>(world), Position(gx, gx, plane), type));

      counter = 1;
      ++cur;
    }			
  }
}

void WorldGenerator::makeLairs()
{
  s32 lairs[] = {25,32}; // lairs, weak lairs
  s32 minDistance = 2, counter = 1;
  
  PlaceType places[] =
  {
    PlaceType::MYSTERIOUS_CAVE,
    PlaceType::DUNGEON,
    PlaceType::ANCIENT_TEMPLE,
    PlaceType::ABANDONED_KEEP,
    PlaceType::MONSTER_LAIR,
    PlaceType::RUINS,
    PlaceType::FALLEN_TEMPLE
  };
  
  s32 placeTypes = sizeof(places)/sizeof(places[0]);
  
  for (int plane = 0; plane < PLANE_COUNT; ++plane)
  {
    int c = 0;
    //counter = 1;
    while (c < lairs[plane])
    {
      int gx = Util::randomIntUpTo(W);
      int gy = Util::randomIntUpTo(H);
      Plane gp = static_cast<Plane>(Util::randomIntUpTo(2)); // TODO: why extracting random plane? check behavior
      
      Tile* t = world->get(gx, gy, gp);
      
      // TODO: then place resources underneath, it shouldn't be that way
      
      if (t->type != TileType::OCEAN && t->type != TileType::SHORE && t->type != TileType::TUNDRA && !t->node() && !t->place() && t->resource == Resource::NONE)
      {
        // check if there are places within minimal distance
        bool allowed = true;
        for (int x = -minDistance; x <= minDistance; ++x)
          for (int y = -minDistance; y <= minDistance; ++y)
          {
            Tile* t2 = world->get(gx+x, gy+y, gp);
            if (t2 && t2->place())
              allowed = false;
          }
        
        // if we spent at least 500 tries looking for a tile at valid distance reduce the maximum, probably we must be less restrictive
        if (counter % 500 == 0 && !allowed && minDistance > 1)
          --minDistance;
        else if (counter % 50000 == 0 && !allowed && minDistance > 0)
          --minDistance;
        
        if (!allowed)
        {
          ++counter;
          continue;
        }
        
        //TODO: weak dipende da plane esterno ma poi plane viene estratto random? check behavior
        t->placePlace(new Place(places[Util::randomIntUpTo(placeTypes)], plane == MYRRAN));
        ++c;
      }
    }
  }
}

void WorldGenerator::atlasGenerate()
{
  s32 continents = 12;
  s32 continentsSize = 1;
  s32 islands = 28;
  IslandType islandType = IslandType::MIXED;
  CoastWaters coastWaters = CoastWaters::CALM;
  CoastMode coastMode = CoastMode::BLOBBY;
  
  AtlasMap atlas = AtlasMap(W, H, 'w');
  atlas.initialize(W, H, continents, continentsSize, islands, islandType, coastWaters, true, false, coastMode);
  atlas.fill(world, ARCANUS);
  atlas.print();
  
  atlas = AtlasMap(W, H, 'w');
  atlas.initialize(W, H, continents, continentsSize, islands, islandType, coastWaters, true, false, coastMode);
  atlas.fill(world, MYRRAN);

}

void WorldGenerator::generate()
{
  atlasGenerate();
  
  for (int i = 0; i < PLANE_COUNT; ++i)
  {
    Plane which = static_cast<Plane>(i);
    
    makeAreas(which, TileType::DESERT,  0.04f, 0.40f, 0.30f,0);
  	makeChains(which, TileType::MOUNTAIN, 0.07f, 0.60f, 9);
  	makeChains(which, TileType::HILL, 0.09f, 0.60f, 9);
    
  	makeSpots(which, TileType::SWAMP, 0.05f);
  	makeAreas(which, TileType::TUNDRA,  0.01f, 0.20f, 0.20f,7);
  	makeSpots(which, TileType::FOREST, 0.40f);
    
    makeTundraEdges(which);

  	makeNodes(which);
  }
  
  makeLairs();

}

