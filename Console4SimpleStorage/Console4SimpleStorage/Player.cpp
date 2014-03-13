#include "Player.h"

Player::Player()
{
}

void Player::initialize(std::vector<Node*>& emptyNodes, CardManager& cardManagerRef, int simulations)
{
	//uct.setNodes(emptyNodes);
	cardManager = cardManagerRef;
	uct.cardManager = cardManagerRef;
	numSims = simulations;
}


Option Player::getNextOption(GameState& currentState)
{
	Option option = uct.doUCT(numSims, playerStateIndex, currentState);
	return option;
}