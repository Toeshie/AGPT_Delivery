#include "GameObject.hpp"

GameObject::GameObject(float x, float y, int width, int height)
	: x(x), y(y), initialX(x), initialY(y), width(width), height(height) {
}

GameObject::~GameObject() {
	components.clear();
}

void GameObject::Update(float deltaTime) {
	// Update only enabled components
	for (auto& component : components) {
		if (component->IsEnabled()) {
			component->Update(deltaTime);
		}
	}
}

void GameObject::Render(SDL_Renderer* renderer) {
	// Let components handle rendering
	for (auto& component : components) {
		if (component->IsEnabled()) {
			component->Render(renderer);
		}
	}
}

void GameObject::SetActive(bool active) {
	isActive = active;
	// Use SetEnabled instead of Enable
	for (auto& component : components) {
		if (component) {
			component->SetEnabled(active);
		}
	}
}

bool GameObject::IsActive() const {
	return isActive;
}


