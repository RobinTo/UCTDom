#pragma once

#include <vector>
#include "UCTMonteCarlo.h"
#include "BigMoneyAI.h"
#include "Move.h"

class Player
{
public:
	int AIType;
	int playerStateIndex;
	Option getNextOption(GameState& currentState, std::vector<Move> moveHistory);
	CardManager cardManager;
	
	void initialize( CardManager& cardManagerRef, int simulations, int AIType2);

	Player();
	


private:
	UCTMonteCarlo uct;
	BigMoneyAI bmai;
	int numSims;
};