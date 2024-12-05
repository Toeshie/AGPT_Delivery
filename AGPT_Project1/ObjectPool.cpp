#include "ObjectPool.hpp"
#include <algorithm>

ObjectPool::ObjectPool(size_t initialSize, std::function<std::shared_ptr<GameObject>()> factory) 
    : objectFactory(factory) {
    // Pre-allocate objects
    for (size_t i = 0; i < initialSize; ++i) {
        auto obj = objectFactory();
        obj->SetActive(false);
        inactiveObjects.push_back(obj);
    }
}

std::shared_ptr<GameObject> ObjectPool::Acquire() {
    std::shared_ptr<GameObject> obj;
    
    if (inactiveObjects.empty()) {
        obj = objectFactory();
        // Set the pool reference for collision components
        if (auto collision = obj->GetComponent<CollisionComponent>()) {
            collision->SetObjectPool(shared_from_this());
        }
    } else {
        obj = inactiveObjects.back();
        inactiveObjects.pop_back();
    }
    
    obj->SetActive(true);
    activeObjects.push_back(obj);
    return obj;
}

void ObjectPool::Release(std::shared_ptr<GameObject> obj) {
    obj->SetActive(false);
    
    // Remove from active objects
    auto it = std::find(activeObjects.begin(), activeObjects.end(), obj);
    if (it != activeObjects.end()) {
        activeObjects.erase(it);
        inactiveObjects.push_back(obj);
    }
}

void ObjectPool::Update(float deltaTime) {
    // Update only active objects
    for (auto& obj : activeObjects) {
        obj->Update(deltaTime);
    }
}

void ObjectPool::Render(SDL_Renderer* renderer) {
    // Render only active objects
    for (auto& obj : activeObjects) {
        obj->Render(renderer);
    }
}