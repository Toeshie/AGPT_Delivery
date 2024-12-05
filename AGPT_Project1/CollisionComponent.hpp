#pragma once
#include "Component.hpp"
#include <SDL.h>
#include <functional>
#include <memory>
#include <iostream>
#include "GameObject.hpp"

// Forward declare ObjectPool to avoid circular dependency
class ObjectPool;

class CollisionComponent : public Component {
public:
    enum class ColliderType {
        PLAYER,
        ENEMY,
        PLAYER_BULLET,
        ENEMY_BULLET
    };

    CollisionComponent(ColliderType type = ColliderType::PLAYER) 
        : colliderType(type), isEnabled(true), isCurrentlyColliding(false) {}

    void Update(float deltaTime) override {
        if (!owner) return;
        
        // Update bounds to match owner's position and size
        bounds = {
            static_cast<int>(owner->GetX() - owner->GetWidth() / 2),
            static_cast<int>(owner->GetY() - owner->GetHeight() / 2),
            owner->GetWidth(),
            owner->GetHeight()
        };

    }

    SDL_Rect GetBounds() const {
        if (owner) {
            return SDL_Rect{
                static_cast<int>(owner->GetX() - owner->GetWidth() / 2),
                static_cast<int>(owner->GetY() - owner->GetHeight() / 2),
                owner->GetWidth(),
                owner->GetHeight()
            };
        }
        return bounds;
    }

    bool CheckCollision(const std::shared_ptr<CollisionComponent>& other) const {
        if (!other || !isEnabled || !other->IsEnabled()) return false;

        // Get updated bounds for both objects
        SDL_Rect thisBounds = GetBounds();
        SDL_Rect otherBounds = other->GetBounds();
        
        // Check for overlap
        bool collision = !(thisBounds.x + thisBounds.w <= otherBounds.x ||
                         thisBounds.x >= otherBounds.x + otherBounds.w ||
                         thisBounds.y + thisBounds.h <= otherBounds.y ||
                         thisBounds.y >= otherBounds.y + otherBounds.h);

        if (collision) {
            //std::cout << "COLLISION DETECTED!" << std::endl;
        }

        return collision;
    }

    void SetOnCollisionCallback(std::function<void(CollisionComponent*)> callback) {
        onCollision = callback;
    }

    void OnCollide(CollisionComponent* other) {
        if (!isEnabled) return;
        /*
        std::cout << "Collision detected between types: " 
                  << static_cast<int>(colliderType) << " and " 
                  << static_cast<int>(other->GetType()) << std::endl;
        */
        if (onCollision) {
            onCollision(other);
        }
        isCurrentlyColliding = true;
    }

    void ResetCollisionState() {
        isCurrentlyColliding = false;
    }

    ColliderType GetType() const { return colliderType; }
    void SetEnabled(bool enabled) { isEnabled = enabled; }
    bool IsEnabled() const { return isEnabled; }
    
    std::string GetName() const override { return "CollisionComponent"; }

    void SetObjectPool(const std::shared_ptr<ObjectPool>& pool) {
        objectPool = pool;
    }

    std::shared_ptr<ObjectPool> GetObjectPool() const {
        return objectPool.lock(); 
    }

    GameObject* GetOwner() const { return owner; }

private:
    ColliderType colliderType;
    bool isEnabled;
    bool isCurrentlyColliding;
    SDL_Rect bounds;
    std::function<void(CollisionComponent*)> onCollision;
    std::weak_ptr<ObjectPool> objectPool;
};