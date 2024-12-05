#pragma once
#include "Component.hpp"
#include <cmath>
#include <algorithm>

class MovementComponent : public Component {
public:
    MovementComponent(float speed = 200.0f) 
        : speed(speed), velocityX(0), velocityY(0), 
          lockX(false), lockY(false),
          boundToScreen(false),
          screenBounds{0, 0, 800, 600} {}

    void Update(float deltaTime) override {
        // Apply axis locking
        float moveX = lockX ? 0 : velocityX;
        float moveY = lockY ? 0 : velocityY;

        // Normalize velocity if moving diagonally
        float length = std::sqrt(moveX * moveX + moveY * moveY);
        if (length > 0) {
            float normalizedX = moveX / length;
            float normalizedY = moveY / length;
            
            float newX = owner->GetX() + normalizedX * speed * deltaTime;
            float newY = owner->GetY() + normalizedY * speed * deltaTime;

            // Apply screen bounds if enabled
            if (boundToScreen) {
                newX = Clamp(newX, 
                    screenBounds.x + owner->GetWidth()/2.0f, 
                    screenBounds.w - owner->GetWidth()/2.0f);
                newY = Clamp(newY, 
                    screenBounds.y + owner->GetHeight()/2.0f, 
                    screenBounds.h - owner->GetHeight()/2.0f);
            }

            owner->SetPosition(newX, newY);
        }
    }

    void SetVelocity(float x, float y) {
        velocityX = x;
        velocityY = y;
    }

    // Axis locking methods
    void LockX(bool lock) { lockX = lock; }
    void LockY(bool lock) { lockY = lock; }
    void SetAxisLock(bool lockXAxis, bool lockYAxis) {
        lockX = lockXAxis;
        lockY = lockYAxis;
    }

    // Screen bounds methods
    void SetBoundToScreen(bool bound) { boundToScreen = bound; }
    void SetScreenBounds(int x, int y, int width, int height) {
        screenBounds = {x, y, width, height};
    }

    // Getters
    bool IsXLocked() const { return lockX; }
    bool IsYLocked() const { return lockY; }
    bool IsBoundToScreen() const { return boundToScreen; }
    float GetSpeed() const { return speed; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }

    std::string GetName() const override { return "MovementComponent"; }

private:
    float Clamp(float value, float min, float max) {
        return std::min(std::max(value, min), max);
    }

    float speed;
    float velocityX;
    float velocityY;
    bool lockX;
    bool lockY;
    bool boundToScreen;
    SDL_Rect screenBounds;
};