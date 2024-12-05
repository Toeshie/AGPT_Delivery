#pragma once

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL.h>
#include <string>

class Sprite {
public:
	Sprite(SDL_Renderer* renderer, const std::string& filePath);
	~Sprite();

	void Render(int x, int y, int width, int height);
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	// Add these getters
	SDL_Texture* GetTexture() const { return texture; }
	SDL_Renderer* GetRenderer() const { return renderer; }

private:
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	int width, height;
};


#endif // SPRITE_HPP