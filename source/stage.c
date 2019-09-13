
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "fundamentals.h"
#include "stage.h"
#include "mobs.h"
#include "render.h"
#include "blocks.h"
#include "textures.h"
#include "audio.h"

#define RUNNING 1
#define PAUSED 0
#define GAMEOVER -1
#define PASSED 2

#define SPIDER_SCORE 100
#define DIAMOND_SCORE 50
#define MONSTER_SCORE 200

typedef struct{
  int l,r,d,u;
}KeyStates;

void show_door(Level * level);
int player_update(Block ** map, Direction moveDir, Point * playerPos, int *diaCount, int * score);
int game_update(Level * level, Point playerPos, int * score);
void event_handler(SDL_Event *e, KeyStates * keys, int * winW, int * winH,SDL_Window *window, int * state);
Direction rock_fall_direction(Block **map, Point p);
void take_move_direction(KeyStates * keys, Direction * moveDir, int active);
void water_update(Level * level);

int game(Level * level, int * width, int * height, int health, int * score, SDL_Window * window, SDL_Renderer * renderer){
  /* init variables */
  Point playerPos=level->playerStartPos;
  Point doorPos = level->doorPos;
  Block ** map = level->blocks;

  /* sdl variables */
  SDL_Event e;

  //SDL_AudioSpec aud_wav_spec;
  SDL_AudioSpec want;
  SDL_AudioSpec have;
  SDL_AudioDeviceID device;
  Sound sound;

  /* game variables */
  KeyStates keys = {0,0,0,0};
  int diamonds = 0;
  int state = RUNNING;
  Direction moveDirection;
  int playerActive;

  //time fps
  const double updateStep = 0.15;
  const double animationStep = updateStep/1.5;
  Uint32 frameStart;
  Uint32 frameEnd;
  double timer = 0.0;
  double updateCounter = updateStep;
  double animationCounter = animationStep;
  double waterCounter = level->waterRate;
  double soundCounter;

  /* animation variables */
  int currentAnim = 0;
  Direction playerAnim = _none;

  /* coordinates */
  Point scorePos;
  Point timePos;
  Point diamondPos;
  SDL_Rect scoreRect;
  SDL_Rect timeRect;
  SDL_Rect diamondRect;
  SDL_Rect pauseSmall;
  SDL_Rect pauseBig;

  /* setting up audio and sound */
  init_sound(&sound);

  soundCounter = sound.speed;

  want.freq = sound.sampleFreq;
  want.format = AUDIO_S16SYS;
  want.channels = 1;
  want.samples = 2048;
  want.callback = synth_triangle_callback;
  want.userdata = &sound;

  device = SDL_OpenAudioDevice(NULL,0,&want, &have,SDL_AUDIO_ALLOW_ANY_CHANGE);

  if(want.format != have.format) SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to get the desired AudioSpec");
  if(device==0) printf("Audio Device Error\n");

  SDL_PauseAudioDevice(device,0);

  /* setting hud coordinates */
  scorePos.x=*width-125;
  scorePos.y=7;
  scoreRect=create_rect_xy(*width-135,3,130,24);

  timePos.x=*width-65;
  timePos.y=34;
  timeRect=create_rect_xy(*width-95,30,90,24);

  diamondPos.x=20;
  diamondPos.y=7;
  diamondRect=create_rect_xy(10,3,60,24);

  pauseSmall=create_rect_xy(*width/2-15,7,30,30);
  pauseBig=create_rect_xy(*width/2-50,*height/2-50,100,100);

  /* preparing map */
  set_block(map,playerPos,player);
  set_block(map,doorPos,border);


  while(state==PAUSED || state==RUNNING){

    frameStart = SDL_GetPerformanceCounter();

    /* Events */
    playerActive = map[playerPos.y][playerPos.x].active;
    event_handler(&e,&keys,width,height,window,&state);
    take_move_direction(&keys,&moveDirection,playerActive);

    if(state==RUNNING){
      // printf("Keys: l:%d r:%d u:%d d:%d\n", keys.l, keys.r, keys.u, keys.d);
      // printf("Move: %d\n", moveDirection);
      playerAnim=moveDirection;
    }

    /* Game Update */
    if(timer>updateCounter){

      if(state==RUNNING){
        state = game_update(level,playerPos,score);
        if(state==RUNNING)
          state = player_update(map,moveDirection,&playerPos,&diamonds,score);
        moveDirection=_none;
      }
      updateCounter+=updateStep;
      if(diamonds==level->diamondsRequired) show_door(level);
    }
    if(timer>level->time){
      state=GAMEOVER;
    }

    /* Water Update */
    if(timer>waterCounter){
      if(state==RUNNING) water_update(level);
      waterCounter+=level->waterRate;
    }

    /* Sound update */
    if(timer>level->time*0.9){
      sound.speed=get_speed(STOCK_BPM*1.6);
    }
    else if(timer>level->time*0.8){
      sound.speed=get_speed(STOCK_BPM*1.3);
    }
    else if(timer>level->time*0.7){
      sound.speed=get_speed(STOCK_BPM*1.1);
    }
    sound_update(&sound,timer,&soundCounter);
    if(state==PAUSED) sound.samplesPerSine=0;

    /* Animations Update */
    if(timer>animationCounter){
      currentAnim ++;
      if(currentAnim>3) currentAnim=0;
      animationCounter+=animationStep;
    }

    /* Screen Update */

    SDL_RenderClear(renderer);
    draw_roi(level,renderer,*width,*height,currentAnim,playerAnim,playerPos);
    SDL_RenderCopy(renderer,tPaused,NULL,&pauseSmall);
    SDL_RenderCopy(renderer,tBack,NULL,&scoreRect);
    render_number(renderer,*score,scorePos,18,6);
    SDL_RenderCopy(renderer,tBack,NULL,&timeRect);
    render_number(renderer,(int)level->time-timer,timePos,18,3);
    SDL_RenderCopy(renderer,tBack,NULL,&diamondRect);
    render_number(renderer,level->diamondsRequired-diamonds>0?level->diamondsRequired-diamonds:0,diamondPos,18,2);
    if(state==PAUSED) SDL_RenderCopy(renderer,tPaused,NULL,&pauseBig);
    SDL_RenderPresent(renderer);

    if(state==GAMEOVER) SDL_Delay(1000);

    /* time management */
    if(state==RUNNING){
      frameEnd = SDL_GetPerformanceCounter();
      timer += (double)(frameEnd - frameStart)/(double)SDL_GetPerformanceFrequency();
    }
  }
  SDL_CloseAudioDevice(device);
  if(state==GAMEOVER) return -1;
  return 0;
}
int game_update(Level * level,Point playerPos, int * score){
  int i;
  Point *rocks = NULL;
  Point *spiders = NULL;
  Point *monsters = NULL;
  Point *diamonds = NULL;
  Block ** map = level->blocks;
  int mapW = level->width;
  int mapH = level->height;
  int count=0;

  /* Spiders' Update */
  find_blocks(map, &spiders, &count, spider, mapW,mapH);
  for (i = 0; i < count; i++){
    if(spider_move(map, spiders[i])==-1) return GAMEOVER;
  }
  if(spiders!=NULL) free(spiders);


  /* Monsters' Update */
  find_blocks(map, &monsters, &count, monster, mapW,mapH);
  for (i = 0; i < count; i++){
    if(monster_move(map, monsters[i],playerPos)==-1) return GAMEOVER;
  }
  if(monsters!=NULL) free(monsters);

  /* Rocks' Update */
  find_blocks(map, &rocks, &count, rock, mapW,mapH);
  for (i = 0; i < count; i++){
    if (map[rocks[i].y][rocks[i].x].active == 1){
      if(rock_fall_direction(map,rocks[i])==_none){
        deactivate(level->blocks,rocks[i]);
      }
      else if(rock_fall_direction(map,rocks[i])==_down){
        switch (get_side(map,rocks[i],_down)){
          case empty:
            block_move(map,rocks[i],_down);
            break;
          case player:
            block_move(map,rocks[i],_down);
            return GAMEOVER;
          case spider:
            block_move(map,rocks[i],_down);
            create_diamonds(map,rocks[i],spider,mapW,mapH);
            *score += SPIDER_SCORE;
            break;
          case monster:
            block_move(map,rocks[i],_down);
            create_diamonds(map,rocks[i],monster,mapW,mapH);
            *score += MONSTER_SCORE;
            break;
          default: break;
        }
      }
      else{
        block_move(map,rocks[i],rock_fall_direction(map,rocks[i]));
      }
    }
    else{
      if (rock_fall_direction(map, rocks[i]) != _none)
        if (get_side(map, rocks[i], _down) != player)
          activate(level->blocks,rocks[i]);
    }
  }
  if(rocks!=NULL) free(rocks);

  /* Diamonds' Update */
  find_blocks(map, &diamonds, &count, diamond, mapW,mapH);
  for (i = 0; i < count; i++){
    if (get_side(map, diamonds[i], _down) == empty){
      block_move(map, diamonds[i], _down);
    }
  }
  if(diamonds!=NULL) free(diamonds);

  return RUNNING;
}
Direction rock_fall_direction(Block **map, Point p){
  SDL_Point np;
  np = p;
  np.y += 1;
  switch (get_side(map, p, _down)){
  case empty:
    return _down;
  case player:
    return _down;
  case monster:
    return _down;
  case spider:
    return _down;
  case rock:
    if ((get_side(map, np, _right) == empty && get_side(map, p, _right) == empty) ||
        (get_side(map, np, _right) == player && get_side(map, p, _right) == player)||
        (get_side(map, np, _right) == monster && get_side(map, p, _right) == monster)||
        (get_side(map, np, _right) == spider && get_side(map, p, _right) == spider))
      return _right;
    else if ((get_side(map, np, _left) == empty && get_side(map, p, _left) == empty) ||
        (get_side(map, np, _left) == player && get_side(map, p, _left) == player)||
        (get_side(map, np, _left) == monster && get_side(map, p, _left) == monster)||
        (get_side(map, np, _left) == spider && get_side(map, p, _left) == spider))
      return _left;
    else
      return _none;
  default:
    return _none;
  }
}
int player_update(Block ** map, Direction moveDir, Point * playerPos, int *diaCount, int * score){
  Point newPos = side_pos(*playerPos,moveDir);
  if(moveDir==_none){
    deactivate(map,*playerPos);
    return RUNNING;
  }
  switch (get_block(map,newPos)) {
    case diamond:
      set_block(map,newPos,player);
      set_block(map,*playerPos,empty);
      *playerPos=newPos;
      activate(map,newPos);
      (*diaCount)+=1;
      *score += DIAMOND_SCORE;
      return RUNNING;
    case empty:
      set_block(map,newPos,player);
      set_block(map,*playerPos,empty);
      *playerPos=newPos;
      activate(map,newPos);
      return RUNNING;
    case dirt:
      set_block(map,newPos,player);
      set_block(map,*playerPos,empty);
      *playerPos=newPos;
      activate(map,newPos);
      return RUNNING;
    case rock:
      if(moveDir==_left || moveDir==_right){
        if(get_side(map,newPos,moveDir)==empty){
          activate(map,newPos);
          block_move(map,newPos,moveDir);
          set_block(map,newPos,player);
          set_block(map,*playerPos,empty);
          activate(map,newPos);
          *playerPos=newPos;
          return RUNNING;
        }
        return RUNNING;
      }
      return RUNNING;
    case monster:
      set_block(map,*playerPos,empty);
      deactivate(map,*playerPos);
      return GAMEOVER;
    case spider:
      set_block(map,*playerPos,empty);
      deactivate(map,*playerPos);
      return GAMEOVER;
    case door:
      set_block(map,*playerPos,empty);
      set_block(map,newPos,player);
      deactivate(map,*playerPos);
      return PASSED;
    default:
      deactivate(map,*playerPos);
      return RUNNING;
  }
}
void event_handler(SDL_Event *e, KeyStates * keys, int * winW, int * winH,SDL_Window *window, int * state){
  Point mousePos;
  if ( SDL_PollEvent( e )){
    switch (e->type){
      case SDL_KEYDOWN:
        switch(e->key.keysym.sym){
          case SDLK_LEFT: keys->l = 1;break;
          case SDLK_RIGHT: keys->r = 1;break;
          case SDLK_UP: keys->u = 1;break;
          case SDLK_DOWN: keys->d = 1;break;
          case SDLK_p:
            if(*state==RUNNING) *state=PAUSED;
            else *state=RUNNING;
            break;
        }
        break;
      case SDL_KEYUP:
        switch(e->key.keysym.sym){
          case SDLK_LEFT: if(keys->l==1) keys->l = 0;break;
          case SDLK_RIGHT: if(keys->r==1) keys->r = 0;break;
          case SDLK_UP: if(keys->u==1) keys->u = 0;break;
          case SDLK_DOWN: if(keys->d==1) keys->d = 0;break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        if(mousePos.y>0 && mousePos.y<45 && mousePos.x>*winW/2-20 && mousePos.x<*winW/2+20){
          if(*state==RUNNING) *state=PAUSED;
          else *state=RUNNING;
        }
        else if(mousePos.y>*winH/2-50 && mousePos.y<*winH/2+50 && mousePos.x>*winW/2-50 && mousePos.x<*winW/2+50)
          *state=RUNNING;
        break;
      case SDL_WINDOWEVENT:
        if(e->window.event==SDL_WINDOWEVENT_FOCUS_LOST)
          *state=PAUSED;
        break;
      case SDL_QUIT: exit(0); break;
    }
  }
  SDL_GetWindowSize(window,winW,winH);
}
void take_move_direction(KeyStates * keys, Direction * moveDir, int active){
  if(keys->u == keys->d){
    if(keys->l == keys->r){
      if(active) *moveDir=_none;
      return;
    }
    else if(keys->l == 1) *moveDir=_left;
    else *moveDir=_right;
  }
  else if(keys->u ==1) *moveDir=_up;
  else *moveDir=_down;
}
void show_door(Level * level){
  level->blocks[level->doorPos.y][level->doorPos.x].type=door;
}
void water_update(Level * level){
  int count,i;
  Point * waters = NULL;
  find_blocks(level->blocks, &waters, &count, water, level->width, level->height);
  for(i=0;i<count;i++){
    if(get_side(level->blocks,waters[i],_left)==empty) set_block(level->blocks,side_pos(waters[i],_left),water);
    if(get_side(level->blocks,waters[i],_right)==empty) set_block(level->blocks,side_pos(waters[i],_right),water);
    if(get_side(level->blocks,waters[i],_up)==empty) set_block(level->blocks,side_pos(waters[i],_up),water);
    if(get_side(level->blocks,waters[i],_down)==empty) set_block(level->blocks,side_pos(waters[i],_down),water);
  }
  if(waters!=NULL) free(waters);
}
