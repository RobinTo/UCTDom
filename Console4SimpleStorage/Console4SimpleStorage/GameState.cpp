#include "GameState.h"

GameState::GameState()
{
	
}

void GameState::initialize()
{
	for (int index = 0; index < INSUPPLY; index++)
	{
		supplyPiles[index] = 0;
		trash[index] = 0;
	}

	cardManager.initialize();
}