#pragma once
#include "../AGPT_Project1/GameObject.hpp"
#include "../AGPT_Project1/AnimationComponent.hpp"
#include "../AGPT_Project1/AIMovementComponent.hpp"
#include "../AGPT_Project1/CollisionComponent.hpp"
#include "../AGPT_Project1/ObjectPool.hpp"
#include "../AGPT_Project1/CollisionManager.hpp"
#include "Bullet.hpp"
#include <iostream>

class Loner : public GameObject {
public:
    Loner(SDL_Renderer* renderer, float x, float y, CollisionManager& collisionMgr);
    void Update(float deltaTime) override;
    void InitializeBulletPool(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer) override;

private:
    void InitializeAnimations(SDL_Renderer* renderer);
    void InitializeMovement();
    void InitializeCollision();
    void HandleCollision(CollisionComponent* other);
    void Shoot();

    static constexpr int SPRITE_SIZE = 64;
    static constexpr int FRAMES_PER_ROW = 4;
    static constexpr float ANIMATION_FRAME_TIME = 0.1f;
    static constexpr float MOVEMENT_SPEED = 60.0f;
    static constexpr float SHOOT_COOLDOWN = 2.0f;

    AnimationComponent* animationComponent;
    CollisionComponent* collision;
    std::shared_ptr<ObjectPool> bulletPool;
    float shootTimer = 0.0f;
    CollisionManager& collisionManager;
};