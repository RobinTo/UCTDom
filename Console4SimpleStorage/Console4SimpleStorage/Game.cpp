#include <random>
#include <ctime>
#include <set>

#include "Game.h"


Game::Game()
{

}

void Game::initialize()
{
	srand (time(NULL));

	gameState.initialize(PLAYERS, INSUPPLY);
	cardManager.initialize();

	if (PLAYERS == 4)
	{
		// Always in supply
		gameState.supplyPiles[cardManager.cardIndexer[COPPER]] = 32;
		gameState.supplyPiles[cardManager.cardIndexer[SILVER]] = 40;
		gameState.supplyPiles[cardManager.cardIndexer[GOLD]] = 30;
		gameState.supplyPiles[cardManager.cardIndexer[ESTATE]] = 12;
		gameState.supplyPiles[cardManager.cardIndexer[DUCHY]] = 12;
		gameState.supplyPiles[cardManager.cardIndexer[PROVINCE]] = 12;
		gameState.supplyPiles[cardManager.cardIndexer[CURSE]] = 30;

		// Randomize ten cards for the supply
		std::set<int> cardIndexes;
		do
		{
			int newCard = rand() % 25 + 7;
			cardIndexes.insert(newCard);
		} while (cardIndexes.size() < 10);

		int currentIndex = 7;
		for (std::set<int>::iterator iterator = cardIndexes.begin(); iterator != cardIndexes.end(); ++iterator)
		{
			cardManager.cardIndexer[*iterator] = currentIndex;
			cardManager.cardLookupByIndex[currentIndex] = cardManager.cardLookup[*iterator];
			if (*iterator == GARDENS)
				gameState.supplyPiles[currentIndex] = 12;
			else
				gameState.supplyPiles[currentIndex] = 10;
			currentIndex++;
		}

		// Starting hand, and setting stateIndexes
		for (int index = 0; index < 4; index++)
		{
			gameState.playerStates[index].deck[COPPER] = 7;
			gameState.playerStates[index].deck[ESTATE] = 3;
			players[index].stateIndex = index;
			players[index].setCardManager(cardManager);
		}
	}
}

void Game::play()
{
	bool paused = false;
	int turnCounter = 0;
	bool finished = false;
	while (!paused && !finished)
	{
		for (int index = 0; index < 4; index++)
		{
			Option option = players[index].getNextOption(gameState);
			if (option.type == 0)	//End turn
				continue;
			else if (option.type == 2) // Buy card
			{
				gameState.playerStates[index].discard[cardManager.cardIndexer[option.card]] += 1;
				gameState.supplyPiles[cardManager.cardIndexer[option.card]] -= 1;
			}
			
		}

		turnCounter++;
		if (turnCounter >= 40)
			finished = true;
	}
}

