#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TILE_SIZE 8
#define PALETTE_SIZE 16

uint32_t palette[PALETTE_SIZE] = {
	0xFFFFFFFF,
	0xFFC0C0C0,
	0xFF808080,
	0XFF000000
};

int read_palette(const char *pal) {
	int count = 0, n;

	for(int i = 0; i < PALETTE_SIZE; i++) {
		if(sscanf(pal + count, "#%x%n", palette + i, &n) == 0) {
			break;
		}
		
		count += n;

		palette[i] |= 0xFF000000;
	}

	return 0;
}

int read_palette_file(const char *filepath) {
	FILE *file;

	file = fopen(filepath, "r");

	if(file == NULL) {
		return -1;
	}

	for(int i = 0; i < PALETTE_SIZE; i++) {
		if(feof(file)) {
			break;
		}

		if(fscanf(file, "#%x ", &palette[i]) != 1) {
			fclose(file);
			return -1;
		}

		palette[i] |= 0xFF000000;
	}

	fclose(file);

	return 0;
}

void stdout_write(void *context, void *data, int size) {
	(void) context;

	fwrite(data, 1, size, stdout);
}

#define CHUNK_SIZE 4096

void * stdin_read(size_t *size) {
	uint8_t *buffer = NULL;
	size_t total = 0;
	size_t now_read = 0;
	size_t capacity = 4096;

	buffer = (uint8_t *) malloc(capacity);

	uint8_t chunk[CHUNK_SIZE];

	while((now_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
		if(total + now_read > capacity) {
			capacity *= 2;
			buffer = realloc(buffer, capacity);
		}

		memcpy(buffer + total, chunk, now_read);
		
		total += now_read;
	}

	*size = total;

	return (void *) buffer;
}
