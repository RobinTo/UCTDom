#pragma once


#include "Option.h"
#include "GameState.h"


class DoubleWitchAI
{
public:
	CardManager cm;
	DoubleWitchAI();
	~DoubleWitchAI();
	Option getNextOption(GameState gameState, int playerIndex);
	std::vector<Option> getBuyOptions(GameState* gameState, int currentMoney, int playerIndex);
	std::vector<Option> getActionOptions(GameState* gameState, int playerIndex);
	int getCurrentMoney(GameState* gameState, int playerIndex);
};

