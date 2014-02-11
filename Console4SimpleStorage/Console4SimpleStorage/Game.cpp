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

	// Always in supply
	gameState.supplyPiles[cardManager.cardIndexer[COPPER]] = 32;
	gameState.supplyPiles[cardManager.cardIndexer[SILVER]] = 40;
	gameState.supplyPiles[cardManager.cardIndexer[GOLD]] = 30;
	gameState.supplyPiles[cardManager.cardIndexer[ESTATE]] = 12;
	gameState.supplyPiles[cardManager.cardIndexer[DUCHY]] = 12;
	gameState.supplyPiles[cardManager.cardIndexer[PROVINCE]] = 12;
	gameState.supplyPiles[cardManager.cardIndexer[CURSE]] = 30;

	// Randomize ten cards for the supply
	/*std::set<int> cardIndexes;
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
	}*/

	// Starting deck, hand, and setting stateIndexes
	for (int index = 0; index < PLAYERS; index++)
	{
		gameState.playerStates[index].deck[COPPER] = 7;
		gameState.playerStates[index].deck[ESTATE] = 3;
		gameState.playerStates[index].endTurn();
		players[index].stateIndex = index;
		players[index].setCardManager(cardManager);
	}
}

void Game::play()
{
	bool finished = false;
	while (!finished)
	{
		std::cout << "Turn: " << gameState.turnCounter << std::endl;
		// For each player, play turn
		for (int index = 0; index < PLAYERS; index++)
		{
			// While player's turn is not finished, keep playing
			Option option;
			do
			{
				option = players[index].getNextOption(gameState);
				if (option.type == END_TURN)
				{
					// Clean-up cards
					gameState.playerStates[players[index].stateIndex].endTurn();
					std::cout << "Player " << index << " ended turn " << std::endl;
				}
				else if (option.type == BUY)
				{
					std::cout << "Player " << index << " Money: " << gameState.playerStates[players[index].stateIndex].calculateCurrentMoney(cardManager);
					gameState.playerStates[players[index].stateIndex].buyCard(cardManager, option.card);
					gameState.supplyPiles[cardManager.cardIndexer[option.card]] -= 1;								// Remove from supply
					std::cout << " bought " << cardManager.cardLookupByIndex[option.card].name << std::endl;
				}
			} while (option.type != END_TURN);
			
			// When player's turn is finished, check for game end
			
		}

		gameState.turnCounter++;
		if (gameState.gameFinished())
		{
			finished = true;
		}
		/*if (turnCounter >= 41)
		{
			finished = true;
			std::cout << "Time ";
		}*/
	}

	std::cout << "ended game on turn " << gameState.turnCounter << std::endl;
	for (int index = 0; index < PLAYERS; index++)
	{
		std::cout << "Player " << index << " VP: " << gameState.playerStates[players[index].stateIndex].calculateVictoryPoints(cardManager) << std::endl;
	}
}

