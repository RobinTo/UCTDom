#pragma once

#include <vector>
#include "Player.h"

class Game
{
public:
	GameState gameState;
	CardManager cardManager;
	Player players[PLAYERS];
	std::string logString;
	std::vector<Move> moveHistory;

	void initialize();
	void play();
	void writeToFile(std::string outputFileName);
	void writeMoveHistoryToFile(std::string outputFileName);

	Game();
};