#include <iostream>
#include <thread>
#include <random>
#include <ctime>
#include <fstream>

#include "Game.h"

#define SIMULATIONS 100000
#define NODES		8
#define GAMES		1000
#define MULTITHREAD 0

void playGame(Game& game)
{
	game.initialize( SIMULATIONS);
	game.play();
	std::cout << "Game over" << std::endl;
}

void writeToFile(Game& game, std::string outputFileName)
{
	game.writeToFile(outputFileName);
}
/*
void run1Thread()
{
	// Delete the old logfile
	remove("log.txt");

	// Allocate nodes
	//NodePool nodePool;
	//nodePool.allocateNodes(NODES);

	// Run GAMES runs
	for (int counter = 0; counter < GAMES; counter++)
	{
		Game game;
		std::thread first1(playGame, std::ref(game));
		first1.join();
		game.writeToFile("log.txt");
	}
}

void run4Threads()
{
	// Delete the 4 temp logfiles.
	remove("log1.txt");
	remove("log2.txt");
	remove("log3.txt");
	remove("log4.txt");

	// Allocate nodes

	// Run GAMES runs
	for (int counter = 0; counter < GAMES / 4; counter++)
	{
		Game game1, game2, game3, game4;
		std::thread first1(playGame, std::ref(game1), nodePool.getRange(0, NODES / 4 - 1));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));						// Not to avoid race conditions, but to avoid the same random seeds.
		std::thread second1(playGame, std::ref(game2), nodePool.getRange(NODES / 4, 2 * (NODES / 4) - 1));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::thread third1(playGame, std::ref(game3), nodePool.getRange(2 * (NODES / 4), 3 * (NODES / 4) - 1));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::thread fourth1(playGame, std::ref(game4), nodePool.getRange(3 * (NODES / 4), NODES - 1));

		first1.join();
		second1.join();
		third1.join();
		fourth1.join();

		game1.writeToFile("log1.txt");
		game2.writeToFile("log2.txt");
		game3.writeToFile("log3.txt");
		game4.writeToFile("log4.txt");


		nodePool.resetNodes();
	}

	// Append all to one log file.
	std::ofstream file;
	std::ifstream file1, file2, file3, file4;
	std::string temp = "";

	file.open("log-" + std::to_string(SIMULATIONS) + "sims.txt", std::ios::app);
	file1.open("log1.txt");
	file2.open("log2.txt");
	file3.open("log3.txt");
	file4.open("log4.txt");

	while (!file1.eof())
	{
		temp = "";
		file1 >> temp;
		file << temp << std::endl;
	}
	file1.close();

	while (!file2.eof())
	{
		temp = "";
		file2 >> temp;
		file << temp << std::endl;
	}
	file2.close();

	while (!file3.eof())
	{
		temp = "";
		file3 >> temp;
		file << temp << std::endl;
	}
	file3.close();

	while (!file4.eof())
	{
		temp = "";
		file4 >> temp;
		file << temp << std::endl;
	}
	file4.close();

	file.close();
}
*/
int main()
{
	/*if (MULTITHREAD)
		run4Threads();
	else
		run1Thread();*/

	remove("log.txt");
	for (int counter = 0; counter < GAMES; counter++)
	{
		Game* game = new Game;
		playGame(*game);
		game->writeToFile("log.txt");
		delete game;
	}


	// Don't close console yet
	int input = 0;
	std::cin >> input;

	return 0;
}