#pragma once
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Graphics.hpp"

class Level {
public:
    Level(Graphics* graphics);
    ~Level();

    void Update(float deltaTime);
    void Render();
    
    // Object management
    void AddGameObject(std::shared_ptr<GameObject> gameObject);
    void RemoveGameObject(std::shared_ptr<GameObject> gameObject);
    void ClearLevel();

    const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const;

private:
    Graphics* graphics;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
};