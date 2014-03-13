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
	gameState.supplyPiles[cardManager.cardIndexer[COPPER]] = 46;
	gameState.supplyPiles[cardManager.cardIndexer[SILVER]] = 40;
	gameState.supplyPiles[cardManager.cardIndexer[GOLD]] = 30;
	gameState.supplyPiles[cardManager.cardIndexer[ESTATE]] = 8;
	gameState.supplyPiles[cardManager.cardIndexer[DUCHY]] = 8;
	gameState.supplyPiles[cardManager.cardIndexer[PROVINCE]] = 8;
	gameState.supplyPiles[cardManager.cardIndexer[CURSE]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[WOODCUTTER]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[GARDENS]] = 8;
	gameState.supplyPiles[cardManager.cardIndexer[FESTIVAL]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[MONEYLENDER]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[SMITHY]] = 10;

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

	// Starting deck, hand, and setting stateIndices
	for (int index = 0; index < PLAYERS; index++)
	{
		gameState.playerStates[index].deck[cardManager.cardIndexer[COPPER]] = 7;
		gameState.playerStates[index].deck[cardManager.cardIndexer[ESTATE]] = 3;
		gameState.playerStates[index].endTurn();
		players[index].playerStateIndex = index;
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
			
			if (gameState.gameFinished())
				break;
			// While player's turn is not finished, keep playing
			Option option;
			do
			{
				std::cout << "Player " << index << " with money:" << gameState.playerStates[players[index].playerStateIndex].calculateCurrentMoney(cardManager) << ", buys:" << gameState.playerStates[players[index].playerStateIndex].buys << ", actions:" << gameState.playerStates[players[index].playerStateIndex].actions << std::endl;
				std::cout << "Hand: " << gameState.playerStates[players[index].playerStateIndex].printPile(cardManager, gameState.playerStates[players[index].playerStateIndex].hand);
				option = players[index].getNextOption(gameState);
				if (option.type == END_TURN)
				{
					// Clean-up cards
					
					gameState.playerStates[players[index].playerStateIndex].endTurn();
					std::cout << "------------Ended turn---------------------" << std::endl << std::endl;
				}
				else if (option.type == BUY)
				{
					gameState.playerStates[players[index].playerStateIndex].actions = 0;
					gameState.playerStates[players[index].playerStateIndex].buyCard(cardManager, option.absoluteCardId);
					gameState.supplyPiles[cardManager.cardIndexer[option.absoluteCardId]] -= 1;								// Remove from supply
					std::cout << "---Bought " << cardManager.cardLookup[option.absoluteCardId].name << std::endl;
				}
				else if (option.type == ACTION)
				{
					gameState.playerStates[players[index].playerStateIndex].playCard(cardManager, option.absoluteCardId);
					std::cout << "---Played " << cardManager.cardLookup[option.absoluteCardId].name << std::endl;
					switch (option.absoluteCardId)
					{
					case WOODCUTTER:
						gameState.playerStates[players[index].playerStateIndex].buys += 1;
						break;
					case FESTIVAL:
						gameState.playerStates[players[index].playerStateIndex].buys += 1;
						gameState.playerStates[players[index].playerStateIndex].actions += 2;
						break;
					case MONEYLENDER:
						if (cardManager.cardIndexer[COPPER] > 0)
						{
							gameState.playerStates[players[index].playerStateIndex].hand[cardManager.cardIndexer[COPPER]] --;
							gameState.trash[cardManager.cardIndexer[COPPER]] ++;
							gameState.playerStates[players[index].playerStateIndex].spentMoney -= 3; // TODO: Careful here. This is a hack.
						}
						break;
					case SMITHY:
						gameState.playerStates[players[index].playerStateIndex].drawCards(3);
						break;
					default:
						std::cout << "Error, no action card found" << std::endl;
					}
				}
			} while (option.type != END_TURN);
			
			logString += std::to_string(static_cast<long long>(gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints(cardManager)));
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
		std::cout << "Player " << index << " VP: " << gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints(cardManager) << std::endl;
	}

	std::ofstream file;
	file.open(outputFileName, std::ios::app);
	file << logString << std::endl;
	file << gameState.turnCounter - 1 << ":" << "2:" << gameState.playerStates[players[0].playerStateIndex].calculateVictoryPoints(cardManager) << std::endl;

	file.close();
}
