#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>

class App {
	public:
		App(void);
		void run(void);
		~App(void);

	private:
		void loop(void);

		SDL_Window *window = NULL;
		SDL_Renderer *renderer = NULL;
		bool quit = false;
};

#endif
