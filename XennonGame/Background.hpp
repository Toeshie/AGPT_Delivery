#pragma once
#include "GameObject.hpp"
#include "AnimationComponent.hpp"

class Background : public GameObject {
public:
    enum class ScrollDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Background(SDL_Renderer* renderer, float x, float y, 
              const std::string& spritePath, 
              ScrollDirection direction = ScrollDirection::LEFT,
              float scrollSpeed = 0.0f,
              int width = 64,    // Default width for rock sprites
              int height = 720); // Default height
    void Update(float deltaTime);

private:
    AnimationComponent* sprite;
    float scrollSpeed;
    float xOffset;
    float yOffset;
    ScrollDirection direction;
    const float SCREEN_WIDTH = 1280.0f;
};
