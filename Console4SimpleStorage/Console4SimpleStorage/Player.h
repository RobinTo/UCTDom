#pragma once

#include "UCT.h"

class Player
{
public:
	int stateIndex;
	void playTurn(GameState& currentState);

	Player();

private:
	UCT uct;
};