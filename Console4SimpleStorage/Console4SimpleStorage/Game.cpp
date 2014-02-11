#include <random>
#include <ctime>
#include <set>
#include <iostream>

#include "Game.h"


Game::Game()
{

}

void Game::initialize()
{
	srand ( (unsigned int)time(NULL) );

	gameState.initialize(PLAYERS);
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

		// Starting deck, hand, and setting stateIndexes
		for (int index = 0; index < 4; index++)
		{
			gameState.playerStates[index].deck[COPPER] = 7;
			gameState.playerStates[index].deck[ESTATE] = 3;
			gameState.playerStates[index].endTurn();
			players[index].stateIndex = index;
			players[index].setCardManager(cardManager);
		}
	}
}

void Game::play()
{
	int turnCounter = 1;
	bool finished = false;
	while (!finished)
	{
		for (int index = 0; index < 4; index++)
		{
			Option option = players[index].getNextOption(gameState);
			if (option.type == 0)	//End turn
			{
				// Clean-up cards
				for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
				{
					gameState.playerStates[index].endTurn();
				}
				std::cout << "Player " << index << " ended turn " << std::endl;
				continue;
			}
			else if (option.type == 2) // Buy card
			{
				gameState.playerStates[index].discard[cardManager.cardIndexer[option.card]] += 1;	// Add to player discard
				gameState.supplyPiles[cardManager.cardIndexer[option.card]] -= 1;					// Remove from supply
				std::cout << "Player " << index << " bought " << cardManager.cardLookupByIndex[option.card].name << std::endl;
			}
			
			if (gameState.gameFinished()) // Check for game end
			{
				finished = true;
				std::cout << "Player " << index << " ";
				break;
			}
		}

		turnCounter++;
		if (turnCounter >= 41)
		{
			finished = true;
			std::cout << "Time ";
		}
	}

	std::cout << "ended game on turn " << turnCounter << std::endl;
	for (int playerIndex = 0; playerIndex < PLAYERS; playerIndex++)
	{
		std::cout << "Player " << playerIndex+1 << " VP: " << gameState.playerStates[playerIndex].calculateVictoryPoints(cardManager) << std::endl;
	}
}

