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

	// Supply
	gameState.supplyPiles[CardManager::cardIndexer[COPPER]] = 46;
	gameState.supplyPiles[CardManager::cardIndexer[SILVER]] = 40;
	gameState.supplyPiles[CardManager::cardIndexer[GOLD]] = 30;
	gameState.supplyPiles[CardManager::cardIndexer[ESTATE]] = 8;
	gameState.supplyPiles[CardManager::cardIndexer[DUCHY]] = 8;
	gameState.supplyPiles[CardManager::cardIndexer[PROVINCE]] = 8;
	gameState.supplyPiles[CardManager::cardIndexer[CURSE]] = 10;
	if (WOODCUTTERINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[WOODCUTTER]] = 10;
	if (GARDENSINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[GARDENS]] = 8;
	if (FESTIVALINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[FESTIVAL]] = 10;
	if (MONEYLENDERINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[MONEYLENDER]] = 10;
	if (SMITHYINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[SMITHY]] = 10;
	if (VILLAGEINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[VILLAGE]] = 10;
	if (MARKETINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[MARKET]] = 10;
	if (LABORATORYINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[LABORATORY]] = 10;
	if (WITCHINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[WITCH]] = 10;
	if (BUREAUCRATINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[BUREAUCRAT]] = 10;
	if (REMODELINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[REMODEL]] = 10;
	if (THIEFINGAME)
		gameState.supplyPiles[CardManager::cardIndexer[THIEF]] = 10;


	// Starting deck, hand, and setting stateIndices
	for (int index = 0; index < PLAYERS; index++)
	{
		// Set initial deck.
		gameState.playerStates[index].deck[cardManager.cardIndexer[COPPER]] = 7;
		gameState.playerStates[index].deck[cardManager.cardIndexer[ESTATE]] = 3;
		gameState.playerStates[index].endTurn();
	}

	players[0].initialize(FIRSTPLAYERAI);
	players[1].initialize(SECONDPLAYERAI);
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
				
				clock_t begin = clock();
				option = players[index].getNextOption(gameState, moveHistory);
				clock_t end = clock();
				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

				
				int tempIndex = 0;
				if (option.type == END_TURN)
				{
					gameState.playerStates[players[index].playerStateIndex].endTurn();

					Move move(option, players[index].playerStateIndex);
					move.moveString = "----Player" + std::to_string(players[index].playerStateIndex) + "-Ended-turn" + std::to_string(gameState.turnCounter) + "------------------" + " Secs: " + std::to_string(elapsed_secs);
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
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Bought " + CardManager::cardLookup[move.absoluteCardId].name + " Secs: " + std::to_string(elapsed_secs);
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
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Trashed " + CardManager::cardLookup[move.absoluteCardId].name + " Secs: " + std::to_string(elapsed_secs);
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
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Gained " + CardManager::cardLookup[move.absoluteCardId].name + " Secs: " + std::to_string(elapsed_secs);
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;

				}
				else if (option.type == ACTION)
				{
					gameState.playerStates[players[index].playerStateIndex].playCard( option.absoluteCardId);

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Played " + CardManager::cardLookup[move.absoluteCardId].name + " Secs: " + std::to_string(elapsed_secs);
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
						if (WITCHNERF)
						{
							gameState.playerStates[players[index].playerStateIndex].drawCards(1);
						}
						else
						{
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
							thiefMove.moveString = "Player" + std::to_string(thiefMove.player) + "-Flipped " + CardManager::cardLookup[thiefMove.absoluteCardId].name + " and " + CardManager::cardLookup[thiefMove.absoluteExtraCardId].name + " Secs: " + std::to_string(elapsed_secs);
							moveHistory.push_back(thiefMove);
							std::cout << thiefMove.moveString << std::endl << std::endl;
						}

						break;
					default:
						std::cout << "Error, no action card found" << std::endl;
						break;
					}

				}
				else if (option.type == THIEFTRASH) 
				{
					int otherPlayer = (index == 0) ? 1 : 0;
					gameState.trash[CardManager::cardIndexer[option.absoluteCardId]] ++;
					gameState.playerStates[players[otherPlayer].playerStateIndex].discard[CardManager::cardIndexer[option.absoluteCardId]] --;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Trashed " + CardManager::cardLookup[move.absoluteCardId].name + " from Player" + std::to_string(otherPlayer) + " Secs: " + std::to_string(elapsed_secs);
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				else if (option.type == THIEFGAIN) 
				{
					int otherPlayer = (index == 0) ? 1 : 0;
					gameState.playerStates[players[index].playerStateIndex].discard[CardManager::cardIndexer[option.absoluteCardId]] ++;
					gameState.playerStates[players[otherPlayer].playerStateIndex].discard[CardManager::cardIndexer[option.absoluteCardId]] --;

					Move move(option, players[index].playerStateIndex);
					move.moveString = "Player" + std::to_string(players[index].playerStateIndex) + "-Stole " + CardManager::cardLookup[move.absoluteCardId].name + " from Player" + std::to_string(otherPlayer) + " Secs: " + std::to_string(elapsed_secs);
					moveHistory.push_back(move);
					std::cout << move.moveString << std::endl << std::endl;
				}
				
			} while (option.type != END_TURN);
			

			logString += std::to_string(static_cast<long long>(gameState.playerStates[players[index].playerStateIndex].calculateVictoryPoints()));
			logString += ":";
			
		}
		turnLog.push_back(createLogItem(&gameState));
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

