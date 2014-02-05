#include "Game.h"


Game::Game()
{

}

void Game::initialize()
{
	gameState.cardName[COPPER] = "Copper";
	gameState.cardName[SILVER] = "Silver";
	gameState.cardName[GOLD] = "Gold";
	gameState.cardName[ESTATE] = "Estate";
	gameState.cardName[DUCHY] = "Duchy";
	gameState.cardName[PROVINCE] = "Province";
	gameState.cardName[SUPERCARD] = "SuperCard";
	
	gameState.cardType[COPPER] = "Treasure";
	gameState.cardType[SILVER] = "Treasure";
	gameState.cardType[GOLD] = "Treasure";
	gameState.cardType[ESTATE] = "Victory";
	gameState.cardType[DUCHY] = "Victory";
	gameState.cardType[PROVINCE] = "Victory";
	gameState.cardType[SUPERCARD] = "Action";

	gameState.cardCost[COPPER] = 0;
	gameState.cardCost[SILVER] = 3;
	gameState.cardCost[GOLD] = 6;
	gameState.cardCost[ESTATE] = 2;
	gameState.cardCost[DUCHY] = 5;
	gameState.cardCost[PROVINCE] = 8;
	gameState.cardCost[SUPERCARD] = 1;

	gameState.supplyPiles[COPPER] = 32;
	gameState.supplyPiles[SILVER] = 40;
	gameState.supplyPiles[GOLD] = 30;
	gameState.supplyPiles[ESTATE] = 12;
	gameState.supplyPiles[DUCHY] = 12;
	gameState.supplyPiles[PROVINCE] = 12;

	for (int index = 0; index < 4; index++)
	{
		gameState.playerStates[index].deck[COPPER] = 7;
		gameState.playerStates[index].deck[ESTATE] = 3;
		players[index].stateIndex = index;
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
			players[index].playTurn(gameState);

			inputController.takeInput();
		}

		turnCounter++;
		if (turnCounter >= 40)
			finished = true;
	}
}

