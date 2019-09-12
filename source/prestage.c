#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include "prestage.h"
#include "render.h"
#include "textures.h"
void prestage(SDL_Window * window, SDL_Renderer * renderer,int score, int level, int health){
  Point scorePos, levelPos;
  SDL_Rect scoreRect, levelRect, heartRect;
  SDL_Event e;
  int winW,winH,i;
  int counter = 0;
  SDL_GetWindowSize(window,&winW,&winH);
  while (counter<100) {
    if ( SDL_PollEvent( &e )){
      switch (e.type){
        case SDL_QUIT: exit(0); break;
      }
      SDL_GetWindowSize(window,&winW,&winH);
    }

    scorePos.x=winW-125;
    scorePos.y=7;
    scoreRect=create_rect_xy(winW-135,3,130,24);


    levelPos.x=winW/2+60;
    levelPos.y=winH/2-10;
    levelRect = create_rect_xy(levelPos.x-220,levelPos.y,200,40);

    if(theme==1) SDL_SetRenderDrawColor(renderer,66,64,255,255);
    else SDL_SetRenderDrawColor(renderer,30,35,38,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    for(i=1;i<=health;i++){
      heartRect=create_rect_xy(winW-7-20*i,34,20,20);
      SDL_RenderCopy(renderer,tArrow,NULL,&heartRect);
    }

    SDL_RenderCopy(renderer,tBack,NULL,&scoreRect);
    SDL_RenderCopy(renderer,tLevel,NULL,&levelRect);

    render_number(renderer,score,scorePos,18,6);
    render_number(renderer,level,levelPos,36,2);

    SDL_RenderPresent(renderer);
    counter++;
  }
}