void Game::writeCSVLog(std::string outputFileName){
	std::ofstream file;
	file.open(outputFileName, std::ios::app);
	file << "turn, p1Score, p2Score, p1Copper, p1Silver, p1Gold, p1Estate, p1Duchy, p1Province, p1Curse, p1Woodcutter, p1Gardens, p1Festival, p1Moneylender, p1Smithy, p1Village, p1Market, p1Laboratory, p1Witch, p1Remodel, p1Bureaucrat, p1Thief, p2Copper, p2Silver, p2Gold, p2Estate, p2Duchy, p2Province, p2Curse, p2Woodcutter, p2Gardens, p2Festival, p2Moneylender, p2Smithy, p2Village, p2Market, p2Laboratory, p2Witch, p2Remodel, p2Bureaucrat, p2Thief";
	file << std::endl;
	for(int i = 0; i < turnLog.size(); i++){
		file << turnLog.at(i).turn << ", " << turnLog.at(i).p1Score << ", " << turnLog.at(i).p2Score << ", " << turnLog.at(i).p1Copper << ", " << turnLog.at(i).p1Silver << ", " << turnLog.at(i).p1Gold << ", " << turnLog.at(i).p1Estate << ", " << turnLog.at(i).p1Duchy << ", " << turnLog.at(i).p1Province << ", " << turnLog.at(i).p1Curse << ", " << turnLog.at(i).p1Woodcutter << ", " << turnLog.at(i).p1Gardens << ", " << turnLog.at(i).p1Festival << ", " << turnLog.at(i).p1Moneylender << ", " << turnLog.at(i).p1Smithy << ", " << turnLog.at(i).p1Village << ", " << turnLog.at(i).p1Market << ", " << turnLog.at(i).p1Laboratory << ", " << turnLog.at(i).p1Witch << ", " << turnLog.at(i).p1Remodel << ", " << turnLog.at(i).p1Bureaucrat << ", " << turnLog.at(i).p1Thief << ", " << turnLog.at(i).p2Copper << ", " << turnLog.at(i).p2Silver << ", " << turnLog.at(i).p2Gold << ", " << turnLog.at(i).p2Estate << ", " << turnLog.at(i).p2Duchy << ", " << turnLog.at(i).p2Province << ", " << turnLog.at(i).p2Curse << ", " << turnLog.at(i).p2Woodcutter << ", " << turnLog.at(i).p2Gardens << ", " << turnLog.at(i).p2Festival << ", " << turnLog.at(i).p2Moneylender << ", " << turnLog.at(i).p2Smithy << ", " << turnLog.at(i).p2Village << ", " << turnLog.at(i).p2Market << ", " << turnLog.at(i).p2Laboratory << ", " << turnLog.at(i).p2Witch << ", " << turnLog.at(i).p2Remodel << ", " << turnLog.at(i).p2Bureaucrat << ", " << turnLog.at(i).p2Thief;
		file << std::endl;
	}
	file.close();
}

