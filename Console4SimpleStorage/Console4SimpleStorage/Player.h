#pragma once

#include "UCTMonteCarlo.h"

class Player
{
public:
	int stateIndex;
	Option getNextOption(GameState& currentState);
	CardManager cardManager;
	
	void initialize(std::vector<Node*>& emptyNodes, CardManager& cardManagerRef, int simulations);

	Player();
	


private:
	UCTMonteCarlo uct;
	int numSims;
};