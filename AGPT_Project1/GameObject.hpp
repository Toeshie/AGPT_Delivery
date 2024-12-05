#pragma once

#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SDL.h>
#include <vector>
#include <memory>
#include "Component.hpp"

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject(float x, float y, int width, int height);
	virtual ~GameObject();

	virtual void Update(float deltaTime);
	virtual void Render(SDL_Renderer* renderer);

	// Component management
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args) {
		T* component = new T(std::forward<Args>(args)...);
		component->SetOwner(this);
		components.push_back(std::unique_ptr<Component>(component));
		component->Start();
		return component;
	}

	template<typename T>
	T* GetComponent() {
		for (auto& component : components) {
			if (typeid(*component) == typeid(T)) {
				return static_cast<T*>(component.get());
			}
		}
		return nullptr;
	}

	template<typename T>
	void RemoveComponent() {
		components.erase(
			std::remove_if(components.begin(), components.end(),
				[](const std::unique_ptr<Component>& component) {
					return typeid(*component) == typeid(T);
				}
			),
			components.end()
		);
	}

	// Transform methods
	float GetX() const { return x; }
	float GetY() const { return y; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	void SetPosition(float newX, float newY) { x = newX; y = newY; }
	void SetSize(int newWidth, int newHeight) { width = newWidth; height = newHeight; }

	void SetActive(bool active);
	bool IsActive() const;

	// Add these new methods
	void SetX(float newX) { x = newX; }
	void SetY(float newY) { y = newY; }
	float GetInitialX() const { return initialX; }
	float GetInitialY() const { return initialY; }

	void SetRenderOrder(int order) { renderOrder = order; }
	int GetRenderOrder() const { return renderOrder; }

protected:
	float x, y;
	float initialX, initialY;
	int width, height;
	std::vector<std::unique_ptr<Component>> components;
	bool isActive = true;
	int renderOrder = 0;
};

#endif // GAMEOBJECT_HPP

