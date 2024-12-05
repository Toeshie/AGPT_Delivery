#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>

class Graphics {
public:
	Graphics(int width, int height);
	~Graphics();
	bool Initialize();
	SDL_Renderer* GetRenderer() const;

	void Clear();
	void Present();
	void Cleanup();

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

private:
	class SDL_Window* window;
	class SDL_Renderer* renderer;
	int width;
	int height;
};

#endif // GRAPHICS_H