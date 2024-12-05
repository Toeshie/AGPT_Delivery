#pragma once
#include "GameObject.hpp"
#include "CollisionComponent.hpp"
#include "MovementComponent.hpp"
#include "AnimationComponent.hpp"
#include <iostream>

class Bullet : public GameObject {
public:
    // Constants for player bullet sprite sheet
    static constexpr int PLAYER_FRAME_WIDTH = 8;
    static constexpr int PLAYER_FRAME_HEIGHT = 16;
    static constexpr int PLAYER_FRAMES_COUNT = 4;
    
    // Constants for enemy (Loner) bullet sprite sheet
    static constexpr int ENEMY_FRAME_WIDTH = 16;   
    static constexpr int ENEMY_FRAME_HEIGHT = 16;  
    static constexpr int ENEMY_FRAMES_COUNT = 8;   // Updated to 8 frames
    
    static constexpr float ANIMATION_FRAME_TIME = 0.05f;

    Bullet(SDL_Renderer* renderer, float x, float y, bool isPlayerBullet = true)
        : GameObject(x, y, 
                    isPlayerBullet ? PLAYER_FRAME_WIDTH : ENEMY_FRAME_WIDTH,
                    isPlayerBullet ? PLAYER_FRAME_HEIGHT : ENEMY_FRAME_HEIGHT), 
          isPlayerBullet(isPlayerBullet)
    {
        std::cout << "Creating new bullet" << std::endl;
        
        // Add animation component with different sprite based on bullet type
        animationComponent = AddComponent<AnimationComponent>(
            renderer, 
            isPlayerBullet ? "graphics/missilePlayer.bmp" : "graphics/EnWeap6.bmp"
        );
        if (!animationComponent) {
            std::cout << "Failed to create Bullet animation component!" << std::endl;
            return;
        }

        // Set up the animation frames
        std::vector<SDL_Rect> frames;
        if (isPlayerBullet) {
            // Player bullet animation (4 frames, 8x16 each)
            for (int i = 0; i < PLAYER_FRAMES_COUNT; i++) {
                frames.push_back({
                    i * PLAYER_FRAME_WIDTH,
                    0,
                    PLAYER_FRAME_WIDTH,
                    PLAYER_FRAME_HEIGHT
                });
            }
        } else {
            // Enemy bullet animation (8 frames, 16x16 each)
            for (int i = 0; i < ENEMY_FRAMES_COUNT; i++) {
                frames.push_back({
                    i * ENEMY_FRAME_WIDTH,
                    0,
                    ENEMY_FRAME_WIDTH,
                    ENEMY_FRAME_HEIGHT
                });
            }
        }

        // Add and play the animation
        animationComponent->AddAnimation("bullet", frames, ANIMATION_FRAME_TIME);
        animationComponent->PlayAnimation("bullet", true);
        
        // Add collision component
        collision = AddComponent<CollisionComponent>(
            isPlayerBullet ? CollisionComponent::ColliderType::PLAYER_BULLET 
                         : CollisionComponent::ColliderType::ENEMY_BULLET
        );
        
        std::cout << "Bullet collision component created with type: " 
                  << (isPlayerBullet ? "PLAYER_BULLET" : "ENEMY_BULLET") << std::endl;
        
        // Set up collision callback
        collision->SetOnCollisionCallback([this](CollisionComponent* other) {
            HandleCollision(other);
        });
        
        // Add movement component
        auto movement = AddComponent<MovementComponent>();
        movement->SetSpeed(200.0f);
        movement->SetVelocity(0, isPlayerBullet ? -1.0f : 1.0f);
    }

    void HandleCollision(CollisionComponent* other) {
        if (isPlayerBullet) {
            if (other->GetType() == CollisionComponent::ColliderType::ENEMY) {
                if (auto pool = collision->GetObjectPool()) {
                    pool->Release(shared_from_this());
                }
            }
        } else {
            if (other->GetType() == CollisionComponent::ColliderType::PLAYER) {
                std::cout << "Enemy bullet hit player!" << std::endl;
                if (auto pool = collision->GetObjectPool()) {
                    pool->Release(shared_from_this());
                }
            }
        }
    }

    // Add this method to verify collision component state
    void Update(float deltaTime) override {
        GameObject::Update(deltaTime);
        
    }

private:
    bool isPlayerBullet;
    CollisionComponent* collision;
    AnimationComponent* animationComponent;
};