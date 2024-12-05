#include "PlayerShip.hpp"
#include <iostream>

PlayerShip::PlayerShip(SDL_Renderer* renderer, float x, float y, CollisionManager& collisionMgr) 
    : GameObject(x, y, 64, 64), movement(nullptr), animation(nullptr), 
      leftAnim(nullptr), rightAnim(nullptr), collision(nullptr), collisionManager(collisionMgr)
{
    std::cout << "\n=== PlayerShip Constructor Start ===" << std::endl;
    std::cout << "Creating PlayerShip at position: " << x << ", " << y << std::endl;
    
    // Add collision component
    std::cout << "Adding CollisionComponent..." << std::endl;
    collision = AddComponent<CollisionComponent>(CollisionComponent::ColliderType::PLAYER);
    if (collision) {
        collision->SetOnCollisionCallback([this](CollisionComponent* other) {
            HandleCollision(other);
        });
        std::cout << "CollisionComponent added successfully" << std::endl;
    } else {
        std::cout << "Failed to add CollisionComponent!" << std::endl;
    }

    // Add movement component
    std::cout << "Adding MovementComponent..." << std::endl;
    movement = AddComponent<MovementComponent>();
    if (movement) {
        movement->SetSpeed(SHIP_SPEED);
        movement->SetBoundToScreen(true);
        std::cout << "MovementComponent added successfully" << std::endl;
    } else {
        std::cout << "Failed to add MovementComponent!" << std::endl;
    }

    // Add animation components for each sprite sheet
    std::cout << "Adding AnimationComponents..." << std::endl;
    
    // Main idle animation
    animation = AddComponent<AnimationComponent>(renderer, "graphics/Ship1_Idle.bmp");
    if (!animation) {
        std::cout << "Failed to create idle AnimationComponent!" << std::endl;
        return;
    }
    
    // Left bank animation
    leftAnim = AddComponent<AnimationComponent>(renderer, "graphics/Ship1_Left.bmp");
    if (!leftAnim) {
        std::cout << "Failed to create left AnimationComponent!" << std::endl;
        return;
    }
    
    // Right bank animation
    rightAnim = AddComponent<AnimationComponent>(renderer, "graphics/Ship1_Right.bmp");
    if (!rightAnim) {
        std::cout << "Failed to create right AnimationComponent!" << std::endl;
        return;
    }
    
    // Setup animations
    std::cout << "Setting up animations..." << std::endl;
    
    // Idle animation (single frame)
    std::vector<SDL_Rect> idleFrames = {
        {0, 0, 64, 64}
    };
    animation->AddAnimation("idle", idleFrames, 0.1f);
    
    // Left bank animation (3 frames)
    std::vector<SDL_Rect> leftFrames = {
        {0, 0, 64, 64},
        {64, 0, 64, 64},
        {128, 0, 64, 64}
    };
    leftAnim->AddAnimation("bankLeft", leftFrames, 0.1f);
    
    // Right bank animation (3 frames)
    std::vector<SDL_Rect> rightFrames = {
        {0, 0, 64, 64},
        {64, 0, 64, 64},
        {128, 0, 64, 64}
    };
    rightAnim->AddAnimation("bankRight", rightFrames, 0.1f);

    animation->PlayAnimation("idle");
    std::cout << "Animations set up successfully" << std::endl;

    std::cout << "=== PlayerShip Constructor End ===\n" << std::endl;
}

void PlayerShip::InitializeBulletPool(SDL_Renderer* renderer) {
    std::cout << "Initializing bullet pool..." << std::endl;
    bulletPool = std::make_shared<ObjectPool>(
        20,
        [renderer, this]() -> std::shared_ptr<GameObject> {
            std::cout << "Creating new bullet in pool" << std::endl;
            return std::make_shared<Bullet>(renderer, GetX(), GetY(), true);
        }
    );
    std::cout << "Bullet pool initialized with 20 bullets" << std::endl;
}

