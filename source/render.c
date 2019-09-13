
#include "../sdl/include/SDL.h"
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

void render_number(SDL_Renderer * renderer, int num, Point c, int size, int len){
  int i,w,h;
  SDL_Rect source,dest;
  char text[100];
  SDL_QueryTexture(tDigits,NULL,NULL,&w,&h);
  sprintf(text,"%0*d",len,num);
  for(i=0;i<len;i++){
    dest.h=size;
    dest.w=size;
    dest.x=c.x+i*size;
    dest.y=c.y;
    source.y=0;
    source.w=w/10;
    source.h=h;
    switch (text[i]) {
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

void render_block(Block *block, SDL_Renderer * renderer,int size, int anim, Direction p){
  SDL_Texture * current = NULL;
  SDL_Rect source,dest;
  int w,h;
  switch (block->type) {
    case border:
     current = tBorder; break;
    case empty:
     current = tEmpty; break;
    case player:
     current = tPlayer; break;
    case spider:
     current = tSpider; break;
    case diamond:
     current = tDiamond; break;
    case monster:
     current = tMonster; break;
    case rock:
     current = tRock; break;
    case dirt:
     current = tDirt; break;
    case water:
     current = tWater; break;
    case door:
     current = tDoor; break;
    default: current = tDirt; break;
  }
  dest=point_to_rect(block->pos,size);
  if(block->type==rock && block->active==0) current=tRock2;
  if(block->type==door || block->type==monster || block->type==rock || block->type==spider || block->type==water || block->type==diamond){
    SDL_QueryTexture(current,NULL,NULL,&w,&h);
    source.w=w/4;
    source.h=h;
    source.y=0;
    switch (anim) {
      case 0: source.x=0; break;
      case 1: source.x=source.w; break;
      case 2: source.x=source.w*2; break;
      case 3: source.x=source.w*3; break;
      default: break;
    }
    SDL_RenderCopy(renderer,current,&source,&dest);
  }
  else if(block->type==player){
    SDL_QueryTexture(current,NULL,NULL,&w,&h);
    source.w=w/4;
    source.h=h/4;
    switch (anim) {
      case 0: source.x=0; break;
      case 1: source.x=source.w; break;
      case 2: source.x=source.w*2; break;
      case 3: source.x=source.w*3; break;
      default: break;
    }
    switch (p) {
      case _left: source.y=0; break;
      case _right: source.y=source.h; break;
      case _down: source.y=source.h*2; break;
      case _up: source.y=source.h*3; break;
      case _none:
        current=tPlayer2;
        SDL_RenderCopy(renderer,current,NULL,&dest);
        return;
    }
    SDL_RenderCopy(renderer,current,&source,&dest);
  }
  else SDL_RenderCopy(renderer,current,NULL,&dest);
}

int max_limit(int a, int max){
  if(a>max) return max;
  return a;
}

void draw_roi(Level * map,SDL_Renderer * renderer,int w, int h, int anim, Direction p, Point pPos){
  Block ** roi = NULL;
  int i,j;
  roi=take_roi(map,max_limit(w/BLOCK_SIZE,map->width),max_limit(h/BLOCK_SIZE,map->height),pPos);
  for(i=0;i<max_limit(h/BLOCK_SIZE,map->height);i++){
    for(j=0;j<max_limit(w/BLOCK_SIZE,map->width);j++){
      render_block(&roi[i][j],renderer,BLOCK_SIZE,anim,p);
    }
  }
  free_roi(roi,max_limit(h/BLOCK_SIZE,map->height));
}
