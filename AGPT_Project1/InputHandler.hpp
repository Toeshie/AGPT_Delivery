#pragma once

#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SDL.h>
#include <unordered_map>
#include <vector>

class InputHandler {
public:
	InputHandler();
	~InputHandler();

	void Update();  // Called every frame to capture input events
	void HandleInput();

	// Keyboard functions
	bool IsKeyPressed(SDL_Scancode key);
	bool IsKeyReleased(SDL_Scancode key);

	// Game controller functions
	bool IsButtonPressed(int button);
	bool IsButtonReleased(int button);
	int GetJoystickAxis(SDL_GameControllerAxis axis); 

	// D-pad functions
	bool IsDPadPressed(SDL_GameControllerButton dpadButton);

	// Mouse functions
	bool IsMouseButtonPressed(int button);  
	void GetMousePosition(int& x, int& y);  

	bool ShouldQuit() const { return quit; }

private:
	const Uint8* keyboardState;
	std::unordered_map<int, bool> keyPressed;
	std::unordered_map<int, bool> keyReleased;

	SDL_GameController* controller;
	std::unordered_map<int, bool> buttonPressed;

	int mouseX, mouseY;
	std::unordered_map<int, bool> mouseButtonPressed;

	bool quit = false;

	std::vector<SDL_GameController*> controllers;

	void HandleKeyboardInput();
	void HandleGamepadInput();
	void HandleMouseInput();
};

#endif // INPUTHANDLER_HPP