void PlayerShip::Update(float deltaTime) {
    GameObject::Update(deltaTime);
    
    // Update shoot timer
    if (shootTimer > 0) {
        shootTimer -= deltaTime;
    }
    
    // Update bullet pool
    if (bulletPool) {
        bulletPool->Update(deltaTime);
        
        // Clean up bullets that are off screen
        for (size_t i = 0; i < bulletPool->GetActiveCount(); ++i) {
            auto bullet = bulletPool->GetActiveObject(i);
            if (bullet && bullet->GetY() < -bullet->GetHeight()) {
                std::cout << "Bullet went off screen, returning to pool" << std::endl;
                bulletPool->Release(bullet);
            }
        }
        
        // Print pool statistics periodically
        static float statTimer = 0;
        statTimer += deltaTime;
        if (statTimer >= 1.0f) {  // Print every second
            std::cout << "Bullet Pool Stats - Active: " << bulletPool->GetActiveCount() 
                     << ", Inactive: " << bulletPool->GetInactiveCount() << std::endl;
            statTimer = 0;
        }
    }

    // Reset collision state each frame
    if (collision) {
        collision->ResetCollisionState();
    }
}

void PlayerShip::HandleInput(InputHandler& inputHandler) {
    if (!IsActive()) {
        return; // Do nothing if the player is inactive
    }

    float horizontalInput = 0.0f;
    float verticalInput = 0.0f;

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    
    // Safety check for animation components
    if (!animation || !leftAnim || !rightAnim) {
        std::cout << "Warning: Animation components not properly initialized!" << std::endl;
        return;
    }
    
    // Hide all animations initially
    animation->SetVisible(false);
    leftAnim->SetVisible(false);
    rightAnim->SetVisible(false);
    
    // Keyboard input
    if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) {
        horizontalInput -= 1.0f;
        leftAnim->SetVisible(true);
        if (leftAnim->GetCurrentAnimation() != "bankLeft") {
            leftAnim->PlayAnimation("bankLeft", false);
        }
    }
    else if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) {
        horizontalInput += 1.0f;
        rightAnim->SetVisible(true);
        if (rightAnim->GetCurrentAnimation() != "bankRight") {
            rightAnim->PlayAnimation("bankRight", false);
        }
    }
    else {
        animation->SetVisible(true);
        if (animation->GetCurrentAnimation() != "idle") {
            animation->PlayAnimation("idle");
        }
    }

    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) {
        verticalInput -= 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) {
        verticalInput += 1.0f;
    }

    // Controller input
    if (inputHandler.IsButtonPressed(SDL_CONTROLLER_BUTTON_A)) {
        if (shootTimer <= 0) {
            Shoot();
            shootTimer = SHOOT_COOLDOWN;
        }
    }

    // Keyboard space bar for shooting
    if (keyboardState[SDL_SCANCODE_SPACE]) {
        if (shootTimer <= 0) {
            Shoot();
            shootTimer = SHOOT_COOLDOWN;
        }
    }

    // Example for using controller axis for movement
    horizontalInput += inputHandler.GetJoystickAxis(SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f;
    verticalInput += inputHandler.GetJoystickAxis(SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f;

    if (movement) {
        movement->SetVelocity(horizontalInput, verticalInput);
    }
}

void PlayerShip::HandleCollision(CollisionComponent* other) {
    auto colliderType = other->GetType();
    
    if (colliderType == CollisionComponent::ColliderType::ENEMY) {
        std::cout << "Player hit by enemy!" << std::endl;
        SetActive(false); // Deactivate the player
    }
    else if (colliderType == CollisionComponent::ColliderType::ENEMY_BULLET) {
        std::cout << "Player hit by enemy bullet!" << std::endl;
        SetActive(false); // Deactivate the player
    }
}

void PlayerShip::Shoot() {
    if (bulletPool) {
        auto bullet = bulletPool->Acquire();
        if (bullet) {
            bullet->SetPosition(GetX(), GetY() - GetHeight()/2);
            
            // Add bullet's collision component to the collision manager
            if (auto bulletCollision = bullet->GetComponent<CollisionComponent>()) {
                std::cout << "Adding bullet collision to manager" << std::endl;
                // Make sure you have access to the collision manager here
                collisionManager.AddCollider(std::make_shared<CollisionComponent>(*bulletCollision));
            }
            
            std::cout << "Bullet fired! Active bullets: " << bulletPool->GetActiveCount() << std::endl;
        }
    }
}

void PlayerShip::Render(SDL_Renderer* renderer) {
    GameObject::Render(renderer);
    
    // Render bullets
    if (bulletPool) {
        bulletPool->Render(renderer);
    }
}