#include "Level.hpp"
#include <algorithm>

Level::Level(Graphics* graphics) : graphics(graphics) {
}

Level::~Level() {
    ClearLevel();
}

void Level::Update(float deltaTime) {
    // Update all game objects
    for (auto& gameObject : gameObjects) {
        gameObject->Update(deltaTime);
    }
}

void Level::Render() {
    // Render all game objects
    for (auto& gameObject : gameObjects) {
        gameObject->Render(graphics->GetRenderer());
    }
}

void Level::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.push_back(gameObject);
}

void Level::RemoveGameObject(std::shared_ptr<GameObject> gameObject) {
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
}

void Level::ClearLevel() {
    gameObjects.clear();
}

const std::vector<std::shared_ptr<GameObject>>& Level::GetGameObjects() const {
    return gameObjects;
}
