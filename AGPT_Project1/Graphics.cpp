#include "Graphics.hpp"
#include <iostream>

Graphics::Graphics(int width, int height) : window(nullptr), renderer(nullptr), width(width), height(height) {}

Graphics::~Graphics() {
	Cleanup();
}

bool Graphics::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
		std::cerr << "SDL Initialization Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create Window
	window = SDL_CreateWindow("XennonClone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window Creation Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Renderer Creation Error: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

SDL_Renderer* Graphics::GetRenderer() const {
	return renderer;
}

void Graphics::Clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Graphics::Present() {
	SDL_RenderPresent(renderer);
}

void Graphics::Cleanup() {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	SDL_Quit();
}