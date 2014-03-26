#pragma once

#include "Option.h"
#include "GameState.h"
#include <vector>
#include <iostream>
#include <map>
#include "Move.h"

class HumanInputAI
{
public:
	CardManager cm;
	HumanInputAI();
	~HumanInputAI();
	Option getNextOption(GameState gameState, int playerIndex, std::vector<Move> moveHistory);
private:
	std::vector<Option> getActionOptions(GameState* gameState, const int(&hand)[INSUPPLY]);
	std::vector<Option> getBuyOptions(GameState* gameState, int currentMoney);
	int getCurrentMoney(GameState* gameState, int playerIndex);
};