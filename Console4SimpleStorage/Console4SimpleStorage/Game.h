#pragma once

#include "Player.h"
#include "GameState.h"

#define PLAYERS		4


class Game
{
public:
	GameState gameState;
	Player players[PLAYERS];

	void initialize();
	void play();

	Game();
};