#pragma once
#include "Loner.hpp"
#include "Rusher.hpp"
#include "Level.hpp"
#include "ObjectPool.hpp"
#include <random>

class EnemySpawner {
public:
    EnemySpawner(SDL_Renderer* renderer, Level& level, CollisionManager& collisionMgr);
    void Update(float deltaTime);

private:
    void SpawnLoner();
    void SpawnRusher();

    SDL_Renderer* renderer;
    Level& level;
    CollisionManager& collisionManager;
    float lonerSpawnTimer;
    float rusherSpawnTimer;
    std::mt19937 rng;
    float screenWidth;
    float screenHeight;
    float spawnY;

    std::shared_ptr<ObjectPool> lonerPool;
    std::shared_ptr<ObjectPool> rusherPool;

    // Spawn intervals in seconds
    static constexpr float LONER_SPAWN_INTERVAL = 3.0f;
    static constexpr float RUSHER_SPAWN_INTERVAL = 4.0f;
};
