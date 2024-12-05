#include <iostream>
#include "AnimationClass.hpp"

Animation::Animation(Sprite* sprite, int frameWidth, int frameHeight, int numFrames, float frameTime, int frameCols, int frameRows, int startX, int startY)
	: sprite(sprite), frameWidth(frameWidth), frameHeight(frameHeight), numFrames(numFrames), frameTime(frameTime),
	frameCols(frameCols), frameRows(frameRows), startX(startX), startY(startY), currentFrame(0), lastFrameTime(0) {}

Animation::~Animation() {}

void Animation::Update() {
	Uint32 currentTime = SDL_GetTicks();

	if (currentTime - lastFrameTime >= frameTime) {
		lastFrameTime = currentTime;
		
		if (isLooping || currentFrame < numFrames - 1) {
			currentFrame = (currentFrame + 1) % numFrames;
		}
	}
}

void Animation::Render(int x, int y) {
	int frameX = startX + (currentFrame % frameCols) * frameWidth;
	int frameY = startY + (currentFrame / frameCols) * frameHeight;

	SDL_Rect srcRect = { frameX, frameY, frameWidth, frameHeight };
	SDL_Rect destRect = { x, y, frameWidth, frameHeight };
	SDL_RenderCopy(sprite->GetRenderer(), sprite->GetTexture(), &srcRect, &destRect);
}