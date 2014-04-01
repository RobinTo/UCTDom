#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

#include "PlayerState.h"



class GameState
{
public:
	std::vector<PlayerState> playerStates;
	int supplyPiles[INSUPPLY];
	int trash[INSUPPLY];
	int turnCounter;

	void initialize();
	bool gameFinished();
	int earlyMidLateGame();

	GameState();
	~GameState();
};