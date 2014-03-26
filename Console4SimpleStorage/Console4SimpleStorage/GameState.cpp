#include "GameState.h"

GameState::GameState()
{
	turnCounter = 1;
}

GameState::~GameState()
{
	std::vector<PlayerState>().swap(playerStates);
}

void GameState::initialize()
{
	for (int index = 0; index < INSUPPLY; index++)
	{
		supplyPiles[index] = 0;
		trash[index] = 0;
	}

	for (int index = 0; index < PLAYERS; index++)
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

	if (supplyPiles[CardManager::cardIndexer[PROVINCE]] == 0 || emptyCounter >= SUPPLYPILESTOGO || turnCounter >= MAXTURNS+1) // Check for game end
		return true;
	else
		return false;
}