LogItem Game::createLogItem(GameState* gameState){
	LogItem l;

	l.turn = gameState->turnCounter;

	l.p1Score = gameState->playerStates.at(0).calculateVictoryPoints();
	l.p2Score  = gameState->playerStates.at(1).calculateVictoryPoints();

	l.p1Copper = gameState->playerStates.at(0).countCardType(COPPER);
	l.p1Silver = gameState->playerStates.at(0).countCardType(SILVER);
	l.p1Gold = gameState->playerStates.at(0).countCardType(GOLD);
	l.p1Estate = gameState->playerStates.at(0).countCardType(ESTATE);
	l.p1Duchy = gameState->playerStates.at(0).countCardType(DUCHY);
	l.p1Province = gameState->playerStates.at(0).countCardType(PROVINCE);
	l.p1Curse = gameState->playerStates.at(0).countCardType(CURSE);
	l.p1Woodcutter = gameState->playerStates.at(0).countCardType(WOODCUTTER);
	l.p1Gardens = gameState->playerStates.at(0).countCardType(GARDENS);
	l.p1Festival = gameState->playerStates.at(0).countCardType(FESTIVAL);
	l.p1Moneylender = gameState->playerStates.at(0).countCardType(MONEYLENDER);
	l.p1Smithy = gameState->playerStates.at(0).countCardType(SMITHY);
	l.p1Village = gameState->playerStates.at(0).countCardType(VILLAGE);
	l.p1Market = gameState->playerStates.at(0).countCardType(MARKET);
	l.p1Laboratory = gameState->playerStates.at(0).countCardType(LABORATORY);
	l.p1Witch = gameState->playerStates.at(0).countCardType(WITCH);
	l.p1Remodel = gameState->playerStates.at(0).countCardType(REMODEL);
	l.p1Bureaucrat = gameState->playerStates.at(0).countCardType(BUREAUCRAT);
	l.p1Thief = gameState->playerStates.at(0).countCardType(THIEF);

	l.p2Copper = gameState->playerStates.at(1).countCardType(COPPER);
	l.p2Silver = gameState->playerStates.at(1).countCardType(SILVER);
	l.p2Gold = gameState->playerStates.at(1).countCardType(GOLD);
	l.p2Estate = gameState->playerStates.at(1).countCardType(ESTATE);
	l.p2Duchy = gameState->playerStates.at(1).countCardType(DUCHY);
	l.p2Province = gameState->playerStates.at(1).countCardType(PROVINCE);
	l.p2Curse = gameState->playerStates.at(1).countCardType(CURSE);
	l.p2Woodcutter = gameState->playerStates.at(1).countCardType(WOODCUTTER);
	l.p2Gardens = gameState->playerStates.at(1).countCardType(GARDENS);
	l.p2Festival = gameState->playerStates.at(1).countCardType(FESTIVAL);
	l.p2Moneylender = gameState->playerStates.at(1).countCardType(MONEYLENDER);
	l.p2Smithy = gameState->playerStates.at(1).countCardType(SMITHY);
	l.p2Village = gameState->playerStates.at(1).countCardType(VILLAGE);
	l.p2Market = gameState->playerStates.at(1).countCardType(MARKET);
	l.p2Laboratory = gameState->playerStates.at(1).countCardType(LABORATORY);
	l.p2Witch = gameState->playerStates.at(1).countCardType(WITCH);
	l.p2Remodel = gameState->playerStates.at(1).countCardType(REMODEL);
	l.p2Bureaucrat = gameState->playerStates.at(1).countCardType(BUREAUCRAT);
	l.p2Thief = gameState->playerStates.at(1).countCardType(THIEF);

	return l;
}