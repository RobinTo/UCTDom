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

void Game::initialize(int simulations)
{
	moveHistory.reserve(1000);
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
	//gameState.supplyPiles[cardManager.cardIndexer[GARDENS]] = 8;
	gameState.supplyPiles[cardManager.cardIndexer[FESTIVAL]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[MONEYLENDER]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[SMITHY]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[VILLAGE]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[MARKET]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[LABORATORY]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[WITCH]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[BUREAUCRAT]] = 10;
	gameState.supplyPiles[cardManager.cardIndexer[REMODEL]] = 10;

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
	}
	players[0].initialize( cardManager, simulations, 0);	// TODO: More dynamic/flexible way of setting playerAI
	players[1].initialize( cardManager, simulations, 1);
	players[0].playerStateIndex = 0;
	players[1].playerStateIndex = 1;
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
				option = players[index].getNextOption(gameState, moveHistory);
				int tempIndex = 0;
				if (option.type == END_TURN)
				{
					gameState.playerStates[players[index].playerStateIndex].endTurn();

					Move move(option, players[index].playerStateIndex);
					move.moveString = "----Player" + std::to_string(players[index].playerStateIndex) + "-Ended-turn" + std::to_string(gameState.turnCounter) + "------------------";
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				else if (option.type == BUY)
				{
					if (gameState.supplyPiles[cardManager.cardIndexer[option.absoluteCardId]] <= 0)
						std::cout << "---ERROR. No more cards in supply - " << cardManager.cardLookup[option.absoluteCardId].name << std::endl;

					gameState.playerStates[players[index].playerStateIndex].actions = 0;
					gameState.playerStates[players[index].playerStateIndex].buyCard(cardManager, option.absoluteCardId);
					gameState.supplyPiles[cardManager.cardIndexer[option.absoluteCardId]] -= 1;								// Remove from supply

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Bought " + cardManager.cardLookup[move.absoluteCardId].name;
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				else if (option.type == TRASH)
				{
					if (gameState.playerStates[players[index].playerStateIndex].hand[cardManager.cardIndexer[option.absoluteCardId]] <= 0)
						std::cout << "---ERROR. Card not in hand - " << cardManager.cardLookup[option.absoluteCardId].name << std::endl;

					gameState.playerStates[players[index].playerStateIndex].hand[cardManager.cardIndexer[option.absoluteCardId]]--;
					gameState.trash[cardManager.cardIndexer[option.absoluteCardId]]++;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Trashed " + cardManager.cardLookup[move.absoluteCardId].name;
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				else if (option.type == GAIN)
				{
					if (gameState.supplyPiles[cardManager.cardIndexer[option.absoluteCardId]] <= 0)
						std::cout << "---ERROR. No more cards in supply - " << cardManager.cardLookup[option.absoluteCardId].name << std::endl;

					gameState.playerStates[players[index].playerStateIndex].discard[cardManager.cardIndexer[option.absoluteCardId]] ++;
					gameState.supplyPiles[cardManager.cardIndexer[option.absoluteCardId]] --;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Gained " + cardManager.cardLookup[move.absoluteCardId].name;
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;

				}
				else if (option.type == ACTION)
				{
					gameState.playerStates[players[index].playerStateIndex].playCard(cardManager, option.absoluteCardId);

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Played " + cardManager.cardLookup[move.absoluteCardId].name;
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;

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
					case VILLAGE:
						gameState.playerStates[players[index].playerStateIndex].drawCards(1);
						gameState.playerStates[players[index].playerStateIndex].actions += 2;
						break;
					case MARKET:
						gameState.playerStates[players[index].playerStateIndex].drawCards(1);
						gameState.playerStates[players[index].playerStateIndex].actions += 1;
						gameState.playerStates[players[index].playerStateIndex].buys += 1;
						break;
					case LABORATORY:
						gameState.playerStates[players[index].playerStateIndex].drawCards(2);
						gameState.playerStates[players[index].playerStateIndex].actions += 1;
						break;
					case WITCH:
						gameState.playerStates[players[index].playerStateIndex].drawCards(2);
						if (PLAYERS > 1)
						{
							tempIndex = players[index].playerStateIndex == PLAYERS - 1 ? 0 : players[index].playerStateIndex + 1;
							while (tempIndex != players[index].playerStateIndex)
							{
								if (gameState.supplyPiles[cardManager.cardIndexer[CURSE]] > 0)
								{
									gameState.playerStates[players[tempIndex].playerStateIndex].discard[cardManager.cardIndexer[CURSE]]++;
									gameState.supplyPiles[cardManager.cardIndexer[CURSE]]--;
								}
								tempIndex++;
								if (tempIndex >= PLAYERS)
									tempIndex = 0;
							}
						}
						break;
					case BUREAUCRAT:
						if (gameState.supplyPiles[cardManager.cardIndexer[SILVER]] > 0)
						{
							gameState.playerStates[players[index].playerStateIndex].addToTopOfDeck(cardManager.cardIndexer[SILVER]);
							gameState.supplyPiles[cardManager.cardIndexer[SILVER]] --;
						}
						tempIndex = players[index].playerStateIndex == PLAYERS - 1 ? 0 : players[index].playerStateIndex + 1;
						while (tempIndex != players[index].playerStateIndex)
						{
							if (gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[ESTATE]] > 0)
							{
								gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[ESTATE]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(cardManager.cardIndexer[ESTATE]);
							}
							else if (gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[DUCHY]] > 0)
							{
								gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[DUCHY]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(cardManager.cardIndexer[DUCHY]);
							}
							else if (gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[PROVINCE]] > 0)
							{
								gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[PROVINCE]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(cardManager.cardIndexer[PROVINCE]);
							}
							else if (gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[GARDENS]] > 0)
							{
								gameState.playerStates[tempIndex].hand[cardManager.cardIndexer[GARDENS]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(cardManager.cardIndexer[GARDENS]);
							}
							tempIndex++;
							if (tempIndex >= PLAYERS)
								tempIndex = 0;
						}
						break;
					case REMODEL:
						// Could add some security, that next option must be a trash option (if more cards in hand)
						// XOR could add that instead of asking getnextoption, we ask, get trashoption.
						break;
					default:
						std::cout << "Error, no action card found" << std::endl;
						break;
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
	std::ofstream file;
	file.open(outputFileName, std::ios::app);
	file << "----------" << std::endl;
	for (int index = 0; index < PLAYERS; index++)
	{
		std::cout << "Player " << index << " VP: " << gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints(cardManager) << std::endl;
		file << "Player " << index << " VP: " << gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints(cardManager) << std::endl;
	}
	file << logString << std::endl;
	file << gameState.turnCounter - 1 << ":" << "2:" << gameState.playerStates[players[0].playerStateIndex].calculateVictoryPoints(cardManager) << std::endl;

	file.close();
}
