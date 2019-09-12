#ifndef MAP_H
#define MAP_H
#include<SDL2/SDL.h>
#include "fundamentals.h"
Level level_picker(int i);
Block ** take_roi(Level * map, int width, int height, Point p);
void free_roi(Block ** roi,int h);
Level load_from_file(char * file_name);

#endif
