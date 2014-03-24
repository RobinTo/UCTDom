#pragma once

#include "Option.h"
#include "GameState.h"


class GardensAI{
public:
	GardensAI();
	~GardensAI();
	Option getNextOption(GameState gameState, int playerIndex);

private:
	Card findBestBuy(GameState gameState, int playerIndex);
	Card findBestAction(GameState gameState, int playerIndex);
};

