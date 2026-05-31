#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <SDL2/SDL.h>
#include <rom_tools.h>
#include <memory>

#include <array>

#include "Palette.hpp"
#include "RomData.hpp"

class TextureViewer {
	public:
		static std::unique_ptr<TextureViewer> create(SDL_Renderer *renderer);
		void draw(const RomData& rom_data);
		SDL_Texture * getTexture(void);

		~TextureViewer(void);

		static constexpr int TILES_PER_ROW = 16;
		static constexpr int TILES_PER_COLUMN = 16;
		static constexpr int TILE_SIZE = 8;
		static constexpr int WIDTH = TILES_PER_ROW * TILE_SIZE;
		static constexpr int HEIGHT = TILES_PER_COLUMN * TILE_SIZE;
		static constexpr SDL_PixelFormatEnum FORMAT = SDL_PIXELFORMAT_ARGB8888;

	private:
		SDL_Texture *texture = NULL;
};

#endif
