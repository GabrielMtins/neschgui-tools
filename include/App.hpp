#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

#include "TextureViewer.hpp"
#include "RomData.hpp"

class App {
	public:
		App(void);
		void run(void);
		~App(void);

	private:
		void loop(void);
		void beginRender(void);
		void endRender(void);

		SDL_Window *window = NULL;
		SDL_Renderer *renderer = NULL;
		bool quit = false;

		RomData rom;
		std::unique_ptr<TextureViewer> texture_viewer;
};

#endif
