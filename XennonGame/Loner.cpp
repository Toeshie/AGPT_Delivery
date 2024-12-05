#include "Loner.hpp"

Loner::Loner(SDL_Renderer* renderer, float x, float y, CollisionManager& collisionMgr) 
    : GameObject(x, y, SPRITE_SIZE, SPRITE_SIZE), 
      animationComponent(nullptr),
      collision(nullptr),
      bulletPool(nullptr),
      collisionManager(collisionMgr) {
    InitializeAnimations(renderer);
    InitializeMovement();
    InitializeCollision();
    InitializeBulletPool(renderer);
}

void Loner::InitializeAnimations(SDL_Renderer* renderer) {
    // Create and store animation component
    animationComponent = AddComponent<AnimationComponent>(renderer, "graphics/LonerA.bmp");
    
    if (!animationComponent) {
        std::cout << "Failed to create Loner animation component!" << std::endl;
        return;
    }

   
    std::vector<SDL_Rect> idleFrames;
    for (int col = 0; col < FRAMES_PER_ROW; col++) {
        idleFrames.push_back({
            col * SPRITE_SIZE, 
            0,                  
            SPRITE_SIZE,        
            SPRITE_SIZE         
        });
    }
    
    // Add and play the animation
    animationComponent->AddAnimation("idle", idleFrames, ANIMATION_FRAME_TIME);
    animationComponent->PlayAnimation("idle", true);
}

void Loner::InitializeMovement() {
    auto movement = AddComponent<AIMovementComponent>(
        MOVEMENT_SPEED, 
        AIMovementComponent::MovementPattern::HORIZONTAL
    );
    movement->SetScreenBounds(0, 0, 800, 600);
}

void Loner::InitializeCollision() {
    collision = AddComponent<CollisionComponent>(CollisionComponent::ColliderType::ENEMY);
    if (collision) {
        std::cout << "Loner collision component initialized" << std::endl;
        collision->SetOnCollisionCallback([this](CollisionComponent* other) {
            HandleCollision(other);
        });
    }
}

void Loner::InitializeBulletPool(SDL_Renderer* renderer) {
    std::cout << "Initializing Loner bullet pool..." << std::endl;
    bulletPool = std::make_shared<ObjectPool>(
        10, // Pool size
        [renderer, this]() -> std::shared_ptr<GameObject> {
            std::cout << "Creating new Loner bullet in pool" << std::endl;
            return std::make_shared<Bullet>(renderer, GetX(), GetY(), false); // false for enemy bullet
        }
    );
    std::cout << "Loner bullet pool initialized with 10 bullets" << std::endl;
}

void Loner::Update(float deltaTime) {
    GameObject::Update(deltaTime);
    
    if (IsActive()) {
        shootTimer -= deltaTime;
        if (shootTimer <= 0.0f) {
            Shoot();
            shootTimer = SHOOT_COOLDOWN;
        }
        
        if (bulletPool) {
            bulletPool->Update(deltaTime);
            
            for (size_t i = 0; i < bulletPool->GetActiveCount(); ++i) {
                auto bullet = bulletPool->GetActiveObject(i);
                if (bullet && bullet->GetY() > 600 + bullet->GetHeight()) {
                    bulletPool->Release(bullet);
                }
            }
        }
    }
}

void Loner::Shoot() {
    if (bulletPool) {
        auto bullet = bulletPool->Acquire();
        if (bullet) {
            bullet->SetPosition(GetX(), GetY() + GetHeight()/2);
            
            // Add bullet's collision component to the collision manager
            if (auto bulletCollision = bullet->GetComponent<CollisionComponent>()) {
                collisionManager.AddCollider(std::make_shared<CollisionComponent>(*bulletCollision));
            }
            
            std::cout << "Loner fired bullet! Active bullets: " << bulletPool->GetActiveCount() << std::endl;
        }
    }
}

void Loner::Render(SDL_Renderer* renderer) {
    GameObject::Render(renderer);
    
    // Render bullets
    if (bulletPool) {
        bulletPool->Render(renderer);
    }
}

void Loner::HandleCollision(CollisionComponent* other) {
    //std::cout << "Loner::HandleCollision called with type: " 
              //<< static_cast<int>(other->GetType()) << std::endl;
    
    auto colliderType = other->GetType();
    
    if (colliderType == CollisionComponent::ColliderType::PLAYER_BULLET) {
        //std::cout << "Loner hit by bullet!" << std::endl;
        SetActive(false);
        // Release all active bullets when Loner is destroyed
        if (bulletPool) {
            for (size_t i = 0; i < bulletPool->GetActiveCount(); ++i) {
                auto bullet = bulletPool->GetActiveObject(i);
                if (bullet) {
                    bulletPool->Release(bullet);
                }
            }
        }
    }
}