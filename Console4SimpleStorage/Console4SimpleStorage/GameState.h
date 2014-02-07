#pragma once

#include <unordered_map>
#include <string>

#include "PlayerState.h"



class GameState
{
public:
	PlayerState playerStates[PLAYERS];
	int supplyPiles[TOTALCARDS];
	int trash[TOTALCARDS];


	GameState();
};