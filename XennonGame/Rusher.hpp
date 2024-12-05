#pragma once
#include "GameObject.hpp"
#include "AnimationComponent.hpp"
#include "AIMovementComponent.hpp"
#include "CollisionComponent.hpp"

class Rusher : public GameObject {
public:
    Rusher(SDL_Renderer* renderer, float x, float y);
    virtual ~Rusher() = default;

private:
    void InitializeAnimations(SDL_Renderer* renderer);
    void InitializeMovement();
    void InitializeCollision();
    void HandleCollision(CollisionComponent* other);

    AnimationComponent* animationComponent;  // Store the reference

    static constexpr int SPRITE_WIDTH = 64;
    static constexpr int SPRITE_HEIGHT = 32;
    static constexpr int FRAMES_PER_ROW = 4;
    static constexpr int TOTAL_ROWS = 6;
    static constexpr float MOVEMENT_SPEED = 100.0f;
    static constexpr float ANIMATION_FRAME_TIME = 0.1f;
};