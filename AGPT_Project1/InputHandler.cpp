#include "InputHandler.hpp"
#include <iostream>

InputHandler::InputHandler() {
	// Open all available controllers
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			SDL_GameController* controller = SDL_GameControllerOpen(i);
			if (controller) {
				controllers.push_back(controller);
				std::cout << "Opened controller: " << SDL_GameControllerName(controller) << std::endl;
			} else {
				std::cerr << "Could not open gamecontroller " << i << ": " << SDL_GetError() << std::endl;
			}
		}
	}
}

InputHandler::~InputHandler() {
	// Close all controllers
	for (auto controller : controllers) {
		SDL_GameControllerClose(controller);
	}
}

void InputHandler::HandleInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					quit = true;
				}
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
					std::cout << "A button pressed" << std::endl;
					// Handle A button press
				}
				break;
			case SDL_CONTROLLERBUTTONUP:
				if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
					std::cout << "A button released" << std::endl;
					// Handle A button release
				}
				break;
			case SDL_CONTROLLERAXISMOTION:
				if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
					std::cout << "Left stick X axis: " << event.caxis.value << std::endl;
					// Handle left stick horizontal movement
				}
				if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
					std::cout << "Left stick Y axis: " << event.caxis.value << std::endl;
					// Handle left stick vertical movement
				}
				break;
		}
	}
}

int InputHandler::GetJoystickAxis(SDL_GameControllerAxis axis) {
    if (!controllers.empty()) {
        return SDL_GameControllerGetAxis(controllers[0], axis);
    }
    return 0; // Return 0 if no controller is connected
}

bool InputHandler::IsButtonPressed(int button) {
    if (!controllers.empty()) {
        return SDL_GameControllerGetButton(controllers[0], static_cast<SDL_GameControllerButton>(button));
    }
    return false; // Return false if no controller is connected
}
