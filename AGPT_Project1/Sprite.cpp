#include "Sprite.hpp"
#include <iostream>

Sprite::Sprite(SDL_Renderer* renderer, const std::string& filePath) : renderer(renderer), texture(nullptr), width(0), height(0) {
	SDL_Surface* tempSurface = SDL_LoadBMP(filePath.c_str());
	if (!tempSurface) {
		std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
		return;
	}

	// Set Pink(RGB: 255, 0, 255) as transparent color
	SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 255, 0, 255));

	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (!texture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
	}
	else {
		width = tempSurface->w;
		height = tempSurface->h;
	}

	SDL_FreeSurface(tempSurface);
}

Sprite::~Sprite() {
	if (texture) {
		SDL_DestroyTexture(texture);
	}
}

void Sprite::Render(int x, int y, int width, int height) {
	SDL_Rect destRect = { x, y, width, height };
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
}