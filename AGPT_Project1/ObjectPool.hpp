#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "GameObject.hpp"
#include "CollisionComponent.hpp"

class ObjectPool : public std::enable_shared_from_this<ObjectPool> {
public:
    ObjectPool(size_t initialSize, std::function<std::shared_ptr<GameObject>()> factory);
    
    std::shared_ptr<GameObject> Acquire();
    void Release(std::shared_ptr<GameObject> obj);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    size_t GetActiveCount() const { return activeObjects.size(); }
    size_t GetInactiveCount() const { return inactiveObjects.size(); }
    std::shared_ptr<GameObject> GetActiveObject(size_t index) {
        if (index < activeObjects.size()) {
            return activeObjects[index];
        }
        return nullptr;
    }

private:
    std::vector<std::shared_ptr<GameObject>> activeObjects;
    std::vector<std::shared_ptr<GameObject>> inactiveObjects;
    std::function<std::shared_ptr<GameObject>()> objectFactory;
};
