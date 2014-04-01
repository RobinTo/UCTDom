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

int GameState::earlyMidLateGame()
{
	int provincesLeft = supplyPiles[CardManager::cardIndexer[PROVINCE]];	// Checking provinces left
	int emptyPiles = 3-SUPPLYPILESTOGO;									// Initialize with value, based on number of supplypiles that should be emptied. See Configurations.h TODO: DYNAMIC
	std::vector<int> cardPiles;
	for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
	{
		if (supplyPiles[cardIndex] == 0)
			emptyPiles++;													// Finding totally empty piles
		cardPiles.push_back(supplyPiles[cardIndex]);
	}
	std::sort(cardPiles.begin(), cardPiles.end());
	int cardsLeftInPiles = cardPiles[0] + cardPiles[1] + cardPiles[2] + cardPiles[3] + cardPiles[4];		// Finding how many total cards are left in the piles with the three lowest stacks. + 2, due to two being empty. TODO: DYNAMIC

	int earlyMidLate = -1;
	if (provincesLeft < 6 || emptyPiles > 0 || cardsLeftInPiles < 9)	// Lategame test
		earlyMidLate = 2;
	else if (provincesLeft < 8 || cardsLeftInPiles < 16)				// Midgame test
		earlyMidLate = 1;
	else																// Earlygame if not late- or midgame
		earlyMidLate = 0;

	return earlyMidLate;
}