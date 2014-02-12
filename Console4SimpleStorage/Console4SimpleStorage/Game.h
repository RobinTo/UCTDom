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
	std::string fileName;

	void initialize(std::string outputFileName);
	void play();

	Game();
};