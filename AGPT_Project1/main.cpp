
#include <SDL.h>
#include <iostream>
#include "../XennonGame/XennonGame.cpp"

int main(int argc, char* argv[]) {
	std::cout << "Starting Xenon game..." << std::endl;
	
	Game game;
	if (!game.Initialize()) {
		std::cout << "Game initialization failed!" << std::endl;
		return -1;
	}

	game.Run();
	return 0;
}
