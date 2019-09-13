CC=gcc
CFLAGS=-g -Wall -lm
MAIN_SOURCE=source/main.c source/blocks.c source/intro.c source/mobs.c source/prestage.c source/render.c source/stage.c source/textures.c source/map.c source/audio.c source/ends.c
LIB_FILES=sdl/libSDL2.so
LEVEL_SOURCE=source/level-editor/main.c source/textures.c
MAIN_DEST=build
LEVEL_DEST=levels
MAIN_SH=run-game.sh
LEVEL_SH=run-level.sh
MAIN_OUT=boulder-dash
LEVEL_OUT=level-editor

main: source/main.c
	$(CC) -o $(MAIN_DEST)/$(MAIN_OUT) $(MAIN_SOURCE) $(LIB_FILES) $(CFLAGS)
	@echo "export LD_LIBRARY_PATH=./sdl" > $(MAIN_SH)
	@echo "$(MAIN_DEST)/$(MAIN_OUT)" >> $(MAIN_SH)
	chmod +x $(MAIN_SH)

level: source/level-editor/main.c
	$(CC) -o ${LEVEL_DEST}/$(LEVEL_OUT) $(LEVEL_SOURCE) $(LIB_FILES) $(CFLAGS)
	@echo "export LD_LIBRARY_PATH=./sdl" > $(LEVEL_SH)
	@echo "" >> $(LEVEL_SH)
	@echo "$(LEVEL_DEST)/$(LEVEL_OUT)" >> $(LEVEL_SH)
	chmod +x $(LEVEL_SH)

$(MAIN_SH):	touch $(MAIN_SH)
$(LEVEL_SH): touch $(LEVEL_SH)
$(shell mkdir -p $(MAIN_DEST))
