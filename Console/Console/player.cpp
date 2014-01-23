#include "player.h"
#include "card.h"
#include <iostream>

player::player()
{
	
}

void player::initialize(gameState* gameStatePtr)
{
	playerStatePtr = new playerState();
	gameStatePtr->playerStatePtrs.push_back(playerStatePtr);
	uctmc.initialize(gameStatePtr);
}

void player::playTurn(gameState* gameStatePtr)
{
	bool aiDone = false;
	card* cardToBuy;

	while (!aiDone)
	{
		cardToBuy = uctmc.next(&aiDone, gameStatePtr);
	}
	while(!playerStatePtr->hand.empty())
	{
		playerStatePtr->playCard(playerStatePtr->hand.front());
		playerStatePtr->hand.pop_front();
	}
	playerStatePtr->endTurn();

	std::cout << "Buy: " << cardToBuy->name << " costing " << cardToBuy->cost << std::endl;
	std::cout << "Ended turn." << std::endl;
}