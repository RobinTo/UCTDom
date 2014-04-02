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

void Game::initialize()
{
	moveHistory.reserve(1000);
	srand((unsigned int)time(NULL));
	gameState.initialize();
	cardManager.initialize();

	std::vector<Card> cardsInGame;
	if (BUREAUCRATINGAME)
		cardsInGame.push_back(CardManager::cardLookup[BUREAUCRAT]);
	if (FESTIVALINGAME)
		cardsInGame.push_back(CardManager::cardLookup[FESTIVAL]);
	if (GARDENSINGAME)
		cardsInGame.push_back(CardManager::cardLookup[GARDENS]);
	if (LABORATORYINGAME)
		cardsInGame.push_back(CardManager::cardLookup[LABORATORY]);
	if (MARKETINGAME)
		cardsInGame.push_back(CardManager::cardLookup[MARKET]);
	if (MONEYLENDERINGAME)
		cardsInGame.push_back(CardManager::cardLookup[MONEYLENDER]);
	if (REMODELINGAME)
		cardsInGame.push_back(CardManager::cardLookup[REMODEL]);
	if (SMITHYINGAME)
		cardsInGame.push_back(CardManager::cardLookup[SMITHY]);
	if (THIEFINGAME)
		cardsInGame.push_back(CardManager::cardLookup[THIEF]);
	if (VILLAGEINGAME)
		cardsInGame.push_back(CardManager::cardLookup[VILLAGE]);
	if (WITCHINGAME)
		cardsInGame.push_back(CardManager::cardLookup[WITCH]);
	if (WOODCUTTERINGAME)
		cardsInGame.push_back(CardManager::cardLookup[WOODCUTTER]);

	// Supply
	gameState.supplyPiles[CardManager::cardIndexer[COPPER]] = 46;
	gameState.supplyPiles[CardManager::cardIndexer[SILVER]] = 40;
	gameState.supplyPiles[CardManager::cardIndexer[GOLD]] = 30;
	gameState.supplyPiles[CardManager::cardIndexer[ESTATE]] = 8;
	gameState.supplyPiles[CardManager::cardIndexer[DUCHY]] = 8;
	gameState.supplyPiles[CardManager::cardIndexer[PROVINCE]] = 8;
	gameState.supplyPiles[CardManager::cardIndexer[CURSE]] = 10;
	// Initialization to zero
	gameState.supplyPiles[CardManager::cardIndexer[WOODCUTTER]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[GARDENS]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[FESTIVAL]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[MONEYLENDER]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[SMITHY]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[VILLAGE]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[MARKET]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[LABORATORY]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[WITCH]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[BUREAUCRAT]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[REMODEL]] = 0;
	gameState.supplyPiles[CardManager::cardIndexer[THIEF]] = 0;


	for (std::vector<Card>::iterator it = cardsInGame.begin(); it != cardsInGame.end(); ++it)
	{
		if (it->id != GARDENS)
			gameState.supplyPiles[CardManager::cardIndexer[it->id]] = 10;
		else
			gameState.supplyPiles[CardManager::cardIndexer[it->id]] = 8;
	}


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
		CardManager::cardIndexer[*iterator] = currentIndex;
		CardManager::cardLookupByIndex[currentIndex] = CardManager::cardLookup[*iterator];
		if (*iterator == GARDENS)
			gameState.supplyPiles[currentIndex] = 12;
		else
			gameState.supplyPiles[currentIndex] = 10;
		currentIndex++;
	}*/

	// Starting deck, hand, and setting stateIndices
	for (int index = 0; index < PLAYERS; index++)
	{
		// Set initial deck.
		gameState.playerStates[index].deck[cardManager.cardIndexer[COPPER]] = 7;
		gameState.playerStates[index].deck[cardManager.cardIndexer[ESTATE]] = 3;
		gameState.playerStates[index].endTurn();
	}

	players[0].initialize(0);	// TODO: More dynamic/flexible way of setting playerAI
	players[1].initialize(2);
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
				std::cout << "Player " << index << " with money:" << gameState.playerStates[players[index].playerStateIndex].calculateCurrentMoney() << ", buys:" << gameState.playerStates[players[index].playerStateIndex].buys << ", actions:" << gameState.playerStates[players[index].playerStateIndex].actions << std::endl;
				std::cout << "Hand: " << gameState.playerStates[players[index].playerStateIndex].printPile( gameState.playerStates[players[index].playerStateIndex].hand);
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
					if (gameState.supplyPiles[CardManager::cardIndexer[option.absoluteCardId]] <= 0)
						std::cout << "---ERROR. No more cards in supply - " << CardManager::cardLookup[option.absoluteCardId].name << std::endl;

					gameState.playerStates[players[index].playerStateIndex].actions = 0;
					gameState.playerStates[players[index].playerStateIndex].buyCard( option.absoluteCardId);
					gameState.supplyPiles[CardManager::cardIndexer[option.absoluteCardId]] -= 1;								// Remove from supply

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Bought " + CardManager::cardLookup[move.absoluteCardId].name;
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				else if (option.type == TRASH)
				{
					if (gameState.playerStates[players[index].playerStateIndex].hand[CardManager::cardIndexer[option.absoluteCardId]] <= 0)
						std::cout << "---ERROR. Card not in hand - " << CardManager::cardLookup[option.absoluteCardId].name << std::endl;

					gameState.playerStates[players[index].playerStateIndex].hand[CardManager::cardIndexer[option.absoluteCardId]]--;
					gameState.trash[CardManager::cardIndexer[option.absoluteCardId]]++;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Trashed " + CardManager::cardLookup[move.absoluteCardId].name;
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				else if (option.type == GAIN)
				{
					if (gameState.supplyPiles[CardManager::cardIndexer[option.absoluteCardId]] <= 0)
						std::cout << "---ERROR. No more cards in supply - " << CardManager::cardLookup[option.absoluteCardId].name << std::endl;

					gameState.playerStates[players[index].playerStateIndex].discard[CardManager::cardIndexer[option.absoluteCardId]] ++;
					gameState.supplyPiles[CardManager::cardIndexer[option.absoluteCardId]] --;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Gained " + CardManager::cardLookup[move.absoluteCardId].name;
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;

				}
				else if (option.type == ACTION)
				{
					gameState.playerStates[players[index].playerStateIndex].playCard( option.absoluteCardId);

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Played " + CardManager::cardLookup[move.absoluteCardId].name;
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
						if (gameState.playerStates[players[index].playerStateIndex].hand[CardManager::cardIndexer[COPPER]] > 0)
						{
							gameState.playerStates[players[index].playerStateIndex].hand[CardManager::cardIndexer[COPPER]] --;
							gameState.trash[CardManager::cardIndexer[COPPER]] ++;
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
								if (gameState.supplyPiles[CardManager::cardIndexer[CURSE]] > 0)
								{
									gameState.playerStates[players[tempIndex].playerStateIndex].discard[CardManager::cardIndexer[CURSE]]++;
									gameState.supplyPiles[CardManager::cardIndexer[CURSE]]--;
								}
								tempIndex++;
								if (tempIndex >= PLAYERS)
									tempIndex = 0;
							}
						}
						break;
					case BUREAUCRAT:
						if (gameState.supplyPiles[CardManager::cardIndexer[SILVER]] > 0)
						{
							gameState.playerStates[players[index].playerStateIndex].addToTopOfDeck(CardManager::cardIndexer[SILVER]);
							gameState.supplyPiles[CardManager::cardIndexer[SILVER]] --;
						}
						tempIndex = players[index].playerStateIndex == PLAYERS - 1 ? 0 : players[index].playerStateIndex + 1;
						while (tempIndex != players[index].playerStateIndex)
						{
							if (gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[ESTATE]] > 0)
							{
								gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[ESTATE]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(CardManager::cardIndexer[ESTATE]);
							}
							else if (gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[DUCHY]] > 0)
							{
								gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[DUCHY]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(CardManager::cardIndexer[DUCHY]);
							}
							else if (gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[PROVINCE]] > 0)
							{
								gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[PROVINCE]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(CardManager::cardIndexer[PROVINCE]);
							}
							else if (gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[GARDENS]] > 0)
							{
								gameState.playerStates[tempIndex].hand[CardManager::cardIndexer[GARDENS]] --;
								gameState.playerStates[tempIndex].addToTopOfDeck(CardManager::cardIndexer[GARDENS]);
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
					case THIEF:
						// Do the following for all enemy players
						if (PLAYERS > 1)
						{
							tempIndex = players[index].playerStateIndex == PLAYERS - 1 ? 0 : players[index].playerStateIndex + 1;
							Move thiefMove(option, players[tempIndex].playerStateIndex);
							while (tempIndex != players[index].playerStateIndex)
							{
								gameState.playerStates[tempIndex].flipThiefCards(thiefMove.absoluteCardId, thiefMove.absoluteExtraCardId);

								tempIndex++;
								if (tempIndex >= PLAYERS)
									tempIndex = 0;
							}

							thiefMove.type = THIEFFLIP;
							thiefMove.moveString = "Player" + std::to_string(thiefMove.player) + "-Flipped " + CardManager::cardLookup[thiefMove.absoluteCardId].name + " and " + CardManager::cardLookup[thiefMove.absoluteExtraCardId].name;
							moveHistory.push_back(thiefMove);
							std::cout << thiefMove.moveString << std::endl << std::endl;
						}

						break;
					default:
						std::cout << "Error, no action card found" << std::endl;
						break;
					}

				}
				else if (option.type == THIEFTRASH) // TODO: Support for more than two players
				{
					int otherPlayer = (index == 0) ? 1 : 0;
					gameState.trash[CardManager::cardIndexer[option.absoluteCardId]] ++;
					gameState.playerStates[players[otherPlayer].playerStateIndex].discard[CardManager::cardIndexer[option.absoluteCardId]] --;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Trashed " + CardManager::cardLookup[move.absoluteCardId].name + " from Player" + std::to_string(otherPlayer);
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				else if (option.type == THIEFGAIN) // TODO: Support for more than two players
				{
					int otherPlayer = (index == 0) ? 1 : 0;
					gameState.playerStates[players[index].playerStateIndex].discard[CardManager::cardIndexer[option.absoluteCardId]] ++;
					gameState.playerStates[players[otherPlayer].playerStateIndex].discard[CardManager::cardIndexer[option.absoluteCardId]] --;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Stole " + CardManager::cardLookup[move.absoluteCardId].name + " from Player" + std::to_string(otherPlayer);
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				
			} while (option.type != END_TURN);
			
			logString += std::to_string(static_cast<long long>(gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints()));
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
		std::cout << "Player " << index << " VP: " << gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints() << std::endl;
		file << "Player " << index << " VP: " << gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints() << std::endl;
	}
	file << logString << std::endl;
	file << gameState.turnCounter - 1 << ":" << "2:" << gameState.playerStates[players[0].playerStateIndex].calculateVictoryPoints() << std::endl;

	file.close();
}

void Game::writeMoveHistoryToFile(std::string outputFileName)
{
	std::ofstream file;
	file.open(outputFileName, std::ios::app);
	file << "----------" << std::endl;
	// Write moves
	for (std::vector<Move>::iterator it = moveHistory.begin(); it != moveHistory.end(); ++it)
	{
		file << it->moveString << std::endl;
	}
	// Write end scores
	for (int index = 0; index < PLAYERS; index++)
	{
		file << "Player " << index << " VP: " << gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints() << std::endl;
	}
	file.close();
}
