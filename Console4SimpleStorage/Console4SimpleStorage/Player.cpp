#include "Player.h"

Player::Player()
{
}

void Player::initialize(int simulations, int AIType2)
{
	numSims = simulations;
	AIType = AIType2;
}


Option Player::getNextOption(GameState& currentState, std::vector<Move> moveHistory)
{
	Option option;
	if (AIType == 0)
		option = uct.doUCT(numSims, playerStateIndex, currentState, moveHistory);
	else
		option = bmai.getNextOption(currentState, playerStateIndex);

	

	return option;
}