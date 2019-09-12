#include <SDL2/SDL.h>
#include "intro.h"
#include "textures.h"
#include "render.h"
void intro(SDL_Window * window,SDL_Renderer * renderer){
  int running = 1;
  int choice = 1;
  int winW,winH;
  SDL_Event e;
  SDL_Rect rect;
  while(running){
    if ( SDL_PollEvent( &e )){
      SDL_GetWindowSize(window,&winW,&winH);
      switch (e.type){
        case SDL_QUIT: exit(0); break;
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym){
            case SDLK_RETURN:
              if(choice == 1) return;
              else{
                theme = 3 - theme;
                free_textures();
                load_textures(renderer);
              }
              break;
            case SDLK_RETURN2:
              if(choice == 1) return;
              else{
                theme = 3 - theme;
                load_textures(renderer);
              }
              break;
            case SDLK_SPACE:
              if(choice == 1) return;
              else{
                theme = 3 - theme;
                load_textures(renderer);
              }
              break;
            case SDLK_DOWN:
              choice=1-choice;
              break;
            case SDLK_UP:
              choice=1-choice;
              break;
          }
      }
    }

    SDL_RenderClear(renderer);
    rect = create_rect_xy(0,0,winW,winH);
    SDL_RenderCopy(renderer,tIntro,NULL,&rect);

    if(choice==1){
      rect = create_rect_xy(winW*2.0/5.0,winH*3.0/5.0-winH/50,winW/25,winH/25);
    }
    else{
      rect = create_rect_xy(winW*2.0/5.0,winH*2.0/3.0-winH/50,winW/25,winH/25);
    }
    SDL_RenderCopy(renderer,tArrow,NULL,&rect);
    SDL_RenderPresent(renderer);
  }
}
