#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rom_tools.h"
#include "util.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void usage(const char *prog) {
	fprintf(
			stderr,
			"Usage: %s [OPTIONS]",
			prog
		   );
}

int main(int argc, char **argv) {
	size_t rom_size;
	void *data;
	Rom_Viewer viewer;
	int num_tiles_row = 16;
	int num_tiles_column = 0;
	int width, height;
	uint32_t *img;
	Rom_Format format = ROM_TYPE_NES;

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
		} else if(!strcmp(argv[i], "--num-tiles-row")){
			if(sscanf(argv[i + 1], "%d", &num_tiles_row) != 1) {
				fprintf(stderr, "%s: Failed to load variable num-tiles-row.\n", argv[0]);
				return -1;
			} 
		} else {
			usage(argv[0]);
			return -1;
		}
	}

	data = stdin_read(&rom_size);

	viewer = Rom_CreateViewer(format, data, rom_size);

	fprintf(stderr, "%lu\n", viewer.num_tiles);
	
	num_tiles_column = viewer.num_tiles / num_tiles_row + (viewer.num_tiles % num_tiles_row != 0);
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

	stbi_write_png_to_func(
			stdout_write,
			NULL,
			width,
			height,
			4,
			img, width * 4
			);

	free(img);
	free(data);

	return 0;
}
