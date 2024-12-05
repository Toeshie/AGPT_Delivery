#include "Background.hpp"

Background::Background(SDL_Renderer* renderer, float x, float y, 
                     const std::string& spritePath, 
                     ScrollDirection direction, float scrollSpeed,
                     int width, int height)
    : GameObject(x, y, width, height),
      scrollSpeed(scrollSpeed), 
      xOffset(0.0f),
      yOffset(0.0f),
      direction(direction)
{
    sprite = AddComponent<AnimationComponent>(renderer, spritePath);
    if (sprite) {
        std::vector<SDL_Rect> frames = {
            {0, 0, width, height}
        };
        sprite->AddAnimation("scroll", frames, 0.1f);
        sprite->PlayAnimation("scroll");
    }
}

void Background::Update(float deltaTime) {
    GameObject::Update(deltaTime);
    
    if (scrollSpeed != 0.0f) {
        float movement = scrollSpeed * deltaTime;
        
        if (direction == ScrollDirection::LEFT) {
            xOffset -= movement;
            if (xOffset <= -width) {
                xOffset = 0.0f;
            }
        } else if (direction == ScrollDirection::RIGHT) {
            xOffset += movement;
            if (xOffset >= width) {
                xOffset = 0.0f;
            }
        } else if (direction == ScrollDirection::UP) {
            yOffset -= movement;
            if (yOffset <= -height) {
                yOffset = 0.0f;
            }
        } else if (direction == ScrollDirection::DOWN) {
            yOffset += movement;
            if (yOffset >= height) {
                yOffset = 0.0f;
            }
        }
        
        SetX(GetInitialX() + xOffset);
        SetY(GetInitialY() + yOffset);
    }
}