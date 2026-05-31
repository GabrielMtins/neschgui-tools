#include "TextureViewer.hpp"
#include <iostream>

std::unique_ptr<TextureViewer> TextureViewer::create(SDL_Renderer *renderer) {
	auto texture_viewer = std::make_unique<TextureViewer>();

	texture_viewer->texture = SDL_CreateTexture(
			renderer,
			FORMAT,
			SDL_TEXTUREACCESS_STREAMING,
			WIDTH,
			HEIGHT
			);

	if(texture_viewer->texture == NULL) {
		return nullptr;
	}

	return texture_viewer;
}

void TextureViewer::draw(const RomData& rom_data) {
	uint32_t *pixels;
	int pitch;

	SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);

	pitch /= 4;

	for(int j = 0; j < HEIGHT; j++) {
		for(int i = 0; i < WIDTH; i++) {
			int tile_id = (i / TILE_SIZE) + (j / TILE_SIZE + rom_data.offset_tiles_y) * TILES_PER_ROW;

			int color_id = Rom_GetTilePixelColor(
					&rom_data.viewer,
					tile_id,
					i % TILE_SIZE,
					j % TILE_SIZE
					);

			if(color_id >= PALETTE_SIZE || color_id < 0) {
				continue;
			} 

			pixels[i + j * pitch] = rom_data.palette.at(color_id);
		}
	}

	SDL_UnlockTexture(texture);
}

SDL_Texture * TextureViewer::getTexture(void) {
	return texture;
}

TextureViewer::~TextureViewer(void) {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}
