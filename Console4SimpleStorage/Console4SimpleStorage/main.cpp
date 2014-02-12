#include <iostream>
#include <thread>
#include <random>
#include <ctime>
#include <fstream>

#include "Game.h"


void playGame(std::string outputFileName)
{
	Game game;

	game.initialize(outputFileName);
	game.play();


	std::cout << "Game over" << std::endl;
}

int main()
{
	/*for (int counter = 0; counter < 40; counter ++)
	{
		Game game;

		game.initialize();
		game.play();


		std::cout << "Game over" << std::endl;

	}*/
	std::ofstream file;
	std::ifstream file1, file2, file3, file4;
	file.open("log.txt", std::ios::app);
	file1.open("log1.txt");
	file2.open("log2.txt");
	file3.open("log3.txt");
	file4.open("log4.txt");
	std::string temp;
	file1 >> temp;
	file << temp << std::endl;
	file1.close();

	file2 >> temp;
	file << temp << std::endl;
	file2.close();

	file3 >> temp;
	file << temp << std::endl;
	file3.close();
	
	file4 >> temp;
	file << temp << std::endl;
	file4.close();

	file.close();

	std::thread first(playGame, "log1.txt");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::thread second(playGame, "log2.txt");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::thread third(playGame, "log3.txt");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::thread fourth(playGame, "log4.txt");

	


	int input = 0;
	std::cin >> input;

	return 0;
}