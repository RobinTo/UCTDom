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
	
	return option;
}