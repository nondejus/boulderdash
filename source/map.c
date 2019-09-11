
#include <SDL2/SDL.h>
#include <stdio.h>
#include "map.h"
#include "fundamentals.h"
#include "blocks.h"
int find_player(Level * level, Point * p);
Level init_map();

Level level_picker(int i){
  char name[64];
  sprintf(name,"level%d.bin",i);
  return load_from_file(name);
  switch (i) {
    case 1: return init_map();
    //case ..
    default: return init_map();
  }
}

Level load_from_file(char * file_name){
  int i;
  FILE * f;
  Level level;
  f = fopen(file_name,"r");
  if(f==NULL) printf("Cannot open %s\n", file_name);
  fread(&level.width,sizeof(int),1,f);
  fread(&level.height,sizeof(int),1,f);
  fread(&level.diamondsRequired,sizeof(int),1,f);
  fread(&level.time,sizeof(int),1,f);
  fread(&level.waterRate,sizeof(double),1,f);
  fread(&level.playerStartPos,sizeof(Point),1,f);
  fread(&level.doorPos,sizeof(Point),1,f);

  level.blocks=ALLOC(Block*,level.height);
  for(i=0;i<level.height;i++){
    level.blocks[i]=ALLOC(Block,level.width);
  }
  for(i=0;i<level.height;i++){
    fread(level.blocks[i], sizeof(Block),level.width,f);
  }
  fclose(f);
  return level;
}


Level init_map(){
  int i,j;
  Level map;
  Point pla;
  pla.y=2;
  pla.x=15;
  map.playerStartPos=pla;
  map.width=30;
  map.height=20;
  map.blocks=ALLOC(Block*,map.height);
  for(i=0;i<map.height;i++){
    map.blocks[i]=ALLOC(Block,map.width);
  }
  for(i=0;i<map.height;i++){
    for(j=0;j<map.width;j++){
      map.blocks[i][j].active=0;
      map.blocks[i][j].type=empty;
      map.blocks[i][j].pos.x=j;
      map.blocks[i][j].pos.y=i;
      if(i==0||j==0||i==19||j==29){
        map.blocks[i][j].type=border;
      }
      else if(i==2||j==3){
        map.blocks[i][j].type=diamond;
      }
      else if(i==1||j==1||i==4||j==5||i==6||j==7||j==9){
        map.blocks[i][j].type=dirt;
      }
      else if(i==16||j==15||i==13||j==17){
        map.blocks[i][j].type=rock;
      }
    }
  }
  map.blocks[8][11].type=rock;
  map.blocks[12][11].type=empty;
  map.blocks[13][1].type=spider;
  map.blocks[5][3].type=monster;
  return map;
}

int find_player(Level * level, Point * p){
  int i,j;
  for(i=0;i<level->height;i++){
    for(j=0;j<level->width;j++){
      if(level->blocks[i][j].type==player){
        p->x=j;
        p->y=i;
        return 0;
      }
    }
  }
  return -1;
}

Block ** take_roi(Level * map, int width, int height){
  Block ** roi;
  int i,j;
  SDL_Point p;
  int start_i,start_j;
  roi=ALLOC(Block*,height);
  for(i=0;i<height;i++){
    roi[i]=ALLOC(Block,width);
  }
  if(find_player(map,&p)!=-1){
    for(i=0;i<height;i++){
      for(j=0;j<width;j++){

        if(p.y<=height/2){
          start_i=0;
        }
        else if(p.y>=(map->height-height/2)){
          start_i=map->height-height;
        }
        else{
          start_i=p.y-height/2;
        }
        if(p.x<=width/2){
          start_j=0;
        }
        else if(p.x>=(map->width-width/2)){
          start_j=map->width-width;
        }
        else{
          start_j=p.x-width/2;
        }
        roi[i][j]=map->blocks[start_i+i][start_j+j];
        fflush(stdout);
        roi[i][j].pos.x-=start_j;
        roi[i][j].pos.y-=start_i;
      }
    }
  }
  return roi;
}

void free_roi(Block ** roi,int h){
  int i;
  for(i=0;i<h;i++){
    free(roi[i]);
  }
  free(roi);
}
