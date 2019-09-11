
#include <SDL2/SDL.h>
#include <stdio.h>
#include "fundamentals.h"
#include "render.h"
#include "textures.h"
#include "map.h"
#define BLOCK_SIZE 30

int max_limit(int a, int max);

SDL_Rect create_rect(Point p, int w, int h){
  SDL_Rect rect;
  rect.x = p.x;
  rect.y = p.y;
  rect.w = w;
  rect.h = h;
  return rect;
}
SDL_Rect create_rect_xy(int x, int y, int w, int h){
  SDL_Rect rect = {x,y,w,h};
  return rect;
}

SDL_Rect point_to_rect(Point p, int size){
  SDL_Rect rect;
  rect.x=p.x*size;
  rect.y=p.y*size;
  rect.w=size;
  rect.h=size;
  return rect;
}

void render_number_left(SDL_Renderer * renderer, int num, Point c, int size){
  int len,w,h;
  SDL_Rect source,dest;
  char text[100];
  SDL_QueryTexture(tDigits,NULL,NULL,&w,&h);
  sprintf(text,"%d",num);
  for(len=0;text[len+1]!='\0';len++);
  for(;len>=0;len--){
    dest.h=size;
    dest.w=size;
    dest.x=c.x+len*size;
    dest.y=c.y;
    source.y=0;
    source.w=w/10;
    source.h=h;
    switch (text[len]) {
      case '1': source.x=source.w; break;
      case '2': source.x=source.w*2; break;
      case '3': source.x=source.w*3; break;
      case '4': source.x=source.w*4; break;
      case '5': source.x=source.w*5; break;
      case '6': source.x=source.w*6; break;
      case '7': source.x=source.w*7; break;
      case '8': source.x=source.w*8; break;
      case '9': source.x=source.w*9; break;
      case '0': source.x=0; break;
    }
    SDL_RenderCopy(renderer,tDigits,&source,&dest);
  }
}
void render_number_right(SDL_Renderer * renderer, int num, Point c, int size){
  int len,i,w,h;
  SDL_Rect source,dest;
  char text[100];
  SDL_QueryTexture(tDigits,NULL,NULL,&w,&h);
  sprintf(text,"%d", num);
  for(len=0;text[len+1]!='\0';len++);
  for(i=0;text[i]!='\0';i++){
    dest.h=size;
    dest.w=size;
    dest.x=c.x-(1+len-i)*size;
    dest.y=c.y;
    source.y=0;
    source.w=w/10;
    source.h=h;
    switch (text[len]) {
      case '1': source.x=source.w; break;
      case '2': source.x=source.w*2; break;
      case '3': source.x=source.w*3; break;
      case '4': source.x=source.w*4; break;
      case '5': source.x=source.w*5; break;
      case '6': source.x=source.w*6; break;
      case '7': source.x=source.w*7; break;
      case '8': source.x=source.w*8; break;
      case '9': source.x=source.w*9; break;
      case '0': source.x=0; break;
    }
    SDL_RenderCopy(renderer,tDigits,&source,&dest);
  }
}

void render_block(Block *block, SDL_Renderer * renderer,int size){
  SDL_Texture * current;
  SDL_Rect rect;
  switch (block->type) {
    case border: current = tBorder; break;
    case empty: current = tEmpty; break;
    case player: current = tPlayer; break;
    case spider: current = tSpider; break;
    case diamond: current = tDiamond; break;
    case monster: current = tMonster; break;
    case rock: current = tRock; break;
    case dirt: current = tDirt; break;
    case water: current = tWater; break;
    case door: current = tDoor; break;
    default: current = tDirt; break;
  }
  rect=point_to_rect(block->pos,size);
  SDL_RenderCopy(renderer,current,NULL,&rect);
}

int max_limit(int a, int max){
  if(a>max) return max;
  return a;
}

void draw_roi(Level * map,SDL_Renderer * renderer,int w, int h){
  Block ** roi;
  char score[10];
  char time[10];
  Point text_pos;
  Point time_pos;
  text_pos.x=50;
  text_pos.y=10;
  time_pos.x=50;
  time_pos.y=50;
  int i,j;
  roi=take_roi(map,max_limit(w/BLOCK_SIZE,map->width),max_limit(h/BLOCK_SIZE,map->height));
  for(i=0;i<max_limit(h/BLOCK_SIZE,map->height);i++){
    for(j=0;j<max_limit(w/BLOCK_SIZE,map->width);j++){
      render_block(&roi[i][j],renderer,BLOCK_SIZE);
    }
  }
  free_roi(roi,max_limit(h/BLOCK_SIZE,map->height));
}
