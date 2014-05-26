#pragma once


#include "Option.h"
#include "GameState.h"


class RandomAI
{
public:
	CardManager cm;
	RandomAI();
	~RandomAI();
	Option getNextOption(GameState gameState, int playerIndex);
	std::vector<Option> getOptions(GameState* gameState, int currentMoney, int playerIndex);
	int getCurrentMoney(GameState* gameState, int playerIndex);
};

