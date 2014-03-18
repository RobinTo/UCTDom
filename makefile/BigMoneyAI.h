#pragma once

#include "Option.h"
#include "GameState.h"


class BigMoneyAI{
public:
	CardManager cardManager;

	BigMoneyAI();
	~BigMoneyAI();
	Option getNextOption(GameState gameState, int playerIndex);
};

