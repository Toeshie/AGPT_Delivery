#pragma once
#include <string>
#include <SDL.h>

class GameObject;

class Component {
public:
    Component() : owner(nullptr), isEnabled(true) {}
    virtual ~Component() = default;

    // Core functions
    virtual void Start() {} // Called when component is first added
    virtual void Update(float deltaTime) {} // Called every frame
    virtual void Render(SDL_Renderer* renderer) {} // Called during render phase

    // Enable/Disable functionality
    void Enable() { isEnabled = true; }
    void Disable() { isEnabled = false; }
    void SetEnabled(bool enabled) { isEnabled = enabled; }
    bool IsEnabled() const { return isEnabled; }

    // Owner management
    void SetOwner(GameObject* gameObject) { owner = gameObject; }
    GameObject* GetOwner() const { return owner; }

    //Component identification
    virtual std::string GetName() const { return "Component"; }

protected:
    GameObject* owner;
    bool isEnabled;
};
