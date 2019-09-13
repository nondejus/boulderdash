#ifndef TEXTURES_H
#define TEXTURES_H

#include "../sdl/include/SDL.h"

int theme;

SDL_Texture * tSave;
SDL_Texture * tEmpty;
SDL_Texture * tBorder;
SDL_Texture * tDiamond;
SDL_Texture * tDirt;
SDL_Texture * tMonster;
SDL_Texture * tPlayer;
SDL_Texture * tRock;
SDL_Texture * tRock2;
SDL_Texture * tSpider;
SDL_Texture * tWater;
SDL_Texture * tDoor;
SDL_Texture * tDigits;
SDL_Texture * tScore;
SDL_Texture * tGameover;
SDL_Texture * tStart;
SDL_Texture * tHeart;
SDL_Texture * tPaused;
SDL_Texture * tLevel;
SDL_Texture * tClock;
SDL_Texture * tArrow;
SDL_Texture * tIntro;
SDL_Texture * tBack;
SDL_Texture * tWin;
SDL_Texture * tPlayer2;
void load_textures(SDL_Renderer * renderer);
void free_textures();
#endif
