#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rom_tools.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int get_color_id(uint32_t color) {
	for(int i = 0; i < PALETTE_SIZE; i++) {
		if(color == palette[i]) {
			return i;
		}
	}

	return -1;
}

int main(int argc, char **argv) {
	uint32_t *raw_img;
	uint32_t *img;
	void *data;
	size_t data_size;
	size_t img_size;
	int width, height;
	size_t num_tiles_total;
	int num_tiles_row = 16;
	int num_tiles_column = 0;
	Rom_Format format = ROM_TYPE_NES;
	Rom_Viewer viewer;

	raw_img = stdin_read(&img_size);

	img = (uint32_t *) stbi_load_from_memory(
			(const unsigned char *) raw_img, 
			img_size,
			&width,
			&height,
			NULL,
			4
			);

	num_tiles_total = width * height / (TILE_SIZE * TILE_SIZE);

	fprintf(stderr, "%lu\n", num_tiles_total);

	data_size = Rom_GetRomSizeByTiles(format, num_tiles_total);

	data = malloc(data_size);

	viewer = Rom_CreateViewer(format, data, data_size);

	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			int tile_id = (i / TILE_SIZE) + (j / TILE_SIZE) * num_tiles_row;

			int color = get_color_id(img[i + j * width]);

			if(color < -1) {
				fprintf(stderr, "%s: Image contains a color not in the palette.", argv[0]);
				return -1;
			}

			Rom_SetTilePixelColor(
					&viewer,
					tile_id,
					i % TILE_SIZE,
					j % TILE_SIZE,
					color
					);
		}
	}

	fwrite(data, 1, data_size, stdout);

	free(data);
	free(raw_img);
	stbi_image_free(img);
}
