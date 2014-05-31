#include "Player.h"

Player::Player()
{
}

void Player::initialize(int AIType2)
{
	AIType = AIType2;
}


Option Player::getNextOption(GameState& currentState, std::vector<Move> moveHistory)
{
	Option option;
	if (AIType == UCTAI)
		option = uct.getNextOption(playerStateIndex, currentState, moveHistory);
	else if (AIType == BIGMONEYAI)
		option = bmai.getNextOption(currentState, playerStateIndex);
	else if (AIType == HUMANPLAYER)
		option = humanAI.getNextOption(currentState, playerStateIndex, moveHistory);
	else if (AIType == FLATUCB)
		option = flatUCB.getNextOption(playerStateIndex, currentState, moveHistory);
	else if (AIType == RANDOM)
		option = randomAI.getNextOption(currentState, playerStateIndex);
	else if (AIType == SINGLEWITCH)
		option = singleWitch.getNextOption(currentState, playerStateIndex);
	else if (AIType == DOUBLEWITCH)
		option = doubleWitch.getNextOption(currentState, playerStateIndex);
	else if (AIType == FLATUCBMOD)
		option = flatUCBMod.getNextOption(playerStateIndex, currentState, moveHistory);
	
	return option;
}