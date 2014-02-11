#pragma once

#include "UCT.h"

class Player
{
public:
	int stateIndex;
	Option getNextOption(GameState& currentState);
	CardManager cardManager;
	
	void setCardManager(CardManager& cardManagerRef);

	Player();
	
private:
	UCT uct;
};