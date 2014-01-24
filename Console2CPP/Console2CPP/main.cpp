#include <iostream>

#include "game.h"

int main()
{
	std::cout << "Dominion AI using UCT and Monte Carlo." << std::endl;
	
	Game game;
	game.initialize();
	game.prepareCards();
	game.loopGame();


	int t;
	std::cin >> t;
	return 0;
}