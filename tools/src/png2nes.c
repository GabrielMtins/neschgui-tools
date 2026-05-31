#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rom_tools.h"
#include "util.h"

#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int get_color_id(Color color) {
	for(int i = 0; i < PALETTE_SIZE; i++) {
		if(memcmp(&color, palette + i, sizeof(Color)) == 0) {
			return i;
		}
	}

	return -1;
}

void usage(const char *prog) {
	fprintf(
			stderr,
			"Usage: %s [OPTIONS]\n",
			prog
		   );
}

int main(int argc, char **argv) {
	uint32_t *raw_img;
	Color *img;
	void *data;
	size_t data_size;
	size_t img_size;
	int width, height;
	int num_tiles_total = 0;
	int num_tiles_row = 16;
	Rom_Format format = ROM_TYPE_NES;
	Rom_Viewer viewer;

	for(int i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "--format-nes")) {
			format = ROM_TYPE_NES;
		} else if(!strcmp(argv[i], "--format-gb")) {
			format = ROM_TYPE_GB;
		} else if(i == argc - 1) {
			continue;
		} else if(!strcmp(argv[i], "--palette")) {
			read_palette(argv[i + 1]);
			i++;
		} else if(!strcmp(argv[i], "--palette-file")) {
			if(read_palette_file(argv[i + 1]) != 0) {
				fprintf(stderr, "%s: Failed to read palette file: %s\n", argv[0], argv[i + 1]);
				return -1;
			}

			i++;
		} else if(!strcmp(argv[i], "--num-tiles-total")) {
			if(sscanf(argv[i + 1], "%d", &num_tiles_total) != 1) {
				fprintf(stderr, "%s: Failed to num-tiles-total\n", argv[0]);
				return -1;
			}

			i++;
		} else {
			usage(argv[0]);
			return -1;
		}
	}

	raw_img = stdin_read(&img_size);

	img = (Color *) stbi_load_from_memory(
			(const unsigned char *) raw_img, 
			img_size,
			&width,
			&height,
			NULL,
			4
			);

	num_tiles_row = width / TILE_SIZE;

	if(num_tiles_total == 0) {
		num_tiles_total = num_tiles_row * (height / TILE_SIZE);
	}

	data_size = Rom_GetRomSizeByTiles(format, num_tiles_total);

	data = malloc(data_size);

	viewer = Rom_CreateViewer(format, data, data_size);

	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			int tile_id = (i / TILE_SIZE) + (j / TILE_SIZE) * num_tiles_row;

			int color = get_color_id(img[i + j * width]);

			if(color < 0) {
				fprintf(stderr, "%s: Image contains a color not in the palette.\n", argv[0]);
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
