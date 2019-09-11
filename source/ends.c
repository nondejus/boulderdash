#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include "ends.h"
#include "render.h"
#include "textures.h"

void gameover(SDL_Window * window, SDL_Renderer * renderer,int score, int status){
  Point scorePos;
  SDL_Rect scoreRect, bgRect, shadowRect, gameoverRect;
  SDL_Event e;
  int winW,winH;
  int counter = 0;
  SDL_GetWindowSize(window,&winW,&winH);
  while (counter<200) {
    if ( SDL_PollEvent( &e )){
      switch (e.type){
        case SDL_QUIT: exit(0); break;
      }
      SDL_GetWindowSize(window,&winW,&winH);
    }

    scorePos.x=winW/2-55;
    scorePos.y=winH/2+50;
    scoreRect=create_rect_xy(scorePos.x-10,scorePos.y-4,130,24);

    bgRect=create_rect_xy(winW/2-150,winH/2-100,300,200);
    shadowRect=create_rect_xy(winW/2-146,winH/2-96,300,200);
    gameoverRect=create_rect_xy(winW/2-100,winH/2-85,200,100);

    if(status<0) SDL_SetRenderDrawColor(renderer,255,0,0,255);
    else SDL_SetRenderDrawColor(renderer,0,255,0,255);

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&shadowRect);
    if(theme==1) SDL_SetRenderDrawColor(renderer,66,64,255,255);
    else SDL_SetRenderDrawColor(renderer,30,35,38,255);
    SDL_RenderFillRect(renderer,&bgRect);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    SDL_RenderCopy(renderer,tBack,NULL,&scoreRect);
    SDL_RenderCopy(renderer,tGameover,NULL,&gameoverRect);

    render_number(renderer,score,scorePos,18,6);

    SDL_RenderPresent(renderer);
    counter++;
  }
}
