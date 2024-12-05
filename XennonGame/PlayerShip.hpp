#pragma once
#include "../AGPT_Project1/GameObject.hpp"
#include "../AGPT_Project1/MovementComponent.hpp"
#include "../AGPT_Project1/AnimationComponent.hpp"
#include "../AGPT_Project1/CollisionComponent.hpp"
#include "../AGPT_Project1/InputHandler.hpp"
#include "../AGPT_Project1/ObjectPool.hpp"
#include "../AGPT_Project1/CollisionManager.hpp"
#include "Bullet.hpp"

class PlayerShip : public GameObject {
public:
    static constexpr float SHIP_SPEED = 300.0f;
    
    PlayerShip(SDL_Renderer* renderer, float x, float y, CollisionManager& collisionMgr);
    virtual ~PlayerShip() = default;
    
    void Update(float deltaTime) override;
    void HandleInput(InputHandler& inputHandler);
    void HandleCollision(CollisionComponent* other);
    void InitializeBulletPool(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer) override;

private:
    void Shoot();

    MovementComponent* movement;
    AnimationComponent* animation;
    AnimationComponent* leftAnim;
    AnimationComponent* rightAnim;
    CollisionComponent* collision;
    CollisionManager& collisionManager;
    
    static constexpr float SHOOT_COOLDOWN = 0.25f;
    float shootTimer = 0.0f;
    std::shared_ptr<ObjectPool> bulletPool;
};