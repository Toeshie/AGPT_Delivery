
#pragma once
#include "Component.hpp"

class TransformComponent : public Component {
public:
    TransformComponent(float x = 0.0f, float y = 0.0f) 
        : position{x, y}, scale{1.0f, 1.0f}, rotation(0.0f) {}

    void Update(float deltaTime) override {
        // Update owner's position
        owner->SetPosition(position[0], position[1]);
    }

    void SetPosition(float x, float y) { position[0] = x; position[1] = y; }
    void SetScale(float x, float y) { scale[0] = x; scale[1] = y; }
    void SetRotation(float angle) { rotation = angle; }

    std::string GetName() const override { return "TransformComponent"; }

private:
    float position[2];
    float scale[2];
    float rotation;
};