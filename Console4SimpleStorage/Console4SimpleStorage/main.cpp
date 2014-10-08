#include <iostream>
#include <thread>
#include <random>
#include <ctime>
#include <fstream>

#include "Game.h"



void playGame(Game& game)
{
	game.initialize();
	game.play();
	std::cout << "Game over" << std::endl;
}

void writeToFile(Game& game, std::string outputFileName)
{
	game.writeToFile(outputFileName);
}

int main()
{	
	for (int counter = 0; counter < GAMES; counter++)
	{
		std::string logFile = "log" + std::to_string(counter) + ".csv";
		std::string moveHistoryFile = "moveHistory" + std::to_string(counter) + ".txt";
		remove(logFile.c_str());
		remove(moveHistoryFile.c_str());

		Game* game = new Game;
		playGame(*game);
		game->writeCSVLog(logFile);
		game->writeMoveHistoryToFile(moveHistoryFile);
		delete game;
	}

	// Don't close console yet
	int input = 0;
	std::cin >> input;

	return 0;
}