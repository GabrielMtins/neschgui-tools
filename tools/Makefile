CC:=clang
INCLUDE_FLAGS:=-I../external/stb -I../core
CC_FLAGS:=-std='c99' -Wall -Wextra -pedantic -O2 -lm

NES2PNG_SRC:= nes2png.c
PNG2NES_SRC:= png2nes.c

UTIL_SRC:= util.h
ROM_TOOLS_SRC:=../core/rom_tools.h

all: nes2png png2nes

nes2png: $(NES2PNG_SRC) $(ROM_TOOLS_SRC) $(UTIL_SRC)
	$(CC) $(INCLUDE_FLAGS) $(CC_FLAGS) $(NES2PNG_SRC) -o nes2png

png2nes: $(PNG2NES_SRC) $(ROM_TOOLS_SRC) $(UTIL_SRC)
	$(CC) $(INCLUDE_FLAGS) $(CC_FLAGS) $(PNG2NES_SRC) -o png2nes

clean:
	rm -f nes2png png2nes
