CC:=gcc
INCLUDE_FLAGS:=-I./external/stb -I./rom_tools
CC_FLAGS:=-std='c99' -Wall -Wextra -pedantic -O2

NES2PNG_SRC:= rom_tools/nes2png.c

all: nes2png

nes2png: $(NES2PNG_SRC)
	$(CC) $(INCLUDE_FLAGS) $(CC_FLAGS) $(NES2PNG_SRC) -o nes2png

clean:
	rm -f nes2png
