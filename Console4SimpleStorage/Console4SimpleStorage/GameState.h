#pragma once

#include <unordered_map>
#include <string>

#include "PlayerState.h"
#include "CardManager.h"



class GameState
{
public:
	CardManager cardManager;
	PlayerState playerStates[PLAYERS];
	int supplyPiles[TOTALCARDS];
	int trash[TOTALCARDS];


	GameState();
};