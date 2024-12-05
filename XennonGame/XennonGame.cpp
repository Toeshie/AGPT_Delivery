#include <iostream>
#include "Graphics.hpp"  
#include "PlayerShip.hpp"
#include "InputHandler.hpp"
#include "Level.hpp"
#include "Background.hpp"
#include "Loner.hpp"
#include "Rusher.hpp"
#include "CollisionManager.hpp"
#include "EnemySpawner.hpp"

class Game {
private:
    Graphics graphics;
    InputHandler inputHandler;
    std::shared_ptr<PlayerShip> player;
    std::shared_ptr<Background> backgroundStatic;
    std::shared_ptr<Background> backgroundParallaxLeft;
    bool isRunning;
    Uint32 lastTime;
    Level level;
    std::shared_ptr<Loner> lonerEnemy;
    std::shared_ptr<Rusher> rusherEnemy;
    CollisionManager collisionManager;
    std::vector<std::shared_ptr<CollisionComponent>> collisionComponents;
    std::unique_ptr<EnemySpawner> enemySpawner;

public:
    Game() : graphics(800, 600), isRunning(true), level(&graphics) {
        std::cout << "Game constructor called" << std::endl;
        lastTime = SDL_GetTicks();
    }

    bool Initialize() {
        std::cout << "Initializing game..." << std::endl;
    
    if (!graphics.Initialize()) {
        std::cout << "Failed to initialize graphics!" << std::endl;
        return false;
    }
    std::cout << "Graphics initialized successfully" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create static background (full screen)
    backgroundStatic = std::make_shared<Background>(
        graphics.GetRenderer(),
        graphics.GetWidth() / 2.0f,
        graphics.GetHeight() / 2.0f,
        "graphics/galaxy2.bmp",  
        Background::ScrollDirection::LEFT,  // static no matter
        0.0f,  // No movement
        graphics.GetWidth(),  // Full screen width
        graphics.GetHeight()  // Full screen height
    );

    // Create moving background (vertical scrolling)
    backgroundParallaxLeft = std::make_shared<Background>(
        graphics.GetRenderer(),
        graphics.GetWidth() - 100.0f,  // Positioned at the right side of the screen
        graphics.GetHeight() / 2.0f,   // Centered vertically
        "graphics/bgParallax.bmp",  
        Background::ScrollDirection::UP,   // Scroll direction
        10.0f,  // Scroll speed
        graphics.GetWidth() / 2,  // Width from middle to right edge
        graphics.GetHeight() * 2   // Height to ensure it loops
    );

    // Add backgrounds to level (static first, then moving)
    level.AddGameObject(backgroundStatic);
    level.AddGameObject(backgroundParallaxLeft);

        // Create player
        std::cout << "Creating player..." << std::endl;
        player = std::make_shared<PlayerShip>(graphics.GetRenderer(), 400, 500, collisionManager);
        if (!player) {
            std::cout << "Failed to create player!" << std::endl;
            return false;
        }
        level.AddGameObject(player);
        std::cout << "Player created successfully" << std::endl;
        player->InitializeBulletPool(graphics.GetRenderer());
        // Create loner enemy
        lonerEnemy = std::make_shared<Loner>(
            graphics.GetRenderer(),
            graphics.GetWidth() / 2.0f,
            100.0f,
            collisionManager
        );
        lonerEnemy->InitializeBulletPool(graphics.GetRenderer());

        // Create rusher enemy
        rusherEnemy = std::make_shared<Rusher>(
            graphics.GetRenderer(),
            100.0f,
            graphics.GetHeight() / 2.0f
        );

        // Set render orders (higher numbers render on top)
        lonerEnemy->SetRenderOrder(0);
        rusherEnemy->SetRenderOrder(0);
        player->SetRenderOrder(1);

        level.AddGameObject(player);

        // Store collision components in vector and add to manager
        if (auto playerCollision = player->GetComponent<CollisionComponent>()) {
            auto collisionPtr = std::make_shared<CollisionComponent>(*playerCollision);
            collisionComponents.push_back(collisionPtr);
            collisionManager.AddCollider(collisionPtr);
        }
        
        if (auto lonerCollision = lonerEnemy->GetComponent<CollisionComponent>()) {
            auto collisionPtr = std::make_shared<CollisionComponent>(*lonerCollision);
            collisionComponents.push_back(collisionPtr);
            collisionManager.AddCollider(collisionPtr);
        }
        
        if (auto rusherCollision = rusherEnemy->GetComponent<CollisionComponent>()) {
            auto collisionPtr = std::make_shared<CollisionComponent>(*rusherCollision);
            collisionComponents.push_back(collisionPtr);
            collisionManager.AddCollider(collisionPtr);
        }

        // Create enemy spawner
        enemySpawner = std::make_unique<EnemySpawner>(
            graphics.GetRenderer(), 
            level, 
            collisionManager
        );

        return true;
    }

    void Run() {
        while (isRunning) {
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // Handle input
            inputHandler.HandleInput();
            if (inputHandler.ShouldQuit()) isRunning = false;
            
            // Update player
            if (player) {
                player->HandleInput(inputHandler);
            }

            // Update level
            level.Update(deltaTime);

            // Update collision manager
            collisionManager.Update();
            
            /*
            static float debugTimer = 0;
            debugTimer += deltaTime;
            if (debugTimer >= 1.0f) {
                std::cout << "Active colliders in collision manager: " 
                         << collisionComponents.size() << std::endl;
                debugTimer = 0;
            }*/

            // Update enemy spawner
            if (enemySpawner) {
                enemySpawner->Update(deltaTime);
            }

            // Render
            graphics.Clear();
            level.Render();  // This will render all game objects including player
            graphics.Present();
        }
    }

    ~Game() {
        // Clear collision components
        collisionComponents.clear();
        collisionManager.Clear();
    }

    void RemovePlayer() {
        if (player) {
            player->SetActive(false);
            level.RemoveGameObject(player);
            player.reset();  // Clear the shared pointer
        }
    }
};
