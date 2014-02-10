#include "GameState.h"

GameState::GameState()
{
	
}

void GameState::initialize(int players, int inSupply)
{
	for (int index = 0; index < INSUPPLY; index++)
	{
		supplyPiles[index] = 0;
		trash[index] = 0;
	}

	for (int index = 0; index < players; index++)
	{
		playerStates.push_back(PlayerState());
	}
}