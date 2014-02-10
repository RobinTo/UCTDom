#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "PlayerState.h"
#include "CardManager.h"



class GameState
{
public:
	CardManager cardManager;
	std::vector<PlayerState> playerStates;
	int supplyPiles[INSUPPLY];
	int trash[INSUPPLY];

	void initialize();

	GameState();
};