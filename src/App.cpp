#include "App.hpp"

#include <cstdio>
#include <cstdlib>

App::App(void) {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(-1);
	}

	window = SDL_CreateWindow(
			"neschgui",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			640,
			480,
			SDL_WINDOW_RESIZABLE
			);

	if(window == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(-1);
	}

	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_PRESENTVSYNC
			);

	if(renderer == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(-1);
	}
}

void App::run(void) {
	while(!quit) {
		loop();
	}
}

App::~App(void) {
	SDL_Quit();
}

void App::loop(void) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}
	}
}
