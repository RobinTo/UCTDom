#include <iostream>

#include "Game.h"


int main()
{
	for (int counter = 0; counter < 40; counter ++)
	{
		Game game;

		game.initialize();
		game.play();


		std::cout << "Game over" << std::endl;

	}
	int input = 0;
	std::cin >> input;

	return 0;
}