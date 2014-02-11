#include "GameState.h"

GameState::GameState()
{
	turnCounter = 0;
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
	if (turnCounter >= 40)
		return true;
	// Count supplypiles
	int emptyCounter = 0;
	for (int index = 0; index < INSUPPLY; index++)
	{
		if (supplyPiles[index] == 0)
		{
			emptyCounter++;
		}
	}

	if (supplyPiles[PROVINCE] == 0 || emptyCounter >= 3) // Check for game end
		return true;
	else
		return false;
}