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
	int startLoggingAt = 0;
	int numberOfGames = 1;
	int numSims = 0;
	std::cout << "Enter the log number to start with: " << std::endl;
	std::cin >> startLoggingAt;
	std::cout << "Enter the number of games to play: " << std::endl;
	std::cin >> numberOfGames;
	std::cout << "Starting simulations...";
	for (int counter = startLoggingAt; counter < startLoggingAt+numberOfGames; counter++)
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