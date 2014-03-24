#pragma once


#include "Option.h"
#include "GameState.h"



class BigMoneyAI
{
public:
	CardManager cm;
	BigMoneyAI();
	~BigMoneyAI();
	Option getNextOption(GameState gameState, int playerIndex);
};

