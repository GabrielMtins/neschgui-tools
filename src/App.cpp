#include "App.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>


#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

App::App(void) {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << SDL_GetError() << '\n';
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
		std::cerr << SDL_GetError() << '\n';
		exit(-1);
	}

	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
			);

	if(renderer == NULL) {
		std::cerr << SDL_GetError() << '\n';
		exit(-1);
	}

	SDL_MaximizeWindow(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	if(rom.load("tools/roms/sml2.gb")) {
		std::cout << "loaded!\n";
	}

	texture_viewer = TextureViewer::create(renderer);

	if(texture_viewer == nullptr) {
		std::cerr << "Failed to create texture viewer. " << SDL_GetError() << '\n';
		exit(-1);
	}

	rom.setViewerFormat(ROM_TYPE_GB);
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

	beginRender();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 windowPos = viewport->Pos;
	ImVec2 windowSize = viewport->Size;

	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	
	ImGui::Begin(
			"MainWindowFullscreen",
			nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | 
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoCollapse
			);

	if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("File")) {
			ImGui::MenuItem("New");
			ImGui::MenuItem("Open");
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Save as");
			ImGui::MenuItem("Exit");
			ImGui::EndMenu();
		}
	
		ImGui::EndMenuBar();

	}

	/*
	ImGui::BeginGroup();
	for(int i = 0; i < 10; i++) {
		if(i % 2 == 1) {
			ImGui::SameLine();
		}

		ImGui::Button((std::string("oi") + std::to_string(i)).c_str(), ImVec2(64.0f, 64.0f));
	}
	ImGui::EndGroup();

	ImGui::SameLine();
	*/

	ImGui::BeginGroup();
	static int value = 0;
	rom.offset_tiles_y = value;
	texture_viewer->draw(rom);

	ImGui::VSliderInt("##vslider", ImVec2(36, 800), &value, rom.viewer.num_tiles / TextureViewer::TILES_PER_ROW - TextureViewer::TILES_PER_ROW, 0, "");
	//ImGui::DragInt("##drag", &value, 1.0f, rom.viewer.num_tiles / TextureViewer::TILES_PER_ROW - TextureViewer::TILES_PER_ROW, 0, "");
	ImGui::SameLine();

	ImGui::Image(
			(ImTextureID) (texture_viewer->getTexture()),
			ImVec2(800, 800)
			);
	ImGui::EndGroup();

	ImGui::End();

	//;;ImGui::Begin()

	endRender();
}

void App::beginRender(void) {
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void App::endRender(void) {
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Render();
	SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_RenderPresent(renderer);
}
