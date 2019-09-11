CC=gcc
CFLAGS=`sdl/sdl2-config --cflags --libs` -g -lm -Wall
MAIN_SOURCE=source/main.c source/blocks.c source/intro.c source/mobs.c source/prestage.c source/render.c source/stage.c source/textures.c source/map.c source/audio.c source/ends.c
LEVEL_SOURCE=source/level-editor/main.c source/textures.c
main: source/main.c
	$(CC) -o boulder-dash $(MAIN_SOURCE) $(CFLAGS)

level: source/level-editor/main.c
	$(CC) -o level-editor $(LEVEL_SOURCE) $(CFLAGS)
