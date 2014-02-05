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
	std::unordered_map<int, std::string> cardName;
	std::unordered_map<int, int> cardCost;
	std::unordered_map<int, std::string> cardType;


	GameState();
};