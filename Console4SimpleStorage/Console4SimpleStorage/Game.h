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

	void initialize();
	void play();

	Game();
};