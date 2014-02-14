#pragma once

#include "Player.h"
#include "GameState.h"
#include "CardManager.h"

#define PLAYERS		1


class Game
{
public:
	GameState gameState;
	CardManager cardManager;
	Player players[PLAYERS];
	std::string logString;

	void initialize(std::vector<Node*>& emptyNodes, int simulations);
	void play();
	void writeToFile(std::string outputFileName);

	Game();
};