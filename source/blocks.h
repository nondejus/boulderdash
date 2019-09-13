#ifndef BLOCK_H
#define BLOCK_H
#include "../sdl/include/SDL.h"
#include "fundamentals.h"
void activate(Block ** map, Point pos);
void deactivate(Block ** map, Point pos);
void set_block(Block ** map, Point pos, block_t block);
void set_block_xy(Block ** map, int x, int y, block_t block);
block_t get_block(Block ** map, Point pos);
block_t get_block_xy(Block ** map, int x, int y);
block_t get_side(Block ** map, SDL_Point p, Direction d);
int block_move(Block ** map, SDL_Point p, Direction d);
void find_blocks(Block ** map, SDL_Point ** blocks,int * size,block_t type, int mapW, int mapH);
Point side_pos(Point p, Direction d);
#endif
