#pragma once

#include <vector>
#include "UCTMonteCarlo.h"
#include "BigMoneyAI.h"
#include "HumanInputAI.h"
#include "FlatUCB.h"
#include "RandomAI.h"
#include "SingleWitchAI.h"
#include "DoubleWitchAI.h"
#include "FlatUCBMod.h"



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
	FlatUCB flatUCB;
	RandomAI randomAI;
	SingleWitchAI singleWitch;
	DoubleWitchAI doubleWitch;
	FlatUCBMod flatUCBMod;

};