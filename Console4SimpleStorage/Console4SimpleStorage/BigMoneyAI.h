#pragma once

#include "Option.h"
#include "GameState.h"


class BigMoneyAI{
public:
	BigMoneyAI(CardManager& originalCardManager);
	~BigMoneyAI();
	Option getNextOption(GameState gameState, int playerIndex);

private:
	CardManager cardManager;
};

