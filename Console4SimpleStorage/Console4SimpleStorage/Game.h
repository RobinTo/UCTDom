#pragma once

#include <vector>
#include "Player.h"
#include "GameState.h"
#include "CardManager.h"
#include "Move.h"

#define PLAYERS		2


class Game
{
public:
	GameState gameState;
	CardManager cardManager;
	Player players[PLAYERS];
	std::string logString;
	std::vector<Move> moveHistory;

	void initialize( int simulations);
	void play();
	void writeToFile(std::string outputFileName);

	Game();
};