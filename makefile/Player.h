#pragma once

#include "UCTMonteCarlo.h"
#include "BigMoneyAI.h"

class Player
{
public:
	int AIType;
	int playerStateIndex;
	Option getNextOption(GameState& currentState);
	CardManager cardManager;
	
	void initialize(std::vector<Node*>& emptyNodes, CardManager& cardManagerRef, int simulations, int AIType2);

	Player();
	


private:
	UCTMonteCarlo uct;
	BigMoneyAI bmai;
	int numSims;
};