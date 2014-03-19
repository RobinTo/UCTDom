#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "PlayerState.h"



class GameState
{
public:
	std::vector<PlayerState> playerStates;
	int supplyPiles[INSUPPLY];
	int trash[INSUPPLY];

	int turnCounter;

	void initialize(int players);
	bool gameFinished();

	GameState();
	~GameState();
};