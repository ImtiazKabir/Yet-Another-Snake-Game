default:
	echo try with ems or sdl

ems:
	emcc \
	-D USING_EMSCRIPTEN \
	-Wall -Wextra -Wconversion -Wpedantic \
	-O3 \
	-o dist/index.js \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-s SDL2_IMAGE_FORMATS='["png", "jpg"]' \
	--preload-file build/assets \
	src/*.c \
	-Iinclude -isystem lib

sdl:
	gcc \
	-Wall -Wextra -Wconversion -Wpedantic \
	-O3 \
	-o build/main.exe \
	src/*.c lib/whereami.c -Iinclude -isystem lib \
	-lSDL2 -lSDL2main -lSDL2_image

run:
	build/main.exe

