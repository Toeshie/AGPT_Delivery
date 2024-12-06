#pragma once

#ifndef ANIMATIONCLASS_HPP
#define ANIMATIONCLASS_HPP

#include "Sprite.hpp"
#include <vector>

class Animation {
public:
	Animation(Sprite* sprite, int frameWidth, int frameHeight, int numFrames, float frameTime, int frameCols, int frameRows = 1, int startX = 0, int startY = 0);
	~Animation();

	void Update();
	void Render(int x, int y);

	int GetCurrentFrame() const { return currentFrame; }
	int GetFrameWidth() const { return frameWidth; }
	int GetFrameHeight() const { return frameHeight; }
	int GetFrameCols() const { return frameCols; }
	int GetFrameRows() const { return frameRows; }
	void SetLooping(bool loop) { isLooping = loop; }
	bool IsFinished() const { return !isLooping && currentFrame >= numFrames - 1; }
	void Reset() { currentFrame = 0; lastFrameTime = 0; }

private:
	Sprite* sprite;
	int frameWidth, frameHeight;
	int numFrames;
	float frameTime;
	int frameCols;      
	int frameRows;       
	int currentFrame;
	Uint32 lastFrameTime;
	int startX;
	int startY;
	bool isLooping = true; 
};

#endif // ANIMATIONCLASS_HPP