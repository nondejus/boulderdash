#ifndef RENDER_H
#define RENDER_H
#include <SDL2/SDL.h>
#include <stdio.h>
#include "fundamentals.h"

void render_block(Block *block, SDL_Renderer * renderer, int size, int anim, Direction p);
void render_number(SDL_Renderer * renderer, int num, Point c, int size, int len);
void draw_roi(Level * map,SDL_Renderer * renderer,int w, int h, int anim, Direction p);
SDL_Rect create_rect(Point p, int w, int h);
SDL_Rect create_rect_xy(int x, int y, int w, int h);

#endif
