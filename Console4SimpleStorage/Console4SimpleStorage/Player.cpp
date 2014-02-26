#include "Player.h"

Player::Player()
{
}

void Player::initialize(std::vector<Node*>& emptyNodes, CardManager& cardManagerRef, int simulations)
{
	//uct.setNodes(emptyNodes);
	cardManager = cardManagerRef;
	uct.initialize(cardManagerRef, simulations);
}


Option Player::getNextOption(GameState& currentState)
{
	Option option = uct.getNextOption(currentState, stateIndex);
	return option;
}