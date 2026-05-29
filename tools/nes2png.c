#include <stdio.h>
#include <stdlib.h>

#include "rom_tools.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define TILE_SIZE 8

uint32_t palette[16] = {
	0xFFFFFFFF,
	0xFFC0C0C0,
	0xFF808080,
	0XFF000000
};

int main(int argc, char **argv) {
	FILE *in = stdin, *out = stdout;
	size_t rom_size;
	void *data;
	Rom_Viewer viewer;
	int num_tiles_row = 16;
	int num_tiles_column = 0;
	int width, height;
	uint32_t *img;

	in = fopen("smb_og.nes", "rb");

	fseek(in, 0, SEEK_END);
	rom_size = ftell(in);
	fseek(in, 0, SEEK_SET);

	if(rom_size == 0) {
		return -1;
	}

	data = malloc(rom_size);

	fread(data, 1, rom_size, in);

	viewer = Rom_CreateViewer(ROM_TYPE_NES, data, rom_size);

	num_tiles_column = viewer.num_tiles / num_tiles_row;
	img = (uint32_t *) malloc(TILE_SIZE * TILE_SIZE * sizeof(uint32_t) * viewer.num_tiles);

	width = num_tiles_row * TILE_SIZE;
	height = num_tiles_column * TILE_SIZE;

	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			size_t tile_id = (i / TILE_SIZE) + (j / TILE_SIZE) * num_tiles_row;
			uint8_t color_id = Rom_GetTilePixelColor(&viewer, tile_id, i % TILE_SIZE, j % TILE_SIZE);

			img[i + j * width] = palette[color_id];
		}
	}

	stbi_write_png("out.png", width, height, 4, img, width * 4);

	free(img);
	free(data);

	if(in != stdin) fclose(in);
	if(out != stdout) fclose(out);

	return 0;
}
