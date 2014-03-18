#include "Player.h"

Player::Player()
{
}

void Player::initialize(std::vector<Node*>& emptyNodes, CardManager& cardManagerRef, int simulations, int AIType2)
{
	//uct.setNodes(emptyNodes);
	cardManager = cardManagerRef;
	uct.cardManager = cardManagerRef;
	bmai.cardManager = cardManagerRef;
	numSims = simulations;
	AIType = AIType2;
}


Option Player::getNextOption(GameState& currentState)
{
	Option option;
	if (AIType == 0)
		option = uct.doUCT(numSims, playerStateIndex, currentState);
	else
		option = bmai.getNextOption(currentState, playerStateIndex);

	

	return option;
}