#include "GameState.h"

GameState::GameState()
{
	turnCounter = 1;
}

void GameState::initialize(int players)
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

bool GameState::gameFinished()
{
	// Count supplypiles
	int emptyCounter = 0;
	for (int index = 0; index < INSUPPLY; index++)
	{
		if (supplyPiles[index] == 0)
		{
			emptyCounter++;
		}
	}

	if (supplyPiles[PROVINCE] == 0 || emptyCounter >= 3 || turnCounter >= 3+1) // Check for game end
		return true;
	else
		return false;
}