#include <SDL2/SDL.h>
#include "fundamentals.h"
#include "blocks.h"

void activate(Block ** map, Point pos){
  map[pos.y][pos.x].active=1;
}
void deactivate(Block ** map, Point pos){
  map[pos.y][pos.x].active=0;
}
void set_block(Block ** map, Point pos, block_t block){
  map[pos.y][pos.x].type=block;
}
block_t get_block(Block ** map, Point pos){
  return map[pos.y][pos.x].type;
}
void set_block_xy(Block ** map, int x, int y, block_t block){
  map[y][x].type=block;
}
block_t get_block_xy(Block ** map, int x, int y){
  return map[y][x].type;
}
block_t get_side(Block ** map, Point p, Direction d){
  p = side_pos(p,d);
  return get_block(map,p);
}
Point side_pos(Point p, Direction d){
  switch (d){
    case _left:  p.x-=1; break;
    case _right: p.x+=1; break;
    case _up:    p.y-=1; break;
    case _down:  p.y+=1; break;
    default: break;
  }
  return p;
}
int block_move(Block ** map, Point p, Direction d){
  Point p2=side_pos(p,d);
  if(d==_none) return -1;
  set_block(map,p2,get_block(map,p));
  map[p2.y][p2.x].active=map[p.y][p.x].active;
  map[p.y][p.x].active=0;
  set_block(map,p,empty);
  return 1;
}
void find_blocks(Block ** map, Point ** blocks,int * size,block_t type, int mapW, int mapH){
  int i,j;
  int counter=0;
  for(i=0;i<mapH;i++){
    for(j=0;j<mapW;j++){
      if(get_block_xy(map,j,i)==type){
        if(counter==0)(*blocks)=ALLOC(SDL_Point,1);
        else (*blocks)=(Point*)realloc((*blocks),sizeof(SDL_Point)*(counter+1));
        (*blocks)[counter].x=j;
        (*blocks)[counter].y=i;
        counter++;
      }
    }
  }
  *size=counter;
}
