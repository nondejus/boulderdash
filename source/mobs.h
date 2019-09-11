#ifndef MOBS_H
#define MOBS_H
#include <SDL2/SDL.h>
#include "fundamentals.h"
int monster_move(Block **map, Point m, Point p);
int spider_move(Block **map, Point s);
void create_diamonds(Block **map, Point coords, block_t t, int mapW, int mapH);
#endif
