#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "CollisionComponent.hpp"

class CollisionManager {
public:
    CollisionManager() = default;
    ~CollisionManager() = default;

    CollisionManager(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

    void AddCollider(std::shared_ptr<CollisionComponent> collider);
    void RemoveCollider(std::shared_ptr<CollisionComponent> collider);
    void Update();
    void Clear();

private:
    std::vector<std::shared_ptr<CollisionComponent>> colliders;
};
