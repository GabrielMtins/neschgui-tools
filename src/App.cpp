#include "App.hpp"

#include <cstdio>
#include <cstdlib>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

App::App(void) {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(-1);
	}

	float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);

	window = SDL_CreateWindow(
			"neschgui",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			640,
			480,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_RESIZABLE
			);

	if(window == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(-1);
	}

	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
			);

	if(renderer == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(-1);
	}

	SDL_MaximizeWindow(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
}

void App::run(void) {
	while(!quit) {
		loop();
	}
}

App::~App(void) {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void App::loop(void) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}
	}

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Render();
	SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_RenderPresent(renderer);
}
