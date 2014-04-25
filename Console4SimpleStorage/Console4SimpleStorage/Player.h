#pragma once

#include <vector>
#include "UCTMonteCarlo.h"
#include "BigMoneyAI.h"
#include "HumanInputAI.h"


#define UCTAI		0
#define BIGMONEYAI	1
#define HUMANPLAYER	2


class Player
{
public:
	int AIType;
	int playerStateIndex;
	Option getNextOption(GameState& currentState, std::vector<Move> moveHistory);
	
	void initialize(int AIType2);

	Player();
	


private:
	UCTMonteCarlo uct;
	BigMoneyAI bmai;
	HumanInputAI humanAI;
};