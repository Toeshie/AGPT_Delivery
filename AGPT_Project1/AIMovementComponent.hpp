#pragma once
#include "Component.hpp"

class AIMovementComponent : public Component {
public:
    enum class MovementPattern {
        HORIZONTAL,
        VERTICAL
    };

    AIMovementComponent(float speed = 100.0f, MovementPattern pattern = MovementPattern::HORIZONTAL) 
        : speed(speed), pattern(pattern), direction(1),
          screenBounds{0, 0, 800, 600} {} // Default screen size for movement bounds

    void Update(float deltaTime) override {
        float newX = owner->GetX();
        float newY = owner->GetY();

        switch (pattern) {
            case MovementPattern::HORIZONTAL:
                newX += speed * direction * deltaTime;
                // Check horizontal bounds
                if (newX - owner->GetWidth()/2 <= screenBounds.x) {
                    newX = screenBounds.x + owner->GetWidth()/2;
                    direction = 1; // Reverse direction
                }
                else if (newX + owner->GetWidth()/2 >= screenBounds.w) {
                    newX = screenBounds.w - owner->GetWidth()/2;
                    direction = -1; // Reverse direction
                }
                break;

            case MovementPattern::VERTICAL:
                newY += speed * direction * deltaTime;
                // Check vertical bounds
                if (newY - owner->GetHeight()/2 <= screenBounds.y) {
                    newY = screenBounds.y + owner->GetHeight()/2;
                    direction = 1; // Reverse direction
                }
                else if (newY + owner->GetHeight()/2 >= screenBounds.h) {
                    newY = screenBounds.h - owner->GetHeight()/2;
                    direction = -1; // Reverse direction
                }
                break;
        }

        owner->SetPosition(newX, newY);
    }

    void SetPattern(MovementPattern newPattern) { pattern = newPattern; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }
    void ReverseDirection() { direction *= -1; }

    // Set custom screen bounds
    void SetScreenBounds(int x, int y, int width, int height) {
        screenBounds = {x, y, width, height};
    }

    std::string GetName() const override { return "AIMovementComponent"; }

private:
    float speed;
    MovementPattern pattern;
    int direction;  // 1 or -1 for direction
    SDL_Rect screenBounds;  // Screen boundaries
};
