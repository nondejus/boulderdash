#ifndef FUNDA_H
#define FUNDA_H
#include "../sdl/include/SDL.h"

#define ALLOC(p,c) (p*)malloc(sizeof(p)*c)

typedef SDL_Point Point;

typedef enum{
  empty,border,diamond,dirt,monster,player,rock,spider,water,door
}block_t;

typedef enum{
  _none=0,_left,_right,_up,_down,
}Direction;

typedef struct{
  block_t type;
  Point pos;
  int active;
}Block;

typedef struct{
  int width;
  int height;
  int diamondsRequired;
  int time;
  double waterRate;
  Point playerStartPos;
  Point doorPos;
  Block ** blocks;
}Level;

#endif
