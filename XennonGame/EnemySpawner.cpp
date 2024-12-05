#include "EnemySpawner.hpp"

EnemySpawner::EnemySpawner(SDL_Renderer* renderer, Level& level, CollisionManager& collisionMgr)
    : renderer(renderer), level(level), collisionManager(collisionMgr),
      lonerSpawnTimer(0.0f), rusherSpawnTimer(0.0f),
      rng(std::random_device{}()) {
    // Initialize spawn positions
    screenWidth = 800;  // Adjust based on your screen width
    screenHeight = 600; // Adjust based on your screen height
    spawnY = 50.0f;    // Spawn height from top of screen

    // Initialize object pools
    lonerPool = std::make_shared<ObjectPool>(10, [this, renderer]() -> std::shared_ptr<GameObject> {
        return std::make_shared<Loner>(renderer, 0, 0, collisionManager);
    });

    rusherPool = std::make_shared<ObjectPool>(10, [this, renderer]() -> std::shared_ptr<GameObject> {
        return std::make_shared<Rusher>(renderer, 0, 0);
    });
}

void EnemySpawner::Update(float deltaTime) {
    // Update Loner spawn timer
    lonerSpawnTimer -= deltaTime;
    if (lonerSpawnTimer <= 0.0f) {
        SpawnLoner();
        lonerSpawnTimer = LONER_SPAWN_INTERVAL;
    }

    // Update Rusher spawn timer
    rusherSpawnTimer -= deltaTime;
    if (rusherSpawnTimer <= 0.0f) {
        SpawnRusher();
        rusherSpawnTimer = RUSHER_SPAWN_INTERVAL;
    }

    // Update pools
    lonerPool->Update(deltaTime);
    rusherPool->Update(deltaTime);
}

void EnemySpawner::SpawnLoner() {
    // Generate random X position
    std::uniform_real_distribution<float> xDist(100.0f, screenWidth - 100.0f);
    float spawnX = xDist(rng);

    auto loner = std::dynamic_pointer_cast<Loner>(lonerPool->Acquire());
    if (loner) {
        loner->SetPosition(spawnX, spawnY);
        loner->SetActive(true);
        level.AddGameObject(loner);
        loner->InitializeBulletPool(renderer);

        // Add collision component to collision manager
        if (auto collision = loner->GetComponent<CollisionComponent>()) {
            collisionManager.AddCollider(std::make_shared<CollisionComponent>(*collision));
        }
    }
}

void EnemySpawner::SpawnRusher() {
    // Generate random X position
    std::uniform_real_distribution<float> xDist(100.0f, screenWidth - 100.0f);
    float spawnX = xDist(rng);

    auto rusher = std::dynamic_pointer_cast<Rusher>(rusherPool->Acquire());
    if (rusher) {
        rusher->SetPosition(spawnX, spawnY);
        rusher->SetActive(true);
        level.AddGameObject(rusher);

        // Add collision component to collision manager
        if (auto collision = rusher->GetComponent<CollisionComponent>()) {
            collisionManager.AddCollider(std::make_shared<CollisionComponent>(*collision));
        }
    }
}