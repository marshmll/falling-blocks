#include <iostream>
#include "Game.h"

int main()
{
	// Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	// Init Game engine
	Game game;

	// Game loop
	while (game.isRunning())
	{
		game.update();

		game.render();
	}

	// End of application
	return 0;
}
