#include "CollisionManager.hpp"

void CollisionManager::AddCollider(std::shared_ptr<CollisionComponent> collider) {
    // Check if collider already exists
    auto it = std::find(colliders.begin(), colliders.end(), collider);
    if (it == colliders.end()) {
        colliders.push_back(collider);
    }
}

void CollisionManager::RemoveCollider(std::shared_ptr<CollisionComponent> collider) {
    colliders.erase(
        std::remove(colliders.begin(), colliders.end(), collider),
        colliders.end()
    );
}

void CollisionManager::Update() {
    for (size_t i = 0; i < colliders.size(); i++) {
        if (!colliders[i] || !colliders[i]->IsEnabled()) {
            continue;
        }

        for (size_t j = i + 1; j < colliders.size(); j++) {
            if (!colliders[j] || !colliders[j]->IsEnabled()) {
                continue;
            }

            // Skip collision check between player bullets and player
            if ((colliders[i]->GetType() == CollisionComponent::ColliderType::PLAYER_BULLET &&
                 colliders[j]->GetType() == CollisionComponent::ColliderType::PLAYER) ||
                (colliders[i]->GetType() == CollisionComponent::ColliderType::PLAYER &&
                 colliders[j]->GetType() == CollisionComponent::ColliderType::PLAYER_BULLET)) {
                continue;
            }

            if (colliders[i]->CheckCollision(colliders[j])) {
                /*std::cout << "Collision detected between types: " 
                          << static_cast<int>(colliders[i]->GetType()) 
                          << " and " 
                          << static_cast<int>(colliders[j]->GetType()) << std::endl;
                */
                colliders[i]->OnCollide(colliders[j].get());
                colliders[j]->OnCollide(colliders[i].get());
            }
        }
    }

    // Remove deactivated colliders
    colliders.erase(
        std::remove_if(colliders.begin(), colliders.end(),
                       [](const std::shared_ptr<CollisionComponent>& collider) {
                           return !collider || !collider->GetOwner()->IsActive();
                       }),
        colliders.end()
    );
}

void CollisionManager::Clear() {
    colliders.clear();
}
