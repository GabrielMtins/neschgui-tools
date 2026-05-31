#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define TILE_SIZE 8
#define PALETTE_SIZE 16

typedef struct {
	uint8_t r, g, b, a;
} Color;

Color palette[PALETTE_SIZE] = {
	{0xff, 0xff, 0xff, 0xff},
	{0xc0, 0xc0, 0xc0, 0xff},
	{0x80, 0x80, 0x80, 0xff},
	{0x00, 0x00, 0x00, 0xff},
	{0xff, 0xff, 0xff, 0xff},
	{0xc0, 0xc0, 0xc0, 0xff},
	{0x80, 0x80, 0x80, 0xff},
	{0x00, 0x00, 0x00, 0xff},
	{0xff, 0xff, 0xff, 0xff},
	{0xc0, 0xc0, 0xc0, 0xff},
	{0x80, 0x80, 0x80, 0xff},
	{0x00, 0x00, 0x00, 0xff},
	{0xff, 0xff, 0xff, 0xff},
	{0xc0, 0xc0, 0xc0, 0xff},
	{0x80, 0x80, 0x80, 0xff},
	{0x00, 0x00, 0x00, 0xff},
};

int read_palette(const char *pal) {
	int n;
	size_t count = 0;
	size_t len = strlen(pal);
	unsigned int r, g, b;

	static const size_t chr_read = 7;

	for(int i = 0; i < PALETTE_SIZE; i++) {
		while(count < len && isspace((int) pal[count])) {
			count++;
		}

		if(count + chr_read > len) {
			break;
		}

		if(sscanf(pal + count, "#%2x%2x%2x%n", &r, &g, &b, &n) != 3) {
			break;
		}

		count += n;

		palette[i].r = (uint8_t ) r;
		palette[i].g = (uint8_t ) g;
		palette[i].b = (uint8_t ) b;
		palette[i].a = 0xff;
	}

	return 0;
}

int read_palette_file(const char *filepath) {
	FILE *file;
	unsigned int r, g, b;

	file = fopen(filepath, "r");

	if(file == NULL) {
		return -1;
	}

	for(int i = 0; i < PALETTE_SIZE; i++) {
		int res = fscanf(file, "#%2x%2x%2x ", &r, &g, &b);

		if(res == EOF) {
			break;
		}

		if(res != 3) {
			break;
		}

		palette[i].r = (uint8_t ) r;
		palette[i].g = (uint8_t ) g;
		palette[i].b = (uint8_t ) b;
		palette[i].a = 0xff;
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
