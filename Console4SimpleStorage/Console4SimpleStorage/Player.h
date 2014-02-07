#pragma once

#include "UCT.h"
#include "Option.h"

class Player
{
public:
	int stateIndex;
	Option getNextOption(GameState& currentState);

	Player();

private:
	UCT uct;
};