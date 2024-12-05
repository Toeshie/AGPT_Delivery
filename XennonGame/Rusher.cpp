#include "Rusher.hpp"

Rusher::Rusher(SDL_Renderer* renderer, float x, float y) 
    : GameObject(x, y, SPRITE_WIDTH, SPRITE_HEIGHT), animationComponent(nullptr) {
    InitializeAnimations(renderer);
    InitializeMovement();
    InitializeCollision();
}

void Rusher::InitializeAnimations(SDL_Renderer* renderer) {
    // Create and store animation component
    animationComponent = AddComponent<AnimationComponent>(renderer, "graphics/rusher.bmp");
    
    if (!animationComponent) {
        std::cout << "Failed to create Rusher animation component!" << std::endl;
        return;
    }

    
    std::vector<SDL_Rect> idleFrames;
    for (int col = 0; col < FRAMES_PER_ROW; col++) {
        idleFrames.push_back({
            col * SPRITE_WIDTH,   
            0,                    
            SPRITE_WIDTH,         
            SPRITE_HEIGHT         
        });
    }
    
    // Add and play the animation
    animationComponent->AddAnimation("idle", idleFrames, ANIMATION_FRAME_TIME);
    animationComponent->PlayAnimation("idle", true);
}

void Rusher::InitializeMovement() {
    auto movement = AddComponent<AIMovementComponent>(
        MOVEMENT_SPEED, 
        AIMovementComponent::MovementPattern::VERTICAL
    );
    movement->SetScreenBounds(0, 0, 800, 600);
}

void Rusher::InitializeCollision() {
    auto collision = AddComponent<CollisionComponent>(CollisionComponent::ColliderType::ENEMY);
    collision->SetOnCollisionCallback([this](CollisionComponent* other) {
        HandleCollision(other);
    });
}

void Rusher::HandleCollision(CollisionComponent* other) {
    //std::cout << "Rusher::HandleCollision called with type: " 
              //<< static_cast<int>(other->GetType()) << std::endl;
    
    auto colliderType = other->GetType();
    
    if (colliderType == CollisionComponent::ColliderType::PLAYER) {
        //std::cout << "Rusher killed player!" << std::endl;
    }
    else if (colliderType == CollisionComponent::ColliderType::PLAYER_BULLET) {
        //std::cout << "Rusher hit by bullet!" << std::endl;
        SetActive(false);
    }
}
