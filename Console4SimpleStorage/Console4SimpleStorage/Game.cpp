#include <random>
#include <set>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include "Game.h"


Game::Game()
{

}

void Game::initialize(std::vector<Node*>& emptyNodes, int simulations)
{
	srand((unsigned int)time(NULL));
	gameState.initialize(PLAYERS);
	cardManager.initialize();

	// Always in supply
	gameState.supplyPiles[cardManager.cardIndexer[COPPER]] = 460;
	gameState.supplyPiles[cardManager.cardIndexer[SILVER]] = 400;
	gameState.supplyPiles[cardManager.cardIndexer[GOLD]] = 300;
	gameState.supplyPiles[cardManager.cardIndexer[ESTATE]] = 80;
	gameState.supplyPiles[cardManager.cardIndexer[DUCHY]] = 80;
	gameState.supplyPiles[cardManager.cardIndexer[PROVINCE]] = 80;
	gameState.supplyPiles[cardManager.cardIndexer[CURSE]] = 100;

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
		players[index].initialize(emptyNodes, cardManager, simulations);
	}
}

void Game::play()
{
	logString = "Turns:";

	while (!gameState.gameFinished())
	{
		std::cout << "Turn: " << gameState.turnCounter << std::endl;
		// For each player, play turn
		for (int index = 0; index < PLAYERS; index++)
		{
			std::cout << "Player " << index << " with money " << gameState.playerStates[players[index].stateIndex].calculateCurrentMoney(cardManager) << std::endl;
			if (gameState.gameFinished())
				break;
			// While player's turn is not finished, keep playing
			Option option;
			do
			{
				option = players[index].getNextOption(gameState);
				if (option.type == END_TURN)
				{
					// Clean-up cards
					
					gameState.playerStates[players[index].stateIndex].endTurn();
					std::cout << "Ended turn " << std::endl;
				}
				else if (option.type == BUY)
				{
					gameState.playerStates[players[index].stateIndex].buyCard(cardManager, option.card);
					gameState.supplyPiles[cardManager.cardIndexer[option.card]] -= 1;								// Remove from supply
					std::cout << "Bought " << cardManager.cardLookupByIndex[option.card].name << std::endl;
				}
			} while (option.type != END_TURN);
			
			logString += std::to_string(static_cast<long long>(gameState.playerStates[players[index].stateIndex].calculateVictoryPoints(cardManager)));
			logString += ":";
			
		}

		gameState.turnCounter++;
	}

	std::cout << "ended game on turn " << gameState.turnCounter << std::endl;
	
}

void Game::writeToFile(std::string outputFileName)
{
	for (int index = 0; index < PLAYERS; index++)
	{
		std::cout << "Player " << index << " VP: " << gameState.playerStates[players[index].stateIndex].calculateVictoryPoints(cardManager) << std::endl;
	}

	std::ofstream file;
	file.open(outputFileName, std::ios::app);
	file << logString << std::endl;
	file << gameState.turnCounter - 1 << ":" << "2:" << gameState.playerStates[players[0].stateIndex].calculateVictoryPoints(cardManager) << std::endl;

	file.close();
}
