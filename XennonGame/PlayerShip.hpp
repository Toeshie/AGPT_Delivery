#pragma once
#include "GameObject.hpp"
#include "MovementComponent.hpp"
#include "AnimationComponent.hpp"
#include "CollisionComponent.hpp"
#include "InputHandler.hpp"
#include "ObjectPool.hpp"
#include "CollisionManager.hpp"
